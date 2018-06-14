#include <llvm/IR/InstIterator.h>
#include "llvm/Analysis/Andersen/StackAccessPass.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constants.h"
#include <vector>
#include <deque>
#include <llvm/IR/CFG.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/PatternMatch.h>
#include "llvm/IR/Module.h"
#include <llvm/ProfileData/InstrProfReader.h>
#include <llvm/IR/Dominators.h>
#include "llvm/Support/Debug.h"

using namespace llvm;

#define DEBUG_TYPE "stack_access"

char StackAccessPass::ID = 0;
static RegisterPass<StackAccessPass> X("stack-access", "Scans for Stack variables",
                                       true /* Only looks at CFG */,
                                       true /* Analysis Pass */);

void StackAccessPass::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.setPreservesAll();
}

bool StackAccessPass::runOnModule(Module &M) {
    for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
        F->hasName();
    }
    for (auto &F : M.functions()) {
        if (F.isDeclaration() || F.isIntrinsic())
            continue;
        if (Offsets.find(&F) != Offsets.end() && ValuesForOffset.find(&F) != ValuesForOffset.end()) {
            return true;
        }
        CurrentFunction = &F;

        Offsets[&F] = std::shared_ptr<OffsetMap_t>(new OffsetMap_t());
        OffsetMap_t &OffsetMap = *Offsets[&F];
        ValuesForOffset[&F] = std::shared_ptr<OffsetValueListMap_t>(new OffsetValueListMap_t());
        OffsetValueListMap_t &OffsetValueListMap = *ValuesForOffset[&F];

        std::set<uint64_t> SPIdx;
        SPIdx.insert(3);
        SPIdx.insert(0);

        runOnFunction(F, OffsetMap, OffsetValueListMap, SPIdx);
    }

    return true;
}

void StackAccessPass::runOnFunction(Function &F, OffsetMap_t &OffsetMap, OffsetValueListMap_t &OffsetValueListMap, std::set<uint64_t> SPIdx) {
    DEBUG(errs() << "FUNCTION: " << F.getName() << "\n";);

    std::deque<const Instruction*> IntToPtrInstructions;

    for (inst_iterator I_it = inst_begin(F); I_it != inst_end(F); ++I_it) {
        if (I_it->getOpcode() == Instruction::IntToPtr) {
            IntToPtrInstructions.push_back(dyn_cast<const Instruction>(&*I_it->getOperand(0)));
        } else if (I_it->getOpcode() == Instruction::Store &&
                   PatternMatch::match((Value*)I_it->getOperand(0), PatternMatch::m_BinOp(PatternMatch::m_Value(), PatternMatch::m_ConstantInt()))) {
            //If something gets passed as stack stored parameter there will be no 'inttoptr' instruction
            IntToPtrInstructions.push_back(dyn_cast<const Instruction>(&*I_it->getOperand(0)));
        }
    }

    DEBUG(errs() << "#IntToPtrInstructions: " << IntToPtrInstructions.size() << "\n");


    std::set<const Instruction*> handled;

    while (IntToPtrInstructions.size()) {
        const Instruction *I = IntToPtrInstructions.front();
        IntToPtrInstructions.pop_front();

        if (handled.find(I) != handled.end())
            continue;
        handled.insert(I);

        std::set<int64_t> Results = backtrackInstruction(I, IntToPtrInstructions, SPIdx);
        if (!Results.size())
            continue;
        DEBUG(I->dump());

        OffsetMap[I] = std::shared_ptr<Int64List_t>(new Int64List_t());
        Int64List_t &O = *OffsetMap[I];


        for (std::set<int64_t>::iterator it = Results.begin(); it != Results.end(); ++it) {
            if (OffsetValueListMap.find(*it) == OffsetValueListMap.end())
                OffsetValueListMap[*it] = std::shared_ptr<ValueList_t>(new ValueList_t());
            OffsetValueListMap[*it]->insert(I);
            O.insert(*it);
            DEBUG(errs() << *it << "\t");
        }
        DEBUG(errs() << "\n");
    }

    DEBUG(
            errs() << "Values for Offsets:\n";
            for (OffsetValueListMap_t::iterator OV_it = OffsetValueListMap.begin(); OV_it != OffsetValueListMap.end(); ++OV_it) {
                errs() << OV_it->first << "\n";
                for (ValueList_t::iterator V_it = OV_it->second->begin(); V_it != OV_it->second->end(); ++V_it) {
                    (*V_it)->dump();
                }
                errs() << "\n";
            }
    );
}

bool StackAccessPass::isStackPointer(Value *Ptr, std::set<uint64_t> SPIdx) {
    assert(SPIdx.size());
    if (Instruction *I = dyn_cast<Instruction>(Ptr)) {
        if (I->getOpcode() == Instruction::GetElementPtr) {
            if (ConstantInt *Idx = dyn_cast<ConstantInt>(I->getOperand(2))) {
                if (SPIdx.find(Idx->getZExtValue()) != SPIdx.end()) {
                    return true;
                }
            }
        }
    }
    return false;
}

const Instruction *StackAccessPass::getStackPointer(const Function *F) {
    for (BasicBlock::InstListType::const_iterator I_it = F->getEntryBlock().getInstList().begin();
         I_it != F->getEntryBlock().getInstList().end();
         ++I_it) {
        if (I_it->getOpcode() == Instruction::GetElementPtr) {
            if (ConstantInt *IdxValue = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
                if (IdxValue->getZExtValue() == 3) {
                    return &*I_it;
                }
            }
        }
    }
    return nullptr;
}

int64_t StackAccessPass::getStackPointerValue(const Instruction *Inst, bool findStackPointer) {

    std::function<const Instruction* (const Instruction*, const Value*, bool)> getStore = [&](const Instruction *Inst, const Value *Ptr, bool searchPred){
        const Instruction *Store = nullptr;
        for (const Instruction *I = Inst; ; I = I->getPrevNode()) {
            if (I->getOpcode() == Instruction::Store && I->getOperand(1) == Ptr) {
                Store = &*I;
                break;
            }
            if (I == &I->getParent()->front())
                break;
        }
        if (!Store && searchPred) {
            size_t num = 0;
            const BasicBlock *pred = nullptr;
            for (const_pred_iterator p_it = pred_begin(Inst->getParent()); p_it != pred_end(Inst->getParent()); ++p_it) {
                pred = *p_it;
                ++num;
            }
            if (num == 1) {
                Store = getStore(pred->getTerminator(), Ptr, searchPred);
            } else {
                errs() << Inst->getParent()->getName() << "\n";
                assert(false && "Multiple predecessors...");
            }
        }
        return Store;
    };

    const Value *StackPtr = getStackPointer(Inst->getParent()->getParent());
    if (!StackPtr)
        return 0;
    const Instruction *Store = getStore(Inst, StackPtr, true);

    std::deque<std::pair<const Instruction *, int64_t>> Worklist;
    if (findStackPointer)
        Worklist.push_back(std::pair<const Instruction *, int64_t>(dyn_cast<Instruction>(Store->getOperand(0)), 0));
    else
        Worklist.push_back(std::pair<const Instruction *, int64_t>(Inst, 0));

    std::set<int64_t> Results;

    std::map<const Instruction*, int64_t> Visited;

    std::set<uint64_t> SPIdx;
    SPIdx.insert(3);
    SPIdx.insert(0);

    while (Worklist.size()) {
        std::pair<const Instruction *, int64_t> P = Worklist.front();
        Worklist.pop_front();

        const Instruction *CurrentInst = P.first;
        int64_t CurrentSize = P.second;

        bool Run = true;
        while (Run) {
            if (!CurrentInst) {
                llvm_unreachable("Should not happen...");
            }

            if (Visited.find(CurrentInst) != Visited.end()) {
                break;
            }
            Visited[CurrentInst] = CurrentSize;
            switch (CurrentInst->getOpcode()) {
                default:
                    CurrentInst->dump();
                    llvm_unreachable("Not handled");
                case Instruction::Load: {
                    if (!isStackPointer(CurrentInst->getOperand(0), SPIdx)) {
                        Run = false;
                        break;
                    }
                    assert(isStackPointer(CurrentInst->getOperand(0), SPIdx));
                    Store = getStore(CurrentInst, CurrentInst->getOperand(0), false);
                    if (!Store) {
                        Run = false;
                        Results.insert(-CurrentSize);
                    } else {
                        CurrentInst = dyn_cast<Instruction>(Store->getOperand(0));
                    }
                    break;
                }
                case Instruction::Add: {
                    Value *V = nullptr;
                    uint64_t Const = 0;
                    if (PatternMatch::match(CurrentInst, PatternMatch::m_Add(PatternMatch::m_Value(V), PatternMatch::m_ConstantInt(Const)))) {
                        CurrentInst = dyn_cast<Instruction>(V);
                        CurrentSize += (int64_t)Const;
                    } else {
                        Run = false;
                        break;
                        CurrentInst->dump();
                        llvm_unreachable("Add with non-constant int");
                    }
                    break;
                }
                case Instruction::Sub: {
                    Value *V = nullptr;
                    uint64_t Const = 0;
                    if (PatternMatch::match(CurrentInst, PatternMatch::m_Sub(PatternMatch::m_Value(V), PatternMatch::m_ConstantInt(Const)))) {
                        CurrentInst = dyn_cast<Instruction>(V);
                        CurrentSize -= (int64_t)Const;
                    } else {
                        Run = false;
                        break;
                        CurrentInst->dump();
                        llvm_unreachable("Sub with non-constant int");
                    }
                    break;
                }
                case Instruction::PHI: {
                    for (unsigned i = 1; i < CurrentInst->getNumOperands(); ++i) {
                        Worklist.push_back(std::pair<Instruction*, int64_t>(dyn_cast<Instruction>(CurrentInst->getOperand(i)), CurrentSize));
                    }
                    CurrentInst = dyn_cast<Instruction>(CurrentInst->getOperand(0));
                }
            }
        }
    }
    if (Results.size() != 1) {
        errs() << Inst->getParent()->getParent()->getName() << "\n";
        for (std::set<int64_t>::iterator i = Results.begin(); i != Results.end(); ++i) {
            errs() << *i << "\n";
        }
        if (Results.size() == 0) {
            errs() << "(Probably execption handling...)\n";
            return -1U;
        }
        assert(false);
    }
    return *Results.begin();
}

std::set<int64_t> StackAccessPass::backtrackInstruction(const Instruction *Inst, std::deque<const Instruction*> &InstList, const std::set<uint64_t> SPIdx) {
    std::function<const Instruction* (const Instruction*, const Value*)> getStore = [](const Instruction *Inst, const Value *Ptr){
        const Instruction *Store = nullptr;
        for (const Instruction *I = Inst; ; I = I->getPrevNode()) {
            if (I->getOpcode() == Instruction::Store && I->getOperand(1) == Ptr) {
                Store = &*I;
                break;
            }
            if (I == &I->getParent()->front())
                break;
        }
        return Store;
    };

    std::set<int64_t> Results;

    std::deque<std::pair<const Instruction *, int64_t>> Worklist;
    Worklist.push_back(std::pair<const Instruction *, int64_t>(Inst, 0));

    std::map<const Instruction*, int64_t> Visited;

    while (Worklist.size()) {
        std::pair<const Instruction *, int64_t> P = Worklist.front();
        Worklist.pop_front();

        const Instruction *CurrentInst = P.first;
        int64_t CurrentSize = P.second;

        bool Run = true;
        while (Run) {
            if (!CurrentInst) {
                break;
            }

            if (Visited.find(CurrentInst) != Visited.end()) {
                break;
            }
            Visited[CurrentInst] = CurrentSize;

            if (PatternMatch::match(CurrentInst, PatternMatch::m_BinOp(PatternMatch::m_Constant(), PatternMatch::m_Constant()))) {
                Run = false;
                continue;
            }
            switch (CurrentInst->getOpcode()) {
                default:
                    Run = false;
                    break;
                case Instruction::Load: {
                    if (!isStackPointer(CurrentInst->getOperand(0), SPIdx)) {
                        Run = false;
                        break;
                    }
                    assert(isStackPointer(CurrentInst->getOperand(0), SPIdx));
                    const Instruction *Store = getStore(CurrentInst, CurrentInst->getOperand(0));
                    if (!Store) {
                        Run = false;
                        Results.insert(CurrentSize);
                    } else {
                        CurrentInst = dyn_cast<Instruction>(Store->getOperand(0));
                    }
                    break;
                }
                case Instruction::Add: {
                    Value *V = nullptr;
                    uint64_t Const = 0;
                    if (PatternMatch::match(CurrentInst, PatternMatch::m_Add(PatternMatch::m_Value(V), PatternMatch::m_ConstantInt(Const)))) {
                        CurrentInst = dyn_cast<Instruction>(V);
                        CurrentSize += (int64_t)Const;
                    } else if (PatternMatch::match(CurrentInst, PatternMatch::m_Add(PatternMatch::m_BinOp(), PatternMatch::m_Value()))) {
                        InstList.push_back(dyn_cast<Instruction>(CurrentInst->getOperand(0)));
                        Run = false;
                        break;
                    }
                    break;
                }
                case Instruction::Sub: {
                    Value *V = nullptr;
                    uint64_t Const = 0;
                    if (PatternMatch::match(CurrentInst, PatternMatch::m_Sub(PatternMatch::m_Value(V), PatternMatch::m_ConstantInt(Const)))) {
                        CurrentInst = dyn_cast<Instruction>(V);
                        CurrentSize -= (int64_t)Const;
                    } else if (PatternMatch::match(CurrentInst, PatternMatch::m_Sub(PatternMatch::m_BinOp(), PatternMatch::m_Value()))) {
                        InstList.push_back(dyn_cast<Instruction>(CurrentInst->getOperand(0)));
                        Run = false;
                        break;
                    }
                    break;
                }
                case Instruction::PHI: {
                    for (unsigned i = 1; i < CurrentInst->getNumOperands(); ++i) {
                        //Skip Constants
                        if (Instruction *I = dyn_cast<Instruction>(CurrentInst->getOperand(i))) {
                            Worklist.push_back(std::pair<Instruction*, int64_t>(I, CurrentSize));
                        }
                    }
                    if (Instruction *I = dyn_cast<Instruction>(CurrentInst->getOperand(0))) {
                        CurrentInst = I;
                    } else {
                        Run = false;
                    }
                    break;
                }
                case Instruction::IntToPtr: {
                    CurrentInst = dyn_cast<Instruction>(CurrentInst->getOperand(0));
                    break;
                }
            }
        }
    }


    return Results;
}
