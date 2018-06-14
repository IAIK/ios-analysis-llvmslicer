#include <llvm/IR/InstIterator.h>
#include "llvm/Analysis/Andersen/DetectParametersPass.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"

#include "llvm/Analysis/Andersen/StackAccessPass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "detect_params"

char DetectParametersPass::ID = 0;
static RegisterPass<DetectParametersPass> X("detect-param", "Detect Parameters", true, true);

DetectParametersPass::InstRegUserSet_t DetectParametersPass::specialPreSets;

std::mutex DetectParametersPass::passLock;

void DetectParametersPass::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();

  AU.addRequired<StackAccessPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
}

bool DetectParametersPass::runOnModule(Module &M) {
  for (auto &F : M.functions()) {
    if (F.isDeclaration() || F.isIntrinsic())
      continue;
    StackOffsets[&F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
    RegisterIndexes[&F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
    ReturnIndexes[&F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
    DEBUG(errs() << "Find parameters for: " << F.getName() << "\n");
    CurrentFunction = &F;
    for (BasicBlock::InstListType::iterator I_it = F.getEntryBlock().begin();
         I_it != F.getEntryBlock().end();
         ++I_it) {
      if (I_it->getOpcode() == Instruction::GetElementPtr) {
        if (ConstantInt *IdxValue = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
          if (isParameterRegister(IdxValue->getZExtValue())) {
            Instruction *LoadInst = nullptr;
            if (readBeforeWrite(&*I_it, LoadInst)) {
              DEBUG(errs() << "PARAMETER: " << I_it->getName() << "(" << LoadInst->getName() << ")\n");
              RegisterIndexes[&F]->insert(ParameterAccessPair_t(IdxValue->getZExtValue(), LoadInst));
            }
          }
        }
      }
    }

    std::set<uint64_t> HandledReturns;
    for (Function::iterator BB_it = F.begin(); BB_it != F.end(); ++BB_it) {
      if (BB_it->getTerminator()->getOpcode() != Instruction::Ret)
        continue;
      for (Instruction *I = BB_it->getTerminator(); I != &BB_it->front(); I = I->getPrevNode()) {
        if (I->getOpcode() != Instruction::Store)
          continue;
        Instruction *Ptr = (Instruction *) I->getOperand(1);
        if (Ptr->getOpcode() != Instruction::GetElementPtr)
          continue;
        ConstantInt *Idx = dyn_cast<ConstantInt>(Ptr->getOperand(2));
        if (!Idx)
          continue;
//            if (Idx->getZExtValue() == 5 || Idx->getZExtValue() ==13) {
        if (Idx->getZExtValue() == 5) {
          if (HandledReturns.find(Idx->getZExtValue()) == HandledReturns.end()) {
            ReturnIndexes[&F]->insert(ParameterAccessPair_t(Idx->getZExtValue(), I));
            HandledReturns.insert(Idx->getZExtValue());
          }
        }
      }

    }

    StackAccessPass::OffsetValueListMap_t &OffsetValues = getAnalysis<StackAccessPass>().getOffsetValues(&F);
    const DominatorTree &DomTree = getAnalysis<DominatorTreeWrapperPass>(F).getDomTree();

    for (StackAccessPass::OffsetValueListMap_t::iterator OV_it = OffsetValues.begin();
         OV_it != OffsetValues.end();
         ++OV_it) {

      InstructionList_t LoadInstruction, StoreInstructions;

      if (!OV_it->second) {
        //FIXME: why can this happen???
        continue;
      }

      for (StackAccessPass::ValueList_t::iterator V_it = OV_it->second->begin();
           V_it != OV_it->second->end();
           ++V_it) {
        assert(isa<Instruction>(*V_it));
        getMemoryOperations((Instruction *) *V_it, LoadInstruction, StoreInstructions);
      }

      bool isParameter = false;
      Instruction *LoadParam = nullptr;
      for (InstructionList_t::iterator L_it = LoadInstruction.begin(); L_it != LoadInstruction.end(); ++L_it) {
        bool dominatesAll = true;
        for (InstructionList_t::iterator S_it = StoreInstructions.begin();
             S_it != StoreInstructions.end(); ++S_it) {
          if (!DomTree.dominates(*L_it, *S_it)) {
            dominatesAll = false;
            break;
          }
        }
        if (dominatesAll) {
          isParameter = true;
          LoadParam = *L_it;
          break;
        }
      }

      if (isParameter) {
        DEBUG(errs() << "PARAMETER: " << OV_it->first << "\n");
//            StackOffsets[&F]->insert(OV_it->first);
        StackOffsets[&F]->insert(ParameterAccessPair_t(OV_it->first, LoadParam));
      }

    }
  }
  return false;
}

//TODO: add SIMD registers here
bool DetectParametersPass::isParameterRegister(uint64_t Idx) {
  if (Idx >= 5 && Idx <= 13) {
    return true;
  }
  return false;
}

bool DetectParametersPass::isStackRegister(uint64_t Idx) {
  return Idx == 3 ? true : false;
}

bool DetectParametersPass::readBeforeWrite(Instruction *Inst, Instruction *&LoadInst) {
  for (Instruction *I = Inst; I != I->getParent()->getTerminator(); I = I->getNextNode()) {
    if (I->getOpcode() == Instruction::Load && I->getOperand(0) == Inst) {
      LoadInst = I;
      break;
    }
    if (I->getOpcode() == Instruction::Call)
      break;
  }

  if (!LoadInst)
    return false;

  return true;

//    for (Value::const_use_iterator U_it = LoadInst->use_begin(); U_it != LoadInst->use_end(); ++U_it) {
//        if (Instruction *Use = dyn_cast<Instruction>(U_it->getUser())) {
//            Use->dump();
//            if (Use->getOpcode() != Instruction::Store)
//                return true;
//            if (Use->getOperand(1) != Inst)
//                return true;
//        }
//    }
//    return false;
}

void DetectParametersPass::getMemoryOperations(Instruction *Address,
                                               InstructionList_t &Load,
                                               InstructionList_t &Store) {
  for (Value::const_use_iterator U_it = Address->use_begin(); U_it != Address->use_end(); ++U_it) {
    if (Instruction *Inst = dyn_cast<Instruction>(U_it->getUser())) {
      //TODO: now only the addresses are considered that are directly converted to a pointer
      //variable offsets won't be recognized here
      switch (Inst->getOpcode()) {
        default:
          break;
//                    assert(false);
        case Instruction::Load:
          Load.push_back(Inst);
          break;
        case Instruction::Store:
          Store.push_back(Inst);
          break;
        case Instruction::IntToPtr:
          getMemoryOperations(Inst, Load, Store);
          break;
      }
    } else {
      assert(false);
    }
  }

}

DetectParametersPass::UserSet_t DetectParametersPass::getRegisterValuesAfterCall(const uint64_t RegNo,
                                                                                 const Instruction *Inst) {
  std::set<const BasicBlock *> visited;
  return getRegisterValuesAfterCall(RegNo, Inst, visited);
}

DetectParametersPass::UserSet_t DetectParametersPass::getRegisterValuesAfterCall(const uint64_t RegNo,
                                                                                 const Instruction *Inst,
                                                                                 std::set<const BasicBlock *> &visited) {

  if (visited.find(Inst->getParent()) != visited.end()) {
    return UserSet_t();
  }
  visited.insert(Inst->getParent());

  const Value *RegPtr = nullptr;
  const BasicBlock &EntryBB = Inst->getParent()->getParent()->getEntryBlock();
  for (BasicBlock::const_iterator I_it = EntryBB.begin(); I_it != EntryBB.end(); ++I_it) {
    if (I_it->getOpcode() == Instruction::GetElementPtr) {
      if (ConstantInt *Idx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
        if (Idx->getZExtValue() == RegNo) {
          RegPtr = &*I_it;
          break;
        }
      } else {
        llvm_unreachable("The index should be a constant int...");
      }
    }
  }

  if (!RegPtr) {
    assert(false);
    UserSet_t Result;
    return Result;
  }

  const Instruction *LastVal = nullptr;

  for (const Instruction *I = Inst; I != Inst->getParent()->getTerminator(); I = I->getNextNode()) {
    if (I->getOpcode() == Instruction::Load && I->getOperand(0) == RegPtr) {
      LastVal = I;
      break;
    }
  }

  if (!LastVal) {
    //We consider only a single successor here because the value should be accessed before a branch instruction (and only those have multiple successors)
    //TODO: check this if this is true (e.g. allocate two object before a branch and access one in each of the paths)
    if (Inst->getParent()->getTerminator()->getOpcode() == Instruction::Br &&
        Inst->getParent()->getTerminator()->getNumOperands() == 1) {
      if (BasicBlock *B = dyn_cast<BasicBlock>(Inst->getParent()->getTerminator()->getOperand(0))) {
        return DetectParametersPass::getRegisterValuesAfterCall(RegNo, &B->front(), visited);
      }
    }
    return UserSet_t();
  }

  UserSet_t Result;
  assert(isa<User>(LastVal));
  //FIXME: change UserSet_t to contain const values
  Result.insert(dyn_cast<User>((Value *) LastVal));
  return Result;
}

DetectParametersPass::UserSet_t DetectParametersPass::getRegisterValuesBeforeCall(const uint64_t RegNo,
                                                                                  const Instruction *Inst,
                                                                                  std::set<const BasicBlock *> &visited,
                                                                                  const bool GetStores) {
  //Prevents infinite loops
  if (visited.find(Inst->getParent()) != visited.end()) {
    return UserSet_t();
  }
  visited.insert(Inst->getParent());

  const Function *F = Inst->getParent()->getParent();
  Value *RegPtr = nullptr;
//    BasicBlock &EntryBB = Inst->getParent()->getParent()->getEntryBlock();
//    for (BasicBlock::iterator I_it = EntryBB.begin(); I_it != EntryBB.end(); ++I_it) {
//        if (I_it->getOpcode() == Instruction::GetElementPtr) {
//            if (ConstantInt *Idx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
//                if (Idx->getZExtValue() == RegNo) {
//                    RegPtr = &*I_it;
//                    break;
//                }
//            } else {
//                llvm_unreachable("The index should be a constant int...");
//            }
//        }
//    }

  const Value *regSet = F->arg_begin();

  for (const_inst_iterator I_it = inst_begin(F); I_it != inst_end(F); ++I_it) {
    if (I_it->getOpcode() == Instruction::GetElementPtr) {
      if (I_it->getOperand(0) != regSet)
        continue;
      if (ConstantInt *Idx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
        if (Idx->getZExtValue() == RegNo) {
          RegPtr = (Instruction *) &*I_it;
          break;
        }
      } else {
        llvm_unreachable("The index should be a constant int...");
      }
    }
  }

  if (!RegPtr) {
//        assert(false);
    UserSet_t Result;
    return Result;
  }

  const Value *LastVal = nullptr;

  for (const Instruction *I = Inst; ; I = I->getPrevNode()) {
    if (I->getOpcode() == Instruction::Store && I->getOperand(1) == RegPtr) {
      if (GetStores)
        LastVal = I;
      else
        LastVal = I->getOperand(0);

      break;
    }
    if (I == &I->getParent()->front())
      break;
  }

  if (!LastVal) {
    UserSet_t predResults;
    for (const_pred_iterator pred_it = pred_begin(Inst->getParent());
         pred_it != pred_end(Inst->getParent()); ++pred_it) {
      UserSet_t R = getRegisterValuesBeforeCall(RegNo, (*pred_it)->getTerminator(), visited, GetStores);
      predResults.insert(R.begin(), R.end());
    }
    if (predResults.size() == 0) {
      assert(true);
    }
    return predResults;
    errs() << RegNo << "\n";
    errs() << Inst->getParent()->getParent()->getName() << "\n";
    errs() << Inst->getParent()->getName() << "\n";
    Inst->dump();
    llvm_unreachable("Can't find store for this register");
  }

  UserSet_t Result;
  assert(isa<User>(LastVal));
  //FIXME: change this to const
  Result.insert(dyn_cast<User>((Value *) LastVal));
  return Result;
}

void DetectParametersPass::setSpecialPreSet(const Instruction *inst, uint64_t RegNo, UserSet_t set) {
  passLock.lock();
  specialPreSets[inst][RegNo] = set;
  passLock.unlock();
}

DetectParametersPass::UserSet_t DetectParametersPass::getRegisterValuesBeforeCall(const uint64_t RegNo,
                                                                                  const Instruction *Inst,
                                                                                  const bool GetStores) {
  std::unique_lock<std::mutex> lock(passLock);
  InstRegUserSet_t::iterator irus_it = specialPreSets.find(Inst);
  if (irus_it != specialPreSets.end()) {
    RegUserSet_t::iterator rus_it = irus_it->second.find(RegNo);
    if (rus_it != irus_it->second.end()) {
      if (GetStores) {
        return rus_it->second;
      } else {
        UserSet_t pre;
        for (auto &u : rus_it->second) {
          assert(isa<const User>(u));
          pre.insert(dyn_cast<User>(u->getOperand(0)));
        }
        return pre;
      }
    }
  }
  std::set<const BasicBlock *> visited;
  UserSet_t Results = DetectParametersPass::getRegisterValuesBeforeCall(RegNo, Inst, visited, GetStores);
  assert(Results.size() || Inst->getParent()->getParent()->getName() == "_EXTERNAL_");
  return Results;
}

DetectParametersPass::ParameterAccessPairSet_t DetectParametersPass::getStackParameters(Function &F,
                                                                                        std::set<uint64_t> SPIdx) {
  std::unique_lock<std::mutex> lock(passLock);
  StackAccessPass::OffsetValueListMap_t OffsetValues;
  StackAccessPass::OffsetMap_t Offsets;
  StackAccessPass *SAP = getAnalysisIfAvailable<StackAccessPass>();
  if (!SAP)
    SAP = &getAnalysis<StackAccessPass>();
  SAP->runOnFunction(F, Offsets, OffsetValues, SPIdx);
  DominatorTreeWrapperPass *DTP = getAnalysisIfAvailable<DominatorTreeWrapperPass>();
  if (!DTP)
    DTP = &getAnalysis<DominatorTreeWrapperPass>(F);
  const DominatorTree &DomTree = DTP->getDomTree();

  ParameterAccessPairSet_t StackParameters;

  for (StackAccessPass::OffsetValueListMap_t::iterator OV_it = OffsetValues.begin();
       OV_it != OffsetValues.end();
       ++OV_it) {

    InstructionList_t LoadInstruction, StoreInstructions;

    for (StackAccessPass::ValueList_t::iterator V_it = OV_it->second->begin();
         V_it != OV_it->second->end();
         ++V_it) {
      assert(isa<Instruction>(*V_it));
      getMemoryOperations((Instruction *) *V_it, LoadInstruction, StoreInstructions);
    }

    bool isParameter = false;
    Instruction *LoadParam = nullptr;
    for (InstructionList_t::iterator L_it = LoadInstruction.begin(); L_it != LoadInstruction.end(); ++L_it) {
      bool dominatesAll = true;
      for (InstructionList_t::iterator S_it = StoreInstructions.begin(); S_it != StoreInstructions.end(); ++S_it) {
        if (!DomTree.dominates(*L_it, *S_it)) {
          dominatesAll = false;
          break;
        }
      }
      if (dominatesAll) {
        isParameter = true;
        LoadParam = *L_it;
        break;
      }
    }

    if (isParameter) {
      DEBUG(errs() << "PARAMETER: " << OV_it->first << "\n");
//            StackOffsets[&F]->insert(OV_it->first);
      StackParameters.insert(ParameterAccessPair_t(OV_it->first, LoadParam));
    }

  }
  return StackParameters;
}
