//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// Details are in a white paper by F. Tip called:
// A survey of program slicing techniques
//===----------------------------------------------------------------------===//

#include <ctype.h>
#include <map>
#include <llvm/IR/PatternMatch.h>
#include <llvm/Analysis/Andersen/StackAccessPass.h>
#include <llvm/ADT/StringExtras.h>
#include <llvm/ProfileData/InstrProfReader.h>

#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include "PostDominanceFrontier.h"
#include "../Callgraph/Callgraph.h"
#include "../Modifies/Modifies.h"
#include "../PointsTo/PointsTo.h"
#include "../Languages/LLVMSupport.h"

#include "FunctionStaticSlicer.h"
#include "ExternalHandler.h"

//#define DEBUG_SLICE
//#define DEBUG_SLICING
//#define DEBUG_RC

#define DEBUG_TYPE "function-slicer"

using namespace llvm;
using namespace llvm::slicing;

InsInfo::StructSliceInfoSet_t::iterator InsInfo::defaultStructIterator;

std::mutex FunctionStaticSlicer::passLock;

bool isStackPointer(const Value *v) {
  const Instruction *i = dyn_cast<const Instruction>(v);
  if (!i || i->getOpcode() != Instruction::Load)
    return false;

  const Instruction *regPtr = dyn_cast<const Instruction>(i->getOperand(0));
  if (!regPtr || regPtr->getOpcode() != Instruction::GetElementPtr)
    return false;

  const ConstantInt *idx = dyn_cast<const ConstantInt>(regPtr->getOperand(2));
  if (!idx || (idx->getZExtValue() != 0 && idx->getZExtValue() != 3))
    return false;
  return true;
}

static uint64_t getSizeOfMem(const Value *val) {

  if (const ConstantInt *CI = dyn_cast<ConstantInt>(val)) {
    return CI->getLimitedValue();
  } else if (const Constant *C = dyn_cast<Constant>(val)) {
    if (C->isNullValue())
      return 0;
    assert(0 && "unknown constant");
  }

  /* This sucks indeed, it is only a wild guess... */
  return 64;
}

void InsInfo::addDEFArray(const ptr::PointsToSets &PS, const Value *V,
                          uint64_t lenConst) {
  if (isPointerValue(V)) {
    typedef ptr::PointsToSets::PointsToSet PTSet;

    const PTSet &L = getPointsToSet(V, PS);
    for (PTSet::const_iterator p = L.begin(); p != L.end(); ++p)
      for (uint64_t i = 0; i < lenConst; i++)
        addDEF(Pointee(p->first, p->second + i));
  }
}

void InsInfo::handleVariousFuns(const ptr::PointsToSets &PS, const CallInst *C) {
  StringRef fName;
  if (C->getCalledFunction() && C->getCalledFunction()->hasName())
    fName = C->getCalledFunction()->getName();


  if (fName.equals("klee_make_symbolic")) {
    const Value *l = elimConstExpr(C->getArgOperand(0));
    const Value *len = elimConstExpr(C->getArgOperand(1));
    uint64_t lenConst = getSizeOfMem(len);

    addREF(Pointee(l, -1));
    addDEFArray(PS, l, lenConst);
    if (!isConstantValue(len))
      addREF(Pointee(len, -1));
  } else {
    //FIXME: this should be handled automatically...

    if (fName.equals("objc_msgSend")) {
      DetectParametersPass::UserSet_t Pre_X0 = DetectParametersPass::getRegisterValuesBeforeCall(5, C, true);
      DetectParametersPass::UserSet_t Pre_X1 = DetectParametersPass::getRegisterValuesBeforeCall(6, C, true);

      for (auto Pre : Pre_X0) {
        addREF(Pointee(Pre, -1));
      }
      for (auto Pre : Pre_X1) {
        addREF(Pointee(Pre, -1));
      }
    }

    if (fName.equals("SecItemCopyMatching")) {
      DetectParametersPass::UserSet_t PreX1 = DetectParametersPass::getRegisterValuesBeforeCall(6, C);
      for (auto &X1 : PreX1) {
        std::vector<const Value *> ptsToSet1;
        ptr::getAndersen()->getPointsToSet(X1, ptsToSet1);
        for (auto &pts1 : ptsToSet1) {
          addDEF(Pointee(pts1, -1));
        }
      }
    }

  }
//  } else if (fName.equals("objc_retain") || fName.equals("objc_autoreleaseReturnValue") || fName.equals("objc_autorelease") ||
//          fName.equals("objc_release") ||
//          fName.equals("objc_retainAutoreleasedReturnValue") ||
//          fName.equals("objc_retainAutorelease")) {
//      DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(5, C, true);
//      DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(5, C);
//
//      for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {
//          addREF(Pointee(*Pre_it, -1));
//      }
//      for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
//          addDEF(Pointee(*Post_it, -1));
//      }
//  } else if (fName.equals("objc_msgSendSuper2")) {
//      //Do nothing?
//  } else if (fName.equals("objc_msgSend")) {
//      DetectParametersPass::UserSet_t Pre_X0 = DetectParametersPass::getRegisterValuesBeforeCall(5, (CallInst*)C, true);
//      DetectParametersPass::UserSet_t Pre_X1 = DetectParametersPass::getRegisterValuesBeforeCall(6, (CallInst*)C, true);
//      for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre_X0.begin(); Pre_it != Pre_X0.end(); ++Pre_it) {
//          addREF(Pointee(*Pre_it,-1));
//      }
//      for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre_X1.begin(); Pre_it != Pre_X1.end(); ++Pre_it) {
//          addREF(Pointee(*Pre_it,-1));
//      }
//
  const Module *M = C->getParent()->getParent()->getParent();
  SimpleCallGraph &CG = ptr::getSimpleCallGraph();
  for (SimpleCallGraph::FunctionSet_t::iterator F_it = CG.getCalled(C).begin(); F_it != CG.getCalled(C).end(); ++F_it) {
    const Function *F = M->getFunction(*F_it);
    if (!F) {
      handleCall(this, *F_it, PS);
    } else {
      handleCall(this, F->getName(), PS);
    }
  }
//  } else {
//      errs() << fName << "\n";
////      assert(false);
//  }
}

void InsInfo::addREFArray(const ptr::PointsToSets &PS, const Value *V,
                          uint64_t lenConst) {
  if (isPointerValue(V)) {
    typedef ptr::PointsToSets::PointsToSet PTSet;

    const PTSet &R = getPointsToSet(V, PS);
    for (PTSet::const_iterator p = R.begin(); p != R.end(); ++p)
      for (uint64_t i = 0; i < lenConst; i++)
        addREF(Pointee(p->first, p->second + i));
  }
}

#define DETAILED_DUMP

void InsInfo::dump(bool def, bool ref, bool rc, bool pred) {
  ins->dump();
  if (def) {
    errs() << "Size Def " << DEF.size() << "\n";
    for (ValSet::const_iterator I = DEF.begin(); I != DEF.end(); ++I)
      (*I).first->dump();
  }
  if (ref) {
    errs() << "Size Ref " << REF.size() << "\n";
    for (ValSet::const_iterator I = REF.begin(); I != REF.end(); ++I) {
      errs() << "F: " << (int64_t) getREFInc(*I) << "; ";
      (*I).first->dump();
    }
  }
  if (rc) {
    errs() << "Size RC " << RC.size() << "\n";
    for (ValSet::const_iterator I = RC.begin(); I != RC.end(); ++I) {
      IncType_t inc = getRCInc(*I);

      errs() << "F: " << (inc < INC_MAX ? inc : -1U) << "; ";
      (*I).first->print(errs());
      errs() << ";";
#ifdef DETAILED_DUMP
      for (auto &s : RCSources[(*I).first]) {
        s->print(errs());
        errs() << "; ";
      }
      if (const Instruction *inst = dyn_cast<const Instruction>(I->first)) {
        errs() << "(" << inst->getParent()->getParent()->getName() << ")";
      }
#endif
      errs() << "\n";
    }
  }
#ifdef DETAILED_DUMP
  errs() << "RCStructSliceInfos\n";

  if (false) {
    for (auto it : RCStructInfos) {
      it.first->print(errs());
      errs() << ": ";
      for (auto j : it.second) {
        j->accessInstruction->dump();
        errs() << j->baseOffset << ": ";
        for (auto k : j->basePointers) {
          k.first->print(errs());
          errs() << "   ;   ";
        }
      }
      errs() << "\n";
    }
    errs() << "\n";
  }
  if (pred) {
    errs() << "SlicedPredecessors " << SlicedPredecessors.size() << "\n";
    for (auto &p : SlicedPredecessors) {
      p.first->print(errs());
      errs() << ": ";
      for (auto &i : p.second) {
        i->print(errs());
        errs() << *i << "(" << i->getParent()->getParent()->getName() << "); ";
      }
      errs() << "\n";
    }
    errs() << "\n";
  }
  if (false) {
    errs() << "Translations:\n";

    for (auto &t : translations) {
      t.first->print(errs());
      errs() << ": ";
      for (auto &t2 : t.second) {
        t2->print(errs());
        errs() << ";";
      }
      errs() << "\n";
    }
  }
#endif
}

InsInfo::InsInfo(const Instruction *i, const ptr::PointsToSets &PS,
                 const mods::Modifies &MOD) : ins(i), sliced(true) {
  typedef ptr::PointsToSets::PointsToSet PTSet;

  unsigned opcode = i->getOpcode();

  if (i->getName() == "X0_14582") {
    assert(true);
  }

  switch (opcode) {
    case Instruction::Load: {
      const LoadInst *LI = (const LoadInst *) i;

      BinaryOperator *BO = NULL;
      IntToPtrInst *ItoP = NULL;
      if (PatternMatch::match(LI->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_BinOp(BO)))) {
        if (BO->getOpcode() == Instruction::Add || BO->getOpcode() == Instruction::Sub) {
          ItoP = dyn_cast<IntToPtrInst>(LI->getOperand(0));
          assert(ItoP);
          Instruction *LoadInst = NULL;
          Instruction *GetElem = NULL;
          ConstantInt *Idx = NULL;
          ConstantInt *Offset = NULL;

          //All memory accesses that are not located on the stack (base pointer is not the stack pointer) are assumed
          //to be struct elements
          if (PatternMatch::match(BO->getOperand(0), PatternMatch::m_Instruction(LoadInst)) && LoadInst &&
              LoadInst->getOpcode() ==
              Instruction::Load &&
              PatternMatch::match(
                LoadInst->getOperand(
                  0),
                PatternMatch::m_Instruction(
                  GetElem)) && GetElem &&
              GetElem->getOpcode() ==
              Instruction::GetElementPtr &&
              PatternMatch::match(GetElem->getOperand(2), PatternMatch::m_ConstantInt(Idx)) && Idx &&
              PatternMatch::match(BO, PatternMatch::m_BinOp(PatternMatch::m_Value(),
                                                            PatternMatch::m_ConstantInt(Offset)))) {

            auto findStackValue = [&](const Value *base, int64_t offset, std::set<Pointee> &REFs) {
              bool found = false;

              if (const Instruction *baseInst = dyn_cast<const Instruction>(base)) {
                Instruction *SPInst = NULL;
                Instruction *getElemPtr = NULL;
                ConstantInt *Idx = NULL;
                if (baseInst->getOpcode() == Instruction::Add &&
                    PatternMatch::match(baseInst->getOperand(0), PatternMatch::m_Instruction(SPInst)) &&
                    SPInst->getOpcode() == Instruction::Load &&
                    PatternMatch::match(SPInst->getOperand(0), PatternMatch::m_Instruction(getElemPtr)) &&
                    getElemPtr->getOpcode() == Instruction::GetElementPtr &&
                    PatternMatch::match(getElemPtr->getOperand(2), PatternMatch::m_ConstantInt(Idx)) &&
                    (Idx->getZExtValue() == 3 || Idx->getZExtValue() == 0)) {
                  Function *f = (Function *) baseInst->getParent()->getParent();
                  StackAccessPass *SAP = ptr::getAndersen()->getAnalysisIfAvailable<StackAccessPass>();
                  if (!SAP)
                    SAP = &ptr::getAndersen()->getAnalysis<StackAccessPass>();

                  for (auto &baseStackOffset_it : *SAP->getOffsets(f)[baseInst]) {
                    int64_t targetOffset = baseStackOffset_it + offset;
                    if (!SAP->getOffsetValues(f)[targetOffset])
                      continue;
                    for (auto &target : *SAP->getOffsetValues(f)[targetOffset]) {
                      ptr::PointsToSets::PointsToSet PtsTo = ptr::getPointsToSet(target, PS);
                      for (auto &PtsTo_it : PtsTo) {
                        REFs.insert(PtsTo_it);
                        found = true;
                      }
                    }
                  }
                }
              }

              return found;
            };

            if (Idx->getZExtValue() != 0 && Idx->getZExtValue() != 3) {
              ptr::PointsToSets::PointsToSet P = ptr::getPointsToSet(BO->getOperand(0), PS);
              ptr::PointsToSets::PointsToSet P_loc = ptr::getPointsToSet(ItoP, PS);

//                      //All memory accesses have their own pointer variable
//                      assert(i->getNumUses() == 1);
//                      assert(isa<const Instruction>(*i->user_begin()));
              for (auto user_it : i->users()) {
                StructSliceInfo *structSliceInfo = new StructSliceInfo(Offset->getZExtValue(), user_it);
                std::set<Pointee> REFs;
                for (auto &P_it : P) {
                  if (!findStackValue(P_it.first, Offset->getZExtValue(), REFs))
                    structSliceInfo->basePointers.insert(P_it);
                }
//                                if (P_loc.size() > 5) {
//                                    i->dump();
//                                    errs() << P_loc.size() << "\n";
//                                }
                for (auto &P_it : P_loc) {
                  structSliceInfo->locations.insert(P_it);
//                                    llvm_unreachable("");
                }

                for (auto &DEF_it : REFs) {
                  addREF(DEF_it);
                }

                DEFStructInfos.insert(structSliceInfo);
              }

            }
          }
        }
      }

      addDEF(Pointee(i, -1));

//      const Value *op = elimConstExpr(LI->getPointerOperand());
      const Value *op = LI->getOperand(0);
      const GetElementPtrInst *getElementPtrInst = dyn_cast<GetElementPtrInst>(op);
      if (isa<ConstantPointerNull>(op)) {
        errs() << "ERROR in analysed code -- reading from address 0 at " <<
        i->getParent()->getParent()->getName() << ":\n";
        i->print(errs());
      } else if (isa<ConstantInt>(op)) {
      } else {
        if (getElementPtrInst) {
          ConstantInt *Idx = dyn_cast<ConstantInt>(getElementPtrInst->getOperand(2));
          if (Idx && Idx->getZExtValue() == 5) {
            for (Instruction *I = (Instruction *) i; I != &i->getParent()->front(); I = I->getPrevNode()) {
              if (I->getOpcode() == Instruction::Call) {
//                          addREF(Pointee(I, -1));
                break;
              }
            }
          }
          //TODO: should this be 1???
//              addREF(Pointee(i, -1), 1);
//              addREF(Pointee(i, -1));
          addREF(Pointee(getElementPtrInst, -1));
        }
        //The address is not relevant
//          addREF(Pointee(op, -1), DBL_MAX);
        if (hasExtraReference(op)) {
          addREF(Pointee(op, 0));
        } else {
          ConstantInt *address = nullptr;
          bool refPtsTo = true;
          if (PatternMatch::match(LI->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(address)))) {
            if (ptr::getAndersen()->getMachO().isClassRef(address->getZExtValue()) ||
                ptr::getAndersen()->getMachO().isSelectorRef(address->getZExtValue())) {
              refPtsTo = false;
            }

            if (ptr::getAndersen()->getMachO().isConstValue(address->getZExtValue())) {
              refPtsTo = false;
            }

            std::string sectionName = ptr::getAndersen()->getMachO().getSectionName(address->getZExtValue());
            if (sectionName == "__common") {
              refPtsTo = false;

              const PTSet &S2 = getPointsToSet(i->getOperand(0), PS);
              for (PTSet::const_iterator I = S2.begin(), E = S2.end(); I != E; ++I)
                addREF(*I, 1);
            }
          }

          Value *Base = nullptr;
          Instruction *IVAR = nullptr;
          bool isIVAR = false;
          if (PatternMatch::match(LI->getOperand(0), PatternMatch::m_IntToPtr(
            PatternMatch::m_BinOp(PatternMatch::m_Value(Base),
                                  PatternMatch::m_SExt(PatternMatch::m_Instruction(IVAR)))))) {
            if (!isa<Constant>(Base)) {
              const PTSet &S2 = getPointsToSet(IVAR, PS);
              for (PTSet::const_iterator I = S2.begin(), E = S2.end(); I != E; ++I)
                addREF(*I, 1);

              refPtsTo = false;
              isIVAR = true;
            }
          }

          const PTSet &S = getPointsToSet(i, PS);
          for (PTSet::const_iterator I = S.begin(), E = S.end(); I != E; ++I)
            if (refPtsTo) {
              addREF(*I, 1);
//                      addDEF(*I);
            } else {
              addDEF(*I);
            }

          const PTSet &S1 = getPointsToSet(i->getOperand(0), PS);
          for (PTSet::const_iterator I = S1.begin(), E = S1.end(); I != E; ++I) {
            //The location the address points to is relevant
            if (refPtsTo) {
              addREF(*I, 1);
//                      addDEF(*I);
            } else {
//                            addDEF(*I);
            }
          }

        }
      }
      break;
    }
    case Instruction::Store: {
      const StoreInst *SI = (const StoreInst *) i;
      bool isConstString = false;

      if (SI->getOperand(0)->getName() == "X0_14582") {
        assert(true);
      }

      if (const ConstantInt *c = dyn_cast<ConstantInt>(SI->getOperand(0))) {
        if (c->getBitWidth() <= 64) {
          if (c->getZExtValue() == 4295000208) {
            assert(true);
          }
          std::string secName = ptr::getAndersen()->getMachO().getSectionName(c->getZExtValue());
          if (secName == "__cstring" || secName == "__cfstring") {
            isConstString = true;
          }
          const PTSet &S = getPointsToSet(c, PS);
          for (auto &s : S) {
            addDEF(s);
          }
        }
      }
      addDEF(Pointee(i, -1));
      const Value *l = SI->getOperand(1);// elimConstExpr(SI->getPointerOperand());
//      const Value *l = elimConstExpr(SI->getPointerOperand());

      bool isIVAR = false;

      if (isa<ConstantPointerNull>(l)) {
        errs() << "ERROR in analysed code -- writing to address 0 at " <<
        i->getParent()->getParent()->getName() << ":\n";
        i->print(errs());
      } else if (isa<ConstantInt>(l)) {
        addDEF(Pointee(l, -1));
      } else {
        if (hasExtraReference(l)) {
          addDEF(Pointee(l, 0));
        } else {
          addREF(Pointee(SI->getOperand(1), -1));
          const PTSet &S = getPointsToSet(l, PS);

          for (auto &S_it : S) {
            addDEF(S_it);
          }

//        for (PTSet::const_iterator I = S.begin(), E = S.end(); I != E; ++I)
//          addDEF(*I);


          Value *Base = nullptr;
          Instruction *IVAR = nullptr;

          if (PatternMatch::match(SI->getOperand(1), PatternMatch::m_IntToPtr(
            PatternMatch::m_BinOp(PatternMatch::m_Value(Base),
                                  PatternMatch::m_SExt(PatternMatch::m_Instruction(IVAR)))))) {
            if (!isa<Constant>(Base)) {
              isIVAR = true;
              const PTSet &S2 = getPointsToSet(IVAR, PS);
              for (PTSet::const_iterator I = S2.begin(), E = S2.end(); I != E; ++I)
                addDEF(*I);
            }
          }
        }


        const Value *r = elimConstExpr(SI->getValueOperand());
        const PTSet &S = getPointsToSet(r, PS);
        bool skip = false;

        if (S.size() == 1) {
          const Value *x = S.begin()->first;
          const Value *y = ptr::getAndersen()->getNodeFactory().getLocation(x);
          if (y && isa<ConstantInt>(y)) {
            if (((ConstantInt *) y)->getZExtValue() == 0) {
              skip = true;
            }
          }
        }

        //This allows to trace the value stored here and not a value that was loaded before from the IVAR location...
        if (!isIVAR && !isConstString && !skip) {
          for (auto S_it : S) {
            addREF(S_it, 1);
          }
        }

        if (!l->getType()->isIntegerTy())
          addREF(Pointee(l, -1));

        if (!hasExtraReference(r) && !isConstantValue(r)) {
          addREF(Pointee(r, -1), S.size() && !isIVAR ? INC_MAX : 1);
        }

        std::set<const Value *> visited;
        std::function<void(const PHINode *)> addConstPHIRef = [&](const PHINode *phiNode) {
          if (visited.find(phiNode) == visited.end()) {
            visited.insert(phiNode);
            for (unsigned k = 0; k < phiNode->getNumIncomingValues(); ++k) {
              if (dyn_cast<const ConstantInt>(
                phiNode->getIncomingValue(k))) {
                addREF(Pointee(phiNode, -1), 1);
              } else if (const PHINode *phiNode1 = dyn_cast<const PHINode>(
                phiNode->getIncomingValue(k))) {
                addConstPHIRef(phiNode1);
              }
            }
          }
        };

        if (const PHINode *phiNode = dyn_cast<const PHINode>(r)) {
          addConstPHIRef(phiNode);
        }

      }

      if (const ConstantInt *addr = dyn_cast<const ConstantInt>(ins->getOperand(0))) {
        if (addr->getBitWidth() <= 64) {
          if (ptr::getAndersen()->getMachO().isCFString(addr->getZExtValue())) {
            for (auto &ptsTo : ptr::getPointsToSet(addr, PS)) {
              addDEF(ptsTo);
            }
          }
        }
      }

      break;
    }
    case Instruction::GetElementPtr: {
      addDEF(Pointee(i, -1));
      break;
    }
    case Instruction::Call: {
      const CallInst *C = (const CallInst *) i;

      if (getSuccInBlock(i)->getName() == "LR_8188") {
        assert(true);
      }

      const Value *cv = C->getCalledValue();


      if (const IntToPtrInst *ItoP = dyn_cast<const IntToPtrInst>(C->getOperand(0))) {
        addREF(Pointee(ItoP, -1));
      } else if (C->getCalledValue() && !C->getCalledFunction()) {
        addREF(Pointee(C->getCalledValue(), -1));
      }


      if (isInlineAssembly(C)) {
        errs() << "ERROR: Inline assembler detected in " <<
        i->getParent()->getParent()->getName() << ", ignoring\n";
      } else if (isMemoryAllocation(cv)) {
        if (!isConstantValue(C->getArgOperand(0)))
          addREF(Pointee(C->getArgOperand(0), -1));
        addDEF(Pointee(i, -1));
      } else if (isMemoryDeallocation(cv)) {
      } else if (isMemoryCopy(cv) || isMemoryMove(cv) || isMemorySet(cv)) {
        const Value *len = elimConstExpr(C->getArgOperand(2));
        uint64_t lenConst = getSizeOfMem(len);

        const Value *l = elimConstExpr(C->getOperand(0));
        addDEFArray(PS, l, lenConst);
        addREF(Pointee(l, -1));

        const Value *r = elimConstExpr(C->getOperand(1));
        /* memset has a constant/variable there */
        if (isMemoryCopy(cv) || isMemoryMove(cv))
          addREFArray(PS, r, lenConst);
        addREF(Pointee(r, -1));

        /* memcpy/memset wouldn't work with len being 'undef' */
        if (!isConstantValue(len))
          addREF(Pointee(len, -1));
      } else {
        typedef std::vector<const llvm::Function *> CalledVec;

        /* did we miss something? */
        assert(!memoryManStuff(cv));

//      if (const Function *F = dyn_cast<Function>(cv))
        handleVariousFuns(PS, C);
//      else
//	addREF(Pointee(cv, -1));

        CalledVec CV;
        getCalledFunctions(C, PS, std::back_inserter(CV));

        for (CalledVec::const_iterator f = CV.begin(); f != CV.end(); ++f) {
          mods::Modifies::mapped_type const &M = getModSet(*f, MOD);
          for (mods::Modifies::mapped_type::const_iterator v = M.begin();
               v != M.end(); ++v) {
            addDEF(*v);
          }
        }

        if (!callToVoidFunction(C))
          addDEF(Pointee(C, -1));
      }

      if (C->getCalledFunction() && C->getCalledFunction()->getName() == "memcpy") {
        DetectParametersPass::UserSet_t X2_pre = DetectParametersPass::getRegisterValuesBeforeCall(7, C, false);
        DetectParametersPass::UserSet_t X0_pre = DetectParametersPass::getRegisterValuesBeforeCall(5, C, false);

        Andersen *andersen = ptr::getAndersen();

        Andersen::StackOffsetMap_t stackOffsetMap = andersen->getStackOffsets();

        for (auto &X2_it : X2_pre) {
          if (const ConstantInt *size = dyn_cast<const ConstantInt>(X2_it)) {

            for (auto &X0_it : X0_pre) {

              ptr::PointsToSets::PointsToSet X0_ptsTo = ptr::getPointsToSet(X0_it, PS);

              for (auto &X0pts_it : X0_ptsTo) {
                auto &pairs = stackOffsetMap[X0pts_it.first];

                for (auto &p_it : pairs) {

                  int64_t lo = p_it.second;
                  int64_t hi = p_it.second + size->getZExtValue();

                  Function *f = (Function *) p_it.first;
                  StackAccessPass::OffsetValueListMap_t &OffsetValues = andersen->getAnalysis<StackAccessPass>().getOffsetValues(
                    f);
                  for (auto &O_it : OffsetValues) {
                    if (O_it.first <= lo || O_it.first >= hi)
                      continue;

                    if (!O_it.second)
                      continue;
                    for (auto &V_it: *O_it.second) {
                      ptr::PointsToSets::PointsToSet defPtsTo = ptr::getPointsToSet(V_it, PS);
                      for (auto &def_it : defPtsTo) {
                        addDEF(def_it);

                        int64_t offset = O_it.first - lo;

                        defOffsets[def_it.first].insert(offset);
                      }
                    }
                  }

                }
              }
            }
          }
        }
      }

      break;
    }


    case Instruction::Add:
    case Instruction::FAdd:
    case Instruction::Sub:
    case Instruction::FSub:
    case Instruction::Mul:
    case Instruction::FMul:
    case Instruction::UDiv:
    case Instruction::SDiv:
    case Instruction::FDiv:
    case Instruction::URem:
    case Instruction::SRem:
    case Instruction::FRem:
    case Instruction::Shl:
    case Instruction::LShr:
    case Instruction::AShr:
    case Instruction::And:
    case Instruction::Or:
    case Instruction::Xor: {
      const BinaryOperator *BO = (const BinaryOperator *) i;

      const PTSet &S = getPointsToSet(BO, PS);
      bool add = true;
      for (PTSet::iterator i = S.begin(); i != S.end(); ++i) {
//          if (i->first == BO) {
        add = false;
//                addREF(*i, 1);
//          }
      }
//      if (add)
      addDEF(Pointee(i, -1));

      if (!isConstantValue(BO->getOperand(0)))
        addREF(Pointee(BO->getOperand(0), -1), 1);
//          addREF(Pointee(BO->getOperand(0), -1));
      if (!isConstantValue(BO->getOperand(1)))
        addREF(Pointee(BO->getOperand(1), -1), 1);
//          addREF(Pointee(BO->getOperand(1), -1));

      break;
    }
    case Instruction::IntToPtr: {
      addDEF(Pointee(i, -1));
      addREF(Pointee(i->getOperand(0), -1));
      break;
    }
    case Instruction::Trunc:
    case Instruction::ZExt:
    case Instruction::SExt:
    case Instruction::FPToUI:
    case Instruction::FPToSI:
    case Instruction::UIToFP:
    case Instruction::SIToFP:
    case Instruction::FPTrunc:
    case Instruction::FPExt:
    case Instruction::PtrToInt:
    case Instruction::BitCast: {
      const CastInst *CI = (const CastInst *) i;
      addDEF(Pointee(i, -1));

      if (!hasExtraReference(CI->getOperand(0)))
        addREF(Pointee(CI->getOperand(0), -1), 1); //TODO: always increment by 1?
      break;
    }
    case Instruction::ICmp:
    case Instruction::FCmp: {
      const CmpInst *CI = (const CmpInst *) i;

      addDEF(Pointee(i, -1));

      if (!isConstantValue(CI->getOperand(0)))
        addREF(Pointee(CI->getOperand(0), -1), 1);
      if (!isConstantValue(CI->getOperand(1)))
        addREF(Pointee(CI->getOperand(1), -1), 1);
      break;
    }
    case Instruction::Br:
    case Instruction::IndirectBr: {
      const BranchInst *BI = (const BranchInst *) i;

      if (BI->isConditional() && !isConstantValue(BI->getCondition()))
        addREF(Pointee(BI->getCondition(), -1));
      break;
    }
    case Instruction::PHI: {
      std::set<const Value *> visitedPhi;
      std::function<void(const PHINode *)> addConst = [&](const PHINode *phiNode) {
        if (visitedPhi.find(phiNode) == visitedPhi.end()) {
          visitedPhi.insert(phiNode);
          for (unsigned k = 0; k < phiNode->getNumIncomingValues(); ++k) {
            if (const ConstantInt *constantInt = dyn_cast<const ConstantInt>(
              phiNode->getIncomingValue(k))) {
              addDEF(Pointee(constantInt, -1));
            } else if (const PHINode *phiNode1 = dyn_cast<const PHINode>(phiNode->getIncomingValue(k))) {
              addConst(phiNode1);
            }
          }
        }
      };

      const PHINode *phi = (const PHINode *) i;

      addDEF(Pointee(i, -1));

      for (unsigned k = 0; k < phi->getNumIncomingValues(); ++k) {
        const Value *p = phi->getIncomingValue(k);

        if (!isa<const ConstantInt>(p))
          continue;

//        if (!isConstantValue(phi->getIncomingValue(k)))
//                addREF(Pointee(phi->getIncomingValue(k), -1), 1.0);
//                addDEF(Pointee(phi->getIncomingValue(k), -1));
        ptr::PointsToSets::PointsToSet PtsTo = ptr::getPointsToSet(phi->getIncomingValue(k), PS);
        for (auto &p : PtsTo) {
          addDEF(p);
//                    addREF(p, 1.0);
        }
      }
      break;
    }
    case Instruction::Select: {
      const SelectInst *SI = (const SelectInst *) i;
      // TODO: THE FOLLOWING CODE HAS NOT BEEN TESTED YET

      addDEF(Pointee(i, -1));

      if (!isConstantValue(SI->getCondition()))
        addREF(Pointee(SI->getCondition(), -1));
      if (!isConstantValue(SI->getTrueValue()))
        addREF(Pointee(SI->getTrueValue(), -1));
      if (!isConstantValue(SI->getFalseValue()))
        addREF(Pointee(SI->getFalseValue(), -1));
      break;
    }
    case Instruction::ExtractValue: {
      const ExtractValueInst *EV = (const ExtractValueInst *) i;
      addDEF(Pointee(i, -1));
      addREF(Pointee(EV->getAggregateOperand(), -1));
      break;
    }
    case Instruction::Unreachable:
    case Instruction::Ret: {
      break;
    }
    default:
      DEBUG(errs() << "ERROR: Unsupported instruction reached\n";
              i->dump(););
      break;
  }

//if (const AllocaInst *AI = dyn_cast<const AllocaInst>(i)) {
//      addDEF(Pointee(AI, -1));
//    if (!isConstantValue(AI->getArraySize()))
//      addREF(Pointee(AI->getArraySize(), -1));
//  } else if (const CmpInst *CI = dyn_cast<const CmpInst>(i)) {
//  } else if (const BranchInst *BI = dyn_cast<const BranchInst>(i)) {
//  } else if (const PHINode *phi = dyn_cast<const PHINode>(i)) {
//  } else if (const SwitchInst *SI = dyn_cast<SwitchInst>(i)) {
//    if (!isConstantValue(SI->getCondition()))
//      addREF(Pointee(SI->getCondition(), -1));
//  } else if (const SelectInst *SI = dyn_cast<const SelectInst>(i)) {
//  } else if (isa<const UnreachableInst>(i)) {
//  } else if (const ExtractValueInst *EV = dyn_cast<const ExtractValueInst>(i)) {
//  } else if (const InsertValueInst *IV = dyn_cast<const InsertValueInst>(i)) {
////      TODO THE FOLLOWING CODE HAS NOT BEEN TESTED YET
//
//      const Value *r = IV->getInsertedValueOperand();
//      addDEF(Pointee(IV->getAggregateOperand(), -1));
//      if (!isConstantValue(r))
//	addREF(Pointee(r, -1));
//  } else {
//
//  }

}

void InsInfo::deslice(FunctionStaticSlicer *FSS) {
  sliced = false;
}

namespace {
  class FunctionSlicer : public ModulePass {
  public:
    static char ID;

    FunctionSlicer() : ModulePass(ID) { }

    virtual bool runOnModule(Module &M);

    void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<PostDominatorTree>();
      AU.addRequired<PostDominanceFrontier>();

      AU.addPreserved<PostDominatorTree>();
      AU.addPreserved<PostDominanceFrontier>();
    }

  private:
    bool runOnFunction(Function &F, const ptr::PointsToSets &PS,
                       const mods::Modifies &MOD);
  };
}

static RegisterPass<FunctionSlicer> X("slice", "Slices the code");
char FunctionSlicer::ID;

FunctionStaticSlicer::~FunctionStaticSlicer() {
  for (InsInfoMap::const_iterator I = insInfoMap.begin(), E = insInfoMap.end();
       I != E; I++)
    delete I->second;
}

typedef llvm::SmallVector<const Instruction *, 10> SuccList;

static SuccList getSuccList(const Instruction *i) {
  SuccList succList;
  const BasicBlock *bb = i->getParent();
  if (i != &bb->back()) {
    BasicBlock::const_iterator I(i);
    I++;
    succList.push_back(&*I);
  } else {
    for (succ_const_iterator I = succ_begin(bb), E = succ_end(bb); I != E; I++)
      succList.push_back(&(*I)->front());
  }
  return succList;
}

void FunctionStaticSlicer::initializeInfos() {

  std::unique_lock<std::mutex> lock(initLock);

  if (infosInitialized) {
    return;
  }
  infosInitialized = true;

  for (llvm::inst_iterator I = llvm::inst_begin(fun), E = llvm::inst_end(fun);
       I != E; ++I)
    insInfoMap.insert(InsInfoMap::value_type(&*I, new InsInfo(&*I, PS, mods)));
//
//      for (llvm::inst_iterator I = llvm::inst_begin(F); I != llvm::inst_end(F); ++I) {
//          if (I->getOpcode() == Instruction::Call && F.getName() != "_EXTERNAL_") {
//              SimpleCallGraph::FunctionSet_t Called = ptr::getSimpleCallGraph().getCalled(&*I);
//              for (SimpleCallGraph::FunctionSet_t::iterator Called_it = Called.begin(); Called_it != Called.end(); ++Called_it) {
//                  Function *Called_F = F.getParent()->getFunction(*Called_it);
//                  if (!Called_F)
//                      continue;
//                  DetectParametersPass::ParameterAccessPairSet_t Returns = ptr::getDetectParametersPass().getReturnRegisterIndexes(Called_F);
//
//                  for (DetectParametersPass::ParameterAccessPairSet_t::iterator Return_it = Returns.begin(); Return_it != Returns.end(); ++Return_it) {
//                      DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(Return_it->first, &*I);
//
//                      for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
//                          if (Instruction *Inst = dyn_cast<Instruction>(*Post_it)) {
//                              getInsInfo(Inst)->addREF(ptr::PointsToSets::Pointee(Return_it->second, -1));
//                          }
//                      }
//                  }
//              }
//          }
//      }

  SimpleCallGraph::InstructionSet_t Callers = ptr::getSimpleCallGraph().getCallers(fun.getName());

  DetectParametersPass::ParameterAccessPairSet_t Params = ptr::getDetectParametersPass().getParameterRegisterIndexes(
    &fun);
  for (DetectParametersPass::ParameterAccessPairSet_t::iterator Params_it = Params.begin();
       Params_it != Params.end(); ++Params_it) {

//        for (SimpleCallGraph::InstructionSet_t::iterator Caller_it = Callers.begin(); Caller_it != Callers.end(); ++Caller_it) {
//            if ((*Caller_it)->getParent()->getParent()->getName() == "_EXTERNAL_")
//                continue;
//            const CallInst *call = dyn_cast<const CallInst>(*Caller_it);
//            if (!call) {
//                llvm_unreachable("A caller has to be a call instruction...");
//            }
//            DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(Params_it->first, (Instruction*)(*Caller_it), true);
//            for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {
//                InsInfo *info = getInsInfo(Params_it->second);
////                info->clearREF();
//                info->addREF(ptr::PointsToSets::Pointee(*Pre_it, -1), 1);
//
//                Instruction *param = dyn_cast<Instruction>(*Pre_it);
//                if (!param) {
//                    llvm_unreachable("this has to be a store instruction!");
//                }
//            }
//        }
    InsInfo *info = getInsInfo(Params_it->second);
    info->addREF(ptr::PointsToSets::Pointee(Params_it->second, -1), 1);
  }

  DetectParametersPass &DPP = ptr::getAndersen()->getAnalysis<DetectParametersPass>();

  for (inst_iterator I_it = inst_begin(fun); I_it != inst_end(fun); ++I_it) {
    if (I_it->getOpcode() == Instruction::Call) {
      CallInst *call = (CallInst *) &*I_it;
      if (call->getCalledFunction() &&
          (call->getCalledFunction()->isIntrinsic() || call->getCalledFunction()->isDeclaration())) {

        //Add a 'self-REF' for a parameter/return value that is defined by an external function
        InsInfo *callInfo = getInsInfo(call);
        for (ValSet::const_iterator DEF_it = callInfo->DEF_begin(); DEF_it != callInfo->DEF_end(); ++DEF_it) {
          if (const Instruction *defInst = dyn_cast<const Instruction>(DEF_it->first)) {
            if (defInst->getOpcode() == Instruction::Load &&
                defInst->getParent()->getParent() == call->getParent()->getParent()) {
              getInsInfo(defInst)->addREF(Pointee(defInst, -1), 1);
            }
          }
        }

//                continue;
      } else if (!call->getCalledFunction()) {
        continue;
      }
      DetectParametersPass::UserSet_t PostX0 = DetectParametersPass::getRegisterValuesAfterCall(5, call);

      Module *M = call->getParent()->getParent()->getParent();
      SimpleCallGraph::FunctionSet_t called = ptr::getSimpleCallGraph().getCalled(call);

      for (std::string functionName : called) {
        if (functionName == "-[ViewController getIterations]") {
          assert(true);
        }
        Function *function = M->getFunction(functionName);
        if (!function) {
          continue;
        }


        DetectParametersPass::ParameterAccessPairSet_t &Ret = DPP.getReturnRegisterIndexes(function);

        for (auto &r_it : Ret) {
          Value *ref = nullptr;
          if (r_it.second->getOpcode() == Instruction::Store) {
            ref = r_it.second->getOperand(0);
          } else {
            ref = r_it.second;
          }
          assert(r_it.first == 5);
          for (auto &post_it : PostX0) {
            getInsInfo(dyn_cast<Instruction>(post_it))->addREF(Pointee(ref, -1), 1);
          }
        }
      }
    }
  }
}

bool FunctionStaticSlicer::sameValues(const Pointee &val1, const Pointee &val2) {
  return val1.first == val2.first && val1.second == val2.second;
}

/*
 * RC(i)=RC(i) \cup
 *   {v| v \in RC(j), v \notin DEF(i)} \cup
 *   {v| v \in REF(i), DEF(i) \cap RC(j) \neq \emptyset}
 */
bool FunctionStaticSlicer::computeRCi(InsInfo *insInfoi, InsInfo *insInfoj) {
  bool changed = false;

  if (insInfoj->getIns()->getName() == "LR_6524") {
    assert(true);
  }

  if (insInfoi->getIns()->getOpcode() == Instruction::Call) {
    const CallInst *callInst = dyn_cast<const CallInst>(insInfoi->getIns());
    if (callInst->getCalledFunction() && callInst->getCalledFunction()->getName() == "memcpy") {
      DetectParametersPass::UserSet_t X2_pre = DetectParametersPass::getRegisterValuesBeforeCall(7, callInst, false);
      DetectParametersPass::UserSet_t X1_pre = DetectParametersPass::getRegisterValuesBeforeCall(6, callInst, false);
      DetectParametersPass::UserSet_t X0_pre = DetectParametersPass::getRegisterValuesBeforeCall(5, callInst, false);

      for (auto &X2_pre_it : X2_pre) {
        const ConstantInt *size = dyn_cast<const ConstantInt>(X2_pre_it);
        //We consider only copies of known size because a structs size is known at compile time (other cases signal that we don't have a struct here)
        if (!size)
          continue;

        auto findStackValue = [&](const Value *base, int64_t offset, std::set<Pointee> &REFs) {
          bool found = false;

          if (const Instruction *baseInst = dyn_cast<const Instruction>(base)) {
            Instruction *SPInst = NULL;
            Instruction *getElemPtr = NULL;
            ConstantInt *Idx = NULL;
            if (baseInst->getOpcode() == Instruction::Add &&
                PatternMatch::match(baseInst->getOperand(0), PatternMatch::m_Instruction(SPInst)) &&
                SPInst->getOpcode() == Instruction::Load &&
                PatternMatch::match(SPInst->getOperand(0), PatternMatch::m_Instruction(getElemPtr)) &&
                getElemPtr->getOpcode() == Instruction::GetElementPtr &&
                PatternMatch::match(getElemPtr->getOperand(2), PatternMatch::m_ConstantInt(Idx)) &&
                (Idx->getZExtValue() == 3 || Idx->getZExtValue() == 0)) {
              Function *f = (Function *) baseInst->getParent()->getParent();
              StackAccessPass *SAP = ptr::getAndersen()->getAnalysisIfAvailable<StackAccessPass>();
              if (!SAP)
                SAP = &ptr::getAndersen()->getAnalysis<StackAccessPass>();

              for (auto &baseStackOffset_it : *SAP->getOffsets(f)[baseInst]) {
                int64_t targetOffset = baseStackOffset_it + offset;
                if (!SAP->getOffsetValues(f)[targetOffset])
                  continue;
                for (auto &target : *SAP->getOffsetValues(f)[targetOffset]) {
                  ptr::PointsToSets::PointsToSet PtsTo = ptr::getPointsToSet(target, PS);
                  for (auto &PtsTo_it : PtsTo) {
                    REFs.insert(PtsTo_it);

                    found = true;
                  }
                }
              }
            }
          }

          return found;
        };


//                for (ValSet::const_iterator DEF_it = insInfoi->DEF_begin(); DEF_it != insInfoi->DEF_end(); ++DEF_it) {
        for (ValSet::const_iterator RC_it = insInfoj->RC_begin(); RC_it != insInfoj->RC_end(); ++RC_it) {

          for (InsInfo::StructSliceInfoSet_t::const_iterator ssi_it = insInfoj->RCStruct_begin(RC_it->first);
               ssi_it != insInfoj->RCStruct_end(RC_it->first); ++ssi_it) {
            //Check if the value gets copied here
            if ((*ssi_it)->baseOffset < (int64_t) size->getZExtValue()) {

              for (auto &X0_it : X0_pre) {
                ptr::PointsToSets::PointsToSet X0_ptsTo = ptr::getPointsToSet(X0_it, PS);
                bool usesBase = false;
                for (auto X0_pts_it : X0_ptsTo) {
                  for (auto base_it : (*ssi_it)->basePointers) {
                    if (X0_pts_it == base_it) {
                      usesBase = true;
                      break;
                    }

                  }
                }
                if (!usesBase)
                  continue;

                for (auto X1_it : X1_pre) {

                  std::set<Pointee> REFs;
                  if (findStackValue(X1_it, (*ssi_it)->baseOffset, REFs)) {
                    for (auto &RC_new : REFs) {
                      insInfoi->addRC(RC_new, (*ssi_it)->accessInstruction, insInfoProvider, 1);
                    }
                  } else if (const ConstantInt *constAddr = dyn_cast<const ConstantInt>(X1_it)) {
                    ConstantInt *addr = ConstantInt::get(getGlobalContext(), APInt(64,
                                                                                   constAddr->getZExtValue() +
                                                                                   (*ssi_it)->baseOffset));
                    changed |= insInfoi->addREF(Pointee(addr, -1), insInfoj->getRCInc(*RC_it));
//                                            changed |= insInfoi->addRC(Pointee(addr, -1), insInfoi->getIns(), 1);
//                                            changed |= insInfoi->addDEF(*RC_it);
                    for (auto &loc_it : (*ssi_it)->locations) {
                      changed |= insInfoi->addDEF(loc_it);
                      if (changed) {
                        DEBUG(errs() << "TRANSLATE: ";
                                loc_it.first->print(errs());
                                errs() << " to " << utohexstr(addr->getZExtValue()) << "\n";);
                        insInfoi->addTranslation(loc_it.first, addr);
                      }
                    }
                  } else {
                    if (PatternMatch::match(X1_it,
                                            PatternMatch::m_BinOp(PatternMatch::m_Value(),
                                                                  PatternMatch::m_ConstantInt()))) {
                      //TODO:
//                                                errs() << "TODO: stack stored struct?\n";
                    } else {
                      //The argument represents the new base pointer
                      StructSliceInfo *ssiNew = new StructSliceInfo((*ssi_it)->baseOffset, callInst);

                      ssiNew->basePointers = ptr::getPointsToSet(X1_it, PS);

                      bool alreadyDefined = false;
                      for (auto s_it : getInsInfo(callInst)->getDEFStructSliceInfos()) {
                        if (s_it->basePointers == ssiNew->basePointers &&
                            s_it->baseOffset == ssiNew->baseOffset &&
                            s_it->accessInstruction == ssiNew->accessInstruction) {
                          alreadyDefined = true;

                          for (auto &l : s_it->locations) {
                            changed |= getInsInfo(callInst)->addREF(l, insInfoj->getRCInc(*RC_it));
                          }
                        }
                      }

                      if (alreadyDefined) {
                        delete (ssiNew);
                        continue;
                      }

                      Value *dummy = new llvm::GlobalVariable(*fun.getParent(),
                                                              llvm::IntegerType::get(
                                                                llvm::getGlobalContext(),
                                                                1), false,
                                                              llvm::GlobalVariable::ExternalLinkage,
                                                              nullptr);
                      ssiNew->locations.insert(Pointee(dummy, -1));
//                                            changed |= getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->addREF(Pointee(dummy, -1));
                      changed |= getInsInfo(callInst)->addREF(Pointee(dummy, -1),
                                                              insInfoj->getRCInc(*RC_it));

                      //TODO: maybe add an own function for adding struct infos...
                      getInsInfo(callInst)->getDEFStructSliceInfos().insert(ssiNew);

                      for (auto l : (*ssi_it)->locations) {
                        changed |= getInsInfo(callInst)->addDEF(l);
                        insInfoi->addTranslation(l.first, dummy);
                      }
                    }

                  }
                }
              }
            }
          }
        }
//                }
      }
    }
  }

  if (insInfoi->getIns()->getOpcode() == Instruction::IntToPtr) {
    ptr::PointsToSets::PointsToSet locPts = ptr::getPointsToSet(insInfoi->getIns()->getOperand(0), PS);
    for (ValSet::const_iterator RC_it = insInfoj->RC_begin(); RC_it != insInfoj->RC_end(); ++RC_it) {
      for (InsInfo::StructSliceInfoSet_t::const_iterator ssi_it = insInfoj->RCStruct_begin(RC_it->first);
           ssi_it != insInfoj->RCStruct_end(RC_it->first); ++ssi_it) {

        bool intersect = false;
        for (auto &loc_it : locPts) {
          for (auto &base_it : (*ssi_it)->basePointers) {
            if (loc_it == base_it) {
              intersect = true;
              break;
            }
          }
        }

        //This means that the base pointer was not used...
        if (!intersect) {
          continue;
        }

        assert(insInfoi->getIns()->getNumUses() == 1);
        const StoreInst *baseStoreInst = dyn_cast<const StoreInst>(*insInfoi->getIns()->user_begin());
        if (!baseStoreInst)
          continue;

        uint64_t storeSize =
          baseStoreInst->getOperand(1)->getType()->getPointerElementType()->getPrimitiveSizeInBits() /
          8;

        int64_t baseStoreOffset = 0;

        ConstantInt *baseStoreOffsetValue = NULL;
        Value *basePointer = NULL;
        if (PatternMatch::match(baseStoreInst->getOperand(1), PatternMatch::m_IntToPtr(
          PatternMatch::m_BinOp(PatternMatch::m_Value(basePointer),
                                PatternMatch::m_ConstantInt(baseStoreOffsetValue))))) {
          baseStoreOffset = baseStoreOffsetValue->getZExtValue();
        }

        if ((int64_t) storeSize < (*ssi_it)->baseOffset) {
          for (Instruction *I_it = ((Instruction *) baseStoreInst)->getPrevNode();
               I_it != &I_it->getParent()->front(); I_it = I_it->getPrevNode()) {
            if (const StoreInst *s2 = dyn_cast<const StoreInst>(I_it)) {

              ConstantInt *offset2_1 = NULL;
              ConstantInt *offset2_2 = NULL;

              int64_t offset2 = 0;
              bool match = false;

              if (PatternMatch::match(s2->getOperand(1), PatternMatch::m_IntToPtr(
                PatternMatch::m_BinOp(PatternMatch::m_Specific(basePointer),
                                      PatternMatch::m_ConstantInt(offset2_1))))) {
                offset2 = offset2_1->getZExtValue();
                match = true;
              } else if (PatternMatch::match(s2->getOperand(1), PatternMatch::m_IntToPtr(
                PatternMatch::m_BinOp(
                  PatternMatch::m_BinOp(PatternMatch::m_Specific(basePointer),
                                        PatternMatch::m_ConstantInt(offset2_1)),
                  PatternMatch::m_ConstantInt(offset2_2))))) {
                match = true;
                offset2 = offset2_1->getZExtValue() + offset2_2->getZExtValue();
              }
              if (match) {
                if (baseStoreOffset + (int64_t) storeSize != offset2) {
//                                    assert(false);
                  continue;
                }
                uint64_t s2Size = s2->getOperand(
                  1)->getType()->getPointerElementType()->getPrimitiveSizeInBits() / 8;

                //The current store writes to the structs element
                if ((int64_t) (storeSize + s2Size) >= (*ssi_it)->baseOffset) {
                  if (const LoadInst *baseLoadInst = dyn_cast<const LoadInst>(
                    baseStoreInst->getOperand(0))) {
                    Value *srcBase = NULL;

                    //TODO: does this always mean, that a sub-struct gets copied?
                    if (PatternMatch::match(baseLoadInst->getOperand(0),
                                            PatternMatch::m_IntToPtr(
                                              PatternMatch::m_BinOp()))) {
//                                            assert(PatternMatch::match(baseLoadInst, PatternMatch::m_IntToPtr(srcBase)));
                      srcBase = ((Instruction *) baseLoadInst->getOperand(0))->getOperand(0);

                      //TODO: stack pointers need to be handled separately!?
                      const Value *baseNew = ((Instruction *) srcBase)->getOperand(0);
                      if (isStackPointer(baseNew)) {
                        continue;
                        assert(false);
                      }


                      const ConstantInt *srcBaseOffset = dyn_cast<const ConstantInt>(
                        ((Instruction *) srcBase)->getOperand(1));
                      assert(srcBaseOffset);
                      ptr::PointsToSets::PointsToSet PtsTo = ptr::getPointsToSet(srcBase, PS);

                      for (auto &x : (*ssi_it)->locations) {
                        changed |= getInsInfo(s2)->addDEF(x);
                      }

                      StructSliceInfo *ssiNew = new StructSliceInfo(
                        srcBaseOffset->getZExtValue() + (*ssi_it)->baseOffset,
                        baseLoadInst);

                      ptr::PointsToSets::PointsToSet ptsToNew = ptr::getPointsToSet(baseNew,
                                                                                    PS);
                      ssiNew->basePointers.insert(Pointee(baseNew, -1));
                      for (auto pts_it : ptsToNew) {
                        ssiNew->basePointers.insert(pts_it);
                      }

                      bool alreadyDefined = false;
//                                            for (auto s_it : getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->getDEFStructSliceInfos()) {
                      for (auto s_it : getInsInfo(s2)->getDEFStructSliceInfos()) {
                        if (s_it->basePointers == ssiNew->basePointers &&
                            s_it->baseOffset == ssiNew->baseOffset &&
                            s_it->accessInstruction == ssiNew->accessInstruction) {
                          alreadyDefined = true;
                          break;
                        }
                      }

                      if (alreadyDefined) {
                        delete (ssiNew);
                        continue;
                      }


                      Value *dummy = new llvm::GlobalVariable(*fun.getParent(),
                                                              llvm::IntegerType::get(
                                                                llvm::getGlobalContext(),
                                                                1), false,
                                                              llvm::GlobalVariable::ExternalLinkage,
                                                              nullptr);

                      for (auto &x : (*ssi_it)->locations) {
                        getInsInfo(s2)->addTranslation(x.first, dummy);
                      }

                      ssiNew->locations.insert(Pointee(dummy, -1));
//                                            changed |= getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->addREF(Pointee(dummy, -1));
                      changed |= getInsInfo(s2)->addREF(Pointee(dummy, -1),
                                                        insInfoj->getRCInc(*RC_it));

                      //TODO: maybe add an own function for adding struct infos...
//                                            getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->getDEFStructSliceInfos().insert(ssiNew);
                      getInsInfo(s2)->getDEFStructSliceInfos().insert(ssiNew);


                    } else if (PatternMatch::match(baseLoadInst->getOperand(0),
                                                   PatternMatch::m_IntToPtr(
                                                     PatternMatch::m_Value(srcBase)))) {
//                                            assert(false);
                    }
                  }
                }
              }
            } else if (I_it->getOpcode() == Instruction::Call) {
              break;
            }
          }
        }
      }
    }

  }

  /* {v| v \in RC(j), v \notin DEF(i)} */
  for (ValSet::const_iterator I = insInfoj->RC_begin(),
         E = insInfoj->RC_end(); I != E; I++) {
    const Pointee &RCj = *I;
    bool in_DEF = false;
    for (ValSet::const_iterator II = insInfoi->DEF_begin(),
           EE = insInfoi->DEF_end(); II != EE; II++) {
      if (sameValues(*II, RCj)) {
        in_DEF = true;
        break;
      }
    }
    if (!in_DEF) {

      const InsInfo::ValSet_t &RCSources = insInfoj->getRCSource(RCj);
      if (insInfoi->addRC(RCj, RCSources, insInfoProvider, insInfoj->getRCInc(RCj))) {
        changed = true;
      }

      for (InsInfo::StructSliceInfoSet_t::const_iterator b = insInfoj->RCStruct_begin(RCj.first);
           b != insInfoj->RCStruct_end(RCj.first); ++b) {
        insInfoi->addRCStruct(RCj.first, *b);
      }
    }
  }
  /* DEF(i) \cap RC(j) \neq \emptyset */
  bool isect_nonempty = false;
  IncType_t f_RC_min = INC_MAX;
  InsInfo::StructSliceInfoSet_t structInfos;
  for (ValSet::const_iterator I = insInfoi->DEF_begin(),
//       E = insInfoi->DEF_end(); I != E && !isect_nonempty; I++) {
         E = insInfoi->DEF_end(); I != E; I++) {
    const Pointee &DEFi = *I;
    for (ValSet::const_iterator II = insInfoj->RC_begin(),
           EE = insInfoj->RC_end(); II != EE; II++) {
      if (sameValues(DEFi, *II)) {
        InsInfo::DefOffsets_t &defOffsets = insInfoi->getDEFOffset();
        if (defOffsets.find(DEFi.first) != defOffsets.end()) {

          DetectParametersPass::UserSet_t X1_pre = DetectParametersPass::getRegisterValuesBeforeCall(6,
                                                                                                     insInfoi->getIns(),
                                                                                                     false);

          for (auto &X1_it : X1_pre) {
            if (const ConstantInt *baseAddr = dyn_cast<const ConstantInt>(X1_it)) {
              for (auto &o : defOffsets[DEFi.first]) {
                ConstantInt *addr = ConstantInt::get(getGlobalContext(), APInt(64,
                                                                               baseAddr->getZExtValue() +
                                                                               o));
                changed |= insInfoi->addREF(Pointee(addr, -1), insInfoj->getRCInc(*II));
                insInfoi->addTranslation(DEFi.first, addr);
              }
            } else {
              for (auto &o : defOffsets[DEFi.first]) {
                StructSliceInfo *ssiNew = new StructSliceInfo(o, insInfoi->getIns());

                ptr::PointsToSets::PointsToSet X1PtsTo = ptr::getPointsToSet(X1_it, PS);
                for (auto &X1PtsTo_it : X1PtsTo) {
                  ssiNew->basePointers.insert(X1PtsTo_it);
                }

                bool alreadyDefined = false;
//                                            for (auto s_it : getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->getDEFStructSliceInfos()) {
                for (auto &ssi_it : insInfoi->getDEFStructSliceInfos()) {
                  if (ssi_it->basePointers == ssiNew->basePointers &&
                      ssi_it->baseOffset == ssiNew->baseOffset &&
                      ssi_it->accessInstruction == ssiNew->accessInstruction) {
                    alreadyDefined = true;
                    break;
                  }
                }

                if (alreadyDefined) {
                  delete (ssiNew);
                  continue;
                }


                Value *dummy = new llvm::GlobalVariable(*fun.getParent(),
                                                        llvm::IntegerType::get(llvm::getGlobalContext(), 1), false,
                                                        llvm::GlobalVariable::ExternalLinkage,
                                                        nullptr);

                ssiNew->locations.insert(Pointee(dummy, -1));
//                                            changed |= getInsInfo(dyn_cast<Instruction>(baseLoadInst->getOperand(0)))->addREF(Pointee(dummy, -1));
                changed |= insInfoi->addREF(Pointee(dummy, -1),
                                            insInfoj->getRCInc(*II));

                insInfoi->getDEFStructSliceInfos().insert(ssiNew);
              }

            }
          }

        }

        isect_nonempty = true;
        IncType_t RC_inc = insInfoj->getRCInc(*II);
        f_RC_min = RC_inc < f_RC_min ? RC_inc : f_RC_min;

//          for (InsInfo::StructSliceInfoSet_t::const_iterator b = insInfoj->RCStruct_begin(II->first); b != insInfoj->RCStruct_end(II->first); ++b) {
//              structInfos.insert(*b);
//          }

        structInfos.insert(insInfoi->getDEFStructSliceInfos().begin(), insInfoi->getDEFStructSliceInfos().end());
      }
    }
  }

  /* {v| v \in REF(i), ...} */
  if (isect_nonempty) {
    for (ValSet::const_iterator I = insInfoi->REF_begin(),
           E = insInfoi->REF_end(); I != E; I++) {
      for (auto &s : structInfos) {
        insInfoi->addRCStruct(I->first, s);
      }
      for (InsInfo::StructSliceInfoSet_t::iterator ssi_it = insInfoi->REFStruct_begin(I->first);
           ssi_it != insInfoi->REFStruct_end(I->first); ++ssi_it) {
        insInfoi->addRCStruct(I->first, *ssi_it);
      }

//        if (insInfoi->addRC(*I, insInfoi->getIns(), insInfoi->getREFInc(*I)))
      if (insInfoi->addRC(*I, insInfoi->getIns(), insInfoProvider, f_RC_min + insInfoi->getREFInc(*I)))
        changed = true;
    }
  }
#ifdef DEBUG_RC
  errs() << "  " << __func__ << "2 END";
  if (changed)
    errs() << " ----------CHANGED";
  errs() << '\n';
    insInfoi->dump();
#endif
  return changed;
}

bool FunctionStaticSlicer::computeRCi(InsInfo *insInfoi) {
  const Instruction *i = insInfoi->getIns();
  bool changed = false;
#ifdef DEBUG_RC
  errs() << "  " << __func__ << ": " << i->getOpcodeName();
  if (i->hasName())
    errs() << " (" << i->getName() << ")";
  errs() << '\n';
  errs() << "    DUMP: ";
  i->print(errs());
  errs() << '\n';
#endif
  SuccList succList = getSuccList(i);
  for (SuccList::const_iterator I = succList.begin(), E = succList.end();
       I != E; I++)
    changed |= computeRCi(insInfoi, getInsInfo(*I));

  return changed;
}

void FunctionStaticSlicer::computeRC() {
  bool changed;
#ifdef DEBUG_RC
  int it = 1;
#endif
  do {
    changed = false;
#ifdef DEBUG_RC
    errs() << __func__ << ": ============== Iteration " << it++ << '\n';
#endif
    bool dumpInfo = false;
    typedef std::reverse_iterator<Function::iterator> revFun;
    for (revFun I = revFun(fun.end()), E = revFun(fun.begin()); I != E; I++) {
      typedef std::reverse_iterator<BasicBlock::iterator> rev;
      InsInfo *past = NULL;
      for (rev II = rev(I->end()), EE = rev(I->begin()); II != EE; ++II) {
        InsInfo *insInfo = getInsInfo(&*II);
        if (!past)
          changed |= computeRCi(insInfo);
        else
          changed |= computeRCi(insInfo, past);
        past = insInfo;
        if (dumpInfo) {
          insInfo->dump();
        }
      }
    }
  } while (changed);
}

/*
 * SC(i)={i| DEF(i) \cap RC(j) \neq \emptyset}
 */
void FunctionStaticSlicer::computeSCi(const Instruction *i, const Instruction *j) {
  InsInfo *insInfoi = getInsInfo(i), *insInfoj = getInsInfo(j);


  bool isect_nonempty = false;
  for (ValSet::const_iterator I = insInfoi->DEF_begin(),
//       E = insInfoi->DEF_end(); I != E && !isect_nonempty; I++) {
         E = insInfoi->DEF_end(); I != E; I++) {
    const Pointee &DEFi = *I;
    for (ValSet::const_iterator II = insInfoj->RC_begin(),
           EE = insInfoj->RC_end(); II != EE; II++) {
      if (sameValues(DEFi, *II)) {
        for (auto &src : insInfoj->getRCSource(*II)) {
          if (!src)
            continue;
          if (const Instruction *srcIns = dyn_cast<const Instruction>(src)) {
            InsInfo *srcInfo = insInfoProvider->getInsInfo(srcIns);
            assert(srcInfo);
            //TODO: get this srcInfo from other functions
            if (srcInfo)
              srcInfo->addSlicedPredecessor(*II, i, insInfoProvider);
          }

        }
        isect_nonempty = true;
//        break;
      }
    }
  }

  if (isect_nonempty) {

    insInfoi->deslice(this);

#ifdef DEBUG_SLICING
    errs() << "XXXXXXXXXXXXXY ";
    i->print(errs());
    errs() << '\n';
#endif
  }
}

void FunctionStaticSlicer::computeSC() {
  for (inst_iterator I = inst_begin(fun), E = inst_end(fun); I != E; I++) {
    const Instruction *i = &*I;
    SuccList succList = getSuccList(i);
    for (SuccList::const_iterator II = succList.begin(), EE = succList.end();
         II != EE; II++)
      computeSCi(i, *II);
  }
}

bool FunctionStaticSlicer::computeBC() {
  bool changed = false;
#ifdef DEBUG_BC
  errs() << __func__ << " ============ BEG\n";
#endif
  passLock.lock();
  PostDominanceFrontier &PDF = MP->getAnalysis<PostDominanceFrontier>(fun);
  for (inst_iterator I = inst_begin(fun), E = inst_end(fun); I != E; I++) {
    Instruction *i = &*I;
    const InsInfo *ii = getInsInfo(i);
    if (ii->isSliced())
      continue;
    BasicBlock *BB = i->getParent();
#ifdef DEBUG_BC
    errs() << "  ";
    i->print(errs());
    errs() << " -> bb=" << BB->getName() << '\n';
#endif
    PostDominanceFrontier::const_iterator frontier = PDF.find(BB);
    if (frontier == PDF.end())
      continue;
    changed |= updateRCSC(frontier->second.begin(), frontier->second.end());
  }
  passLock.unlock();
#ifdef DEBUG_BC
  errs() << __func__ << " ============ END\n";
#endif
  return changed;
}

bool FunctionStaticSlicer::updateRCSC(
  PostDominanceFrontier::DomSetType::const_iterator start,
  PostDominanceFrontier::DomSetType::const_iterator end) {
  bool changed = false;
#ifdef DEBUG_RC
  errs() << __func__ << " ============ BEG\n";
#endif
  for (; start != end; start++) {
    const BasicBlock *BB = *start;
    const Instruction &i = BB->back();
    InsInfo *ii = getInsInfo(&i);
    /* SC = BC \cup ... */
#ifdef DEBUG_SLICING
    errs() << "XXXXXXXXXXXXXX " << BB->getName() << " ";
    i.print(errs());
    errs() << '\n';
#endif
    ii->deslice(this);
    /* RC = ... \cup \cup(b \in BC) RB */
    for (ValSet::const_iterator II = ii->REF_begin(), EE = ii->REF_end();
         II != EE; II++)
      //TODO: is this instruction the correct one as RC source?
      if (ii->addRC(*II, ii->getIns(), insInfoProvider)) {
        changed = true;
#ifdef DEBUG_RC

        errs() << "  added " << II->first->getName() << "\n";
#endif
      }
  }
#ifdef DEBUG_RC
  errs() << __func__ << " ============ END: changed=" << changed << "\n";
#endif
  return changed;
}


void FunctionStaticSlicer::dump() {
#ifdef DEBUG_DUMP
  for (inst_iterator I = inst_begin(fun), E = inst_end(fun); I != E; I++) {
    const Instruction &i = *I;
    const InsInfo *ii = getInsInfo(&i);
    i.print(errs());
    errs() << "\n    ";
    if (!ii->isSliced() || !canSlice(i))
      errs() << "UN";
    errs() << "SLICED\n    DEF:\n";
    for (ValSet::const_iterator II = ii->DEF_begin(), EE = ii->DEF_end();
         II != EE; II++) {
      errs() << "      OFF=" << II->second << " ";
      II->first->dump();
    }
    errs() << "    REF:\n";
    for (ValSet::const_iterator II = ii->REF_begin(), EE = ii->REF_end();
         II != EE; II++) {
      errs() << "      OFF=" << II->second << " ";
      II->first->dump();
    }
    errs() << "    RC:\n";
    for (ValSet::const_iterator II = ii->RC_begin(), EE = ii->RC_end();
         II != EE; II++) {
      errs() << "      OFF=" << II->second << " ";
      II->first->dump();
    }
  }
#endif
}

/**
 * this method calculates the static slice for the CFG
 */
void FunctionStaticSlicer::calculateStaticSlice() {
  //If we can't get the lock some other thread handles this function..
//    if (!slicerLock.try_lock())
//        return;
  slicerLock.lock();
  DEBUG(errs() << "Slice: " << fun.getName() << "\n");
#ifdef DEBUG_SLICE
  errs() << __func__ << " ============ BEG\n";
#endif
  do {
#ifdef DEBUG_SLICE
    errs() << __func__ << " ======= compute RC\n";
#endif
    computeRC();
#ifdef DEBUG_SLICE
    errs() << __func__ << " ======= compute SC\n";
#endif
    computeSC();

#ifdef DEBUG_SLICE
    errs() << __func__ << " ======= compute BC\n";
#endif
  } while (computeBC());
  slicerLock.unlock();

//  dump();

#ifdef DEBUG_SLICE
  errs() << __func__ << " ============ END\n";
#endif
}

bool FunctionStaticSlicer::slice() {
#ifdef DEBUG_SLICE
  errs() << __func__ << " ============ BEG\n";
#endif
  bool removed = false;
  for (inst_iterator I = inst_begin(fun), E = inst_end(fun); I != E;) {
    Instruction &i = *I;
    InsInfoMap::iterator ii_iter = insInfoMap.find(&i);
    assert(ii_iter != insInfoMap.end() || !infosInitialized);
    const InsInfo *ii = infosInitialized ? ii_iter->second : nullptr;
    ++I;

    if ((!infosInitialized || ii->isSliced()) && canSlice(i)) {
#ifdef DEBUG_SLICE
      errs() << "  removing:";
      i.print(errs());
      errs() << " from " << i.getParent()->getName() << '\n';
#endif
      i.replaceAllUsesWith(UndefValue::get(i.getType()));
      i.eraseFromParent();
      if (infosInitialized && ii_iter != insInfoMap.end()) {
//            insInfoMap.erase(ii_iter);
//            delete ii;
      }
      removed = true;
    }
  }
  return removed;
}

/**
 * removeUndefBranches -- remove branches with undef condition
 *
 * These are irrelevant to the code, so may be removed completely with their
 * bodies.
 */
void FunctionStaticSlicer::removeUndefBranches(ModulePass *MP, Function &F) {
#ifdef DEBUG_SLICE
  errs() << __func__ << " ============ Removing unused branches\n";
#endif
  passLock.lock();
  PostDominatorTree &PDT = MP->getAnalysis<PostDominatorTree>(F);

  typedef llvm::SmallVector<const BasicBlock *, 10> Unsafe;
  Unsafe unsafe;

  for (Function::iterator I = F.begin(), E = F.end(); I != E; ++I) {
    BasicBlock &bb = *I;
    if (std::distance(succ_begin(&bb), succ_end(&bb)) <= 1)
      continue;
    Instruction &back = bb.back();
    if (back.getOpcode() != Instruction::Br &&
        back.getOpcode() != Instruction::Switch)
      continue;
    const Value *cond = back.getOperand(0);
    if (cond->getValueID() != Value::UndefValueVal)
      continue;
    DomTreeNode *node = PDT.getNode(&bb);
    if (!node) /* this bb is unreachable */
      continue;
    DomTreeNode *idom = node->getIDom();
    assert(idom);
/*    if (!idom)
      continue;*/
    BasicBlock *dest = idom->getBlock();
    if (!dest) /* TODO when there are nodes with noreturn calls */
      continue;
#ifdef DEBUG_SLICE
    errs() << "  considering branch: " << bb.getName() << '\n';
    errs() << "  dest=" << dest->getName() << "\n";
#endif
    if (PHINode *PHI = dyn_cast<PHINode>(&dest->front())) if (PHI->getBasicBlockIndex(&bb) == -1) {
      /* TODO this is unsafe! */
      unsafe.push_back(&bb);
      PHI->addIncoming(Constant::getNullValue(PHI->getType()), &bb);
    }
    BasicBlock::iterator ii(back);
    Instruction *newI = BranchInst::Create(dest);
    ReplaceInstWithInst(bb.getInstList(), ii, newI);
  }
  for (Unsafe::const_iterator I = unsafe.begin(), E = unsafe.end();
       I != E; ++I) {
    const BasicBlock *bb = *I;
    if (std::distance(pred_begin(bb), pred_end(bb)) > 1)
      errs() << "WARNING: PHI node with added value which is zero\n";
  }
  passLock.unlock();
#ifdef DEBUG_SLICE
  errs() << __func__ << " ============ END\n";
#endif
}

/**
 * removeUndefCalls -- remove calls with undef function
 *
 * These are irrelevant to the code, so may be removed completely.
 */
void FunctionStaticSlicer::removeUndefCalls(ModulePass *MP, Function &F) {
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E;) {
    CallInst *CI = dyn_cast<CallInst>(&*I);
    ++I;
    if (CI && isa<UndefValue>(CI->getCalledValue())) {
      CI->replaceAllUsesWith(UndefValue::get(CI->getType()));
      CI->eraseFromParent();
    }
  }
}

void FunctionStaticSlicer::removeUndefs(ModulePass *MP, Function &F) {
  removeUndefBranches(MP, F);
  removeUndefCalls(MP, F);
}

void FunctionStaticSlicer::dumpInfos() {
  for (inst_iterator it = inst_begin(fun); it != inst_end(fun); ++it) {
    insInfoMap[&*it]->dump();
  }
}


static bool handleAssert(Function &F, FunctionStaticSlicer &ss,
                         const CallInst *CI) {

  const char *ass_file = getenv("SLICE_ASSERT_FILE");
  const char *ass_line = getenv("SLICE_ASSERT_LINE");
  const ConstantExpr *fileArg = dyn_cast<ConstantExpr>(CI->getArgOperand(1));
  const ConstantInt *lineArg = dyn_cast<ConstantInt>(CI->getArgOperand(2));

  if (ass_file && ass_line) {
    if (fileArg && fileArg->getOpcode() == Instruction::GetElementPtr &&
        lineArg) {
      const GlobalVariable *strVar =
        dyn_cast<GlobalVariable>(fileArg->getOperand(0));
      assert(strVar && strVar->hasInitializer());
      const ConstantDataArray *str =
        dyn_cast<ConstantDataArray>(strVar->getInitializer());
      assert(str && str->isCString());
      /* trim the NUL terminator */
      StringRef fileArgStr = str->getAsString().drop_back(1);
      const int ass_line_int = atoi(ass_line);

      errs() << "ASSERT at " << fileArgStr << ":" << lineArg->getValue() << "\n";

      if (fileArgStr.equals(ass_file) && lineArg->equalsInt(ass_line_int)) {
        errs() << "\tMATCH\n";
        goto count;
      }
    }
    ss.addSkipAssert(CI);
    return false;
  }

  count:
#ifdef DEBUG_INITCRIT
  errs() << "    adding\n";
#endif

  const Value *aif = F.getParent()->getGlobalVariable("__ai_init_functions",
                                                      true);
  ss.addInitialCriterion(CI, ptr::PointsToSets::Pointee(aif, -1));

  return true;
}

bool llvm::slicing::findInitialCriterion(llvm::Function &F, FunctionStaticSlicer &ss,
                                         std::vector<Rule *> &rules) {

  bool added = false;

  auto addCriterion = [&](std::string functionName, const Instruction *inst, uint64_t regNo, Rule &r,
                          std::vector<Rule *> preconditions) {
    DetectParametersPass::UserSet_t pre = DetectParametersPass::getRegisterValuesBeforeCall(regNo, inst, true);
    for (auto &p_it : pre) {
      Rule::InstructionRuleList_t preconditionInstructions;

      for (auto &preCond : preconditions) {
        for (auto &preCrit : preCond->getCriterions()) {
          if (preCrit.second.first.getFunctionName() != functionName) {
            continue;
            llvm_unreachable("Precondition has to be for the same function");
          }
          DetectParametersPass::UserSet_t prePreCond = DetectParametersPass::getRegisterValuesBeforeCall(
            preCrit.second.first.getRegNo(), (Instruction *) inst, true);
          for (auto &prePreCond_it : prePreCond) {
            const Instruction *prePreInst = dyn_cast<const Instruction>(prePreCond_it);
            Rule::InstructionRule_t instRule(prePreInst, (Rule *) preCrit.first);
            preconditionInstructions.push_back(instRule);
            ss.addInitialCriterion(inst, ptr::PointsToSets::Pointee(prePreInst, -1));
          }
        }
      }

      ss.addInitialCriterion(inst, ptr::PointsToSets::Pointee(p_it, -1));
      r.addInitialInstruction(inst, dyn_cast<const Instruction>(p_it), preconditionInstructions);
      added = true;
    }
  };

  for (inst_iterator inst_it = inst_begin(F), E = inst_end(F); inst_it != E; ++inst_it) {
    if (inst_it->getOpcode() == Instruction::Call) {
      SimpleCallGraph::FunctionSet_t calledFunctions = ptr::getSimpleCallGraph().getCalled(&*inst_it);
      for (auto &rule : rules) {
        for (auto &criterion : rule->getCriterions()) {
          for (auto &called : calledFunctions) {
            if (called == criterion.second.first.getFunctionName()) {
              addCriterion(called, &*inst_it, criterion.second.first.getRegNo(), *(Rule *) criterion.first,
                           criterion.second.second);
              errs() << "Found call to: " << called << "\n";
            }

            if (called == "objc_setProperty") {
              DetectParametersPass::UserSet_t PreVal = DetectParametersPass::getRegisterValuesBeforeCall(7, &*inst_it);
              for (auto &pre : PreVal) {
                Instruction *sext = nullptr;
                if (PatternMatch::match(pre, PatternMatch::m_SExt(PatternMatch::m_Instruction(sext)))) {
                  std::vector<const Value *> ptsToSet;
                  ptr::getAndersen()->getPointsToSet(sext->getOperand(0), ptsToSet);

                  for (auto &ptsTo : ptsToSet) {
                    for (auto &relevant : rule->getRelevantVariables()) {
                      if (ptsTo != relevant) {
                        continue;
                      }
                      ss.addInitialCriterion(&*inst_it, ptr::PointsToSets::Pointee(&*inst_it, -1));
                      rule->addInitialInstruction(nullptr, &*inst_it, Rule::InstructionRuleList_t());
                      added = true;
                    }
                  }
                }
              }
            }
          }
        }
        for (auto &called : calledFunctions) {
          if (called == "objc_setProperty") {
            DetectParametersPass::UserSet_t PreVal = DetectParametersPass::getRegisterValuesBeforeCall(7, &*inst_it);
            for (auto &pre : PreVal) {
              Instruction *sext = nullptr;
              if (PatternMatch::match(pre, PatternMatch::m_SExt(PatternMatch::m_Instruction(sext)))) {
                std::vector<const Value *> ptsToSet;
                ptr::getAndersen()->getPointsToSet(sext->getOperand(0), ptsToSet);

                for (auto &ptsTo : ptsToSet) {
                  for (auto &relevant : rule->getRelevantVariables()) {
                    if (ptsTo == relevant) {
                      DetectParametersPass::UserSet_t PreStore = DetectParametersPass::getRegisterValuesBeforeCall(8,
                                                                                                                   &*inst_it,
                                                                                                                   true);
                      for (auto &store : PreStore) {
                        ss.addInitialCriterion((Instruction *) store,
                                               ptr::PointsToSets::Pointee(store->getOperand(0), -1));
                        rule->addInitialInstruction(nullptr, (Instruction *) store, Rule::InstructionRuleList_t());
                        added = true;
                      }
                    }
                  }
                }
              }
            }
          }
        }

//                for (auto &criterion : rule.getCriterions()) {
//
//
//                    if (const CallInst *call = dyn_cast<const CallInst>(&*inst_it)) {
//                        if (call->getCalledFunction() &&
//                            call->getCalledFunction()->getName() == criterion.getFunctionName()) {
//                            addCriterion(&*inst_it, criterion.getRegNumber(), *((Rule*)&rule));
//                        }
//                    }
//
//                    for (auto &called : calledFunctions) {
//                        if (called == criterion.getFunctionName()) {
//                            addCriterion(&*inst_it, criterion.getRegNumber(), *((Rule*)&rule));
//                        }
//                    }
//                }
      }
    } else if (inst_it->getOpcode() == Instruction::Store) {
      if (inst_it->getOperand(0)->getName() == "X0_67") {
        assert(true);
      }
      for (auto &rule : rules) {
        if (rule->getRelevantLocation()) {
          Instruction *sext = nullptr;
          Value *v = inst_it->getOperand(1);
          if (PatternMatch::match(v, PatternMatch::m_IntToPtr(
            PatternMatch::m_Add(PatternMatch::m_Value(), PatternMatch::m_SExt(PatternMatch::m_Instruction(sext)))))) {
            std::vector<const Value *> ptsToSet;
            ptr::getAndersen()->getPointsToSet(sext->getOperand(0), ptsToSet);
            for (auto &relevant : rule->getRelevantVariables()) {
              for (auto &ptsTo : ptsToSet) {
                if (relevant == ptsTo) {
                  ss.addInitialCriterion(&*inst_it, ptr::PointsToSets::Pointee(inst_it->getOperand(0), -1));
                  rule->addInitialInstruction(nullptr, &*inst_it, Rule::InstructionRuleList_t());
                  added = true;
                }
              }
            }
          } else {
            std::vector<const Value *> ptsToSet;
            ptr::getAndersen()->getPointsToSet(inst_it->getOperand(1), ptsToSet);
            for (auto &relevant : rule->getRelevantVariables()) {
              for (auto &ptsTo : ptsToSet) {
                if (relevant == ptsTo) {
                  ss.addInitialCriterion(&*inst_it, ptr::PointsToSets::Pointee(inst_it->getOperand(0), -1));
                  rule->addInitialInstruction(nullptr, &*inst_it, Rule::InstructionRuleList_t());
                  added = true;
                }
              }
            }
          }
        }
      }

    }
  }
  return added;
}

bool llvm::slicing::findInitialCriterion(Function &F,
                                         FunctionStaticSlicer &ss,
                                         bool starting) {
  bool added = false;
#ifdef DEBUG_INITCRIT
  errs() << __func__ << " ============ BEGIN\n";
#endif
  const Function *Fklee_assume = F.getParent()->getFunction("klee_assume");
  const Function *F__assert_fail = F.getParent()->getFunction("__assert_fail");
  const Function *F_donothing = F.getParent()->getFunction("llvm.donothing");
  const Function *F_slice = F.getParent()->getFunction("llvm.slice");
//  if (!F__assert_fail) /* no cookies in this module */
//    return false;

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    const Instruction *i = &*I;
    if (const StoreInst *SI = dyn_cast<StoreInst>(i)) {
      const Value *LHS = SI->getPointerOperand();
      if (LHS->hasName() && LHS->getName().startswith("__ai_state_")) {
#ifdef DEBUG_INITCRIT
        errs() << "    adding\n";
#endif
        ss.addInitialCriterion(SI, ptr::PointsToSets::Pointee(LHS, -1));
      }
    } else if (const CallInst *CI = dyn_cast<CallInst>(i)) {
      Function *callie = CI->getCalledFunction();
      if (callie && F__assert_fail && callie == F__assert_fail) {
        added = handleAssert(F, ss, CI);
      } else if (callie && callie == Fklee_assume) { // this is kind of hack
        const Value *l = elimConstExpr(CI->getArgOperand(0));
        ss.addInitialCriterion(CI, ptr::PointsToSets::Pointee(l, -1));
        added = true;
      } else if (callie && callie == F_donothing) {
        ss.addInitialCriterion(CI);
        added = true;
      } else if (callie && callie == F_slice) {
        Value *v = CI->arg_operands().begin()->get();
        ss.addInitialCriterion(CI, ptr::PointsToSets::Pointee(v, -1));
        added = true;
      }
    } else if (const ReturnInst *RI = dyn_cast<ReturnInst>(i)) {
      if (starting) {
        const Module *M = F.getParent();
        for (Module::const_global_iterator II = M->global_begin(),
               EE = M->global_end(); II != EE; ++II) {
          const GlobalVariable &GV = *II;
          if (!GV.hasName() || !GV.getName().startswith("__ai_state_"))
            continue;
#ifdef DEBUG_INITCRIT
          errs() << "adding " << GV.getName() << " into " << F.getName() <<
              " to \n";
          RI->dump();
#endif
          ss.addInitialCriterion(RI, ptr::PointsToSets::Pointee(&GV, -1),
                                 false);
        }
      }
    }
  }
#ifdef DEBUG_INITCRIT
  errs() << __func__ << " ============ END\n";
#endif
  return added;
}

bool FunctionSlicer::runOnFunction(Function &F, const ptr::PointsToSets &PS,
                                   const mods::Modifies &MOD) {
  FunctionStaticSlicer ss(F, this, PS, MOD);

  findInitialCriterion(F, ss);

  ss.calculateStaticSlice();

  bool sliced = ss.slice();
  if (sliced)
    FunctionStaticSlicer::removeUndefs(this, F);

  return sliced;
}

bool FunctionSlicer::runOnModule(Module &M) {
  ptr::PointsToSets PS;
  {
    ptr::ProgramStructure P(M);
    computePointsToSets(P, PS);
  }

  callgraph::Callgraph CG(M, PS);

  mods::Modifies MOD;
  {
    mods::ProgramStructure P1(M, PS);
    computeModifies(P1, CG, PS, MOD);
  }

  bool modified = false;
  for (Module::iterator I = M.begin(), E = M.end(); I != E; ++I) {
    Function &F = *I;
    if (!F.isDeclaration())
      modified |= runOnFunction(F, PS, MOD);
  }
  return modified;
}



//InsInfo

bool InsInfo::addRC(const Pointee &var, const Value *src, InsInfoProvider *provider, IncType_t RCInc) {
  bool changed = false;
  if (RCInc < INC_MAX) {
    if (RCSources.find(var.first) == RCSources.end())
      RCSources[var.first] = ValSet_t();
    ValSet_t &v = RCSources[var.first];
    if (src)
      v.insert(src);
  }
  IncMap_t::iterator i = RCIncMap.find(var.first);
  if (RCInc < INC_MAX) {
    if (i == RCIncMap.end()) {
      RCIncMap[var.first] = RCInc;
      changed = true;
    }
    else {
      if (i->second > RCInc) {
        RCIncMap[var.first] = RCInc;
        changed = true;
      }
    }

  }
  if (std::find(RC.begin(), RC.end(), var) != RC.end()) {

  } else {
    RC.insert(var);
    changed = true;
  }
  return changed;
}

bool InsInfo::addRC(const Pointee &var, const ValSet_t &sources, InsInfoProvider *provider, IncType_t RCInc) {
  bool r = addRC(var, NULL, provider, RCInc);
  if (RCInc < INC_MAX) {
    if (RCSources.find(var.first) == RCSources.end())
      RCSources[var.first] = ValSet_t();
    ValSet_t &v = RCSources[var.first];
    for (auto &s : sources) {
      if (std::find(v.begin(), v.end(), s) == v.end()) {
        v.insert(s);
        r = true;
      }
    }
  }
  return r;
}

bool InsInfo::addDEF(const Pointee &var) {
  if (ptr::getAndersen()->isDummyHelper(var.first))
    return false;

  if (std::find(DEF.begin(), DEF.end(), var) != DEF.end()) {
    return false;
  }
  DEF.insert(var);
  return true;
}


bool InsInfo::addREF(const Pointee &var, IncType_t RefInc) {
  if (ptr::getAndersen()->isDummyHelper(var.first))
    return false;
  if (dyn_cast<const ConstantInt>(var.first)) {
    return false;
  }
  if (RefInc < INC_MAX) {
    IncMap_t::iterator i = RefIncMap.find(var.first);
    if (i == RefIncMap.end())
      RefIncMap[var.first] = RefInc;
    else
      RefIncMap[var.first] = i->second < RefInc ? i->second : RefInc;
  }
  if (std::find(REF.begin(), REF.end(), var) != REF.end()) {
    return false;
  }
  REF.insert(var);
  return true;
}

void InsInfo::addRCStruct(const Value *ref, const StructSliceInfo *ssi) {
  bool hasRC = false;
  for (ValSet::const_iterator RC_it = RC_begin(); RC_it != RC_end(); ++RC_it) {
    for (auto &l : ssi->locations) {
      if (RC_it->first == l.first) {
        hasRC = true;
        break;
      }
    }
  }
  if (hasRC)
    RCStructInfos[ref].insert((StructSliceInfo *) ssi);
}

IncType_t InsInfo::getRCInc(const Pointee &var) {
  IncMap_t::iterator i = RCIncMap.find(var.first);
  if (i == RCIncMap.end())
    return INC_MAX;
  return i->second;
}

IncType_t InsInfo::getREFInc(const Pointee &var) {
  IncMap_t::iterator i = RefIncMap.find(var.first);
  if (i == RefIncMap.end())
    return INC_MAX;
  return i->second;
}

void InsInfo::addSlicedPredecessor(const Pointee &RC, const Instruction *Pred, InsInfoProvider *provider) {

  if (SlicedPredecessors.find(RC.first) == SlicedPredecessors.end())
    SlicedPredecessors[RC.first] = std::set<const Instruction *>();
  std::set<const Instruction *> &Preds = SlicedPredecessors[RC.first];
  if (Preds.find(Pred) != Preds.end())
    return;
  if (Pred != ins)
    Preds.insert(Pred);
  else {
    assert(true);
  }

  for (auto &succ : UpSuccessors[RC.first]) {
    const Instruction *i = dyn_cast<const Instruction>(succ);
    if (!i)
      continue;
    InsInfo *p = provider->getInsInfo(i);
    if (!p || p == this)
      continue;
    p->addSlicedPredecessor(RC, ins, provider);
  }
}
