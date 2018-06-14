#include <llvm/IR/Function.h>
#include "llvm/Analysis/Andersen/NonVolatileRegistersPass.h"

#include <set>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/Instructions.h>
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;


bool NonVolatileRegistersPass::runOnModule(Module &M) {
    for (Module::iterator F_it = M.begin(); F_it != M.end(); ++F_it) {
        Function &F = *F_it;
        if (F.isDeclaration() || F.isIntrinsic()) {
            continue;
        }
        std::set<Value *> NonVolatileAndStackPointers;
        std::set<Value *> NonVolatilePointers;

        BasicBlock &Entry = F.getEntryBlock();
        for (BasicBlock::iterator I_it = Entry.begin(); I_it != Entry.end(); ++I_it) {
            if (I_it->getOpcode() == Instruction::GetElementPtr) {
                if (ConstantInt *ConstIdx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
                    if (isNonVolatile(ConstIdx->getZExtValue())) {
                        NonVolatileAndStackPointers.insert(&*I_it);
                        NonVolatilePointers.insert(&*I_it);
                    } else if (isStack(ConstIdx->getZExtValue())) {
                        NonVolatileAndStackPointers.insert(&*I_it);
                    }
                }
            }
        }

        for (Function::iterator BB_it = F.begin(); BB_it != F.end(); ++BB_it) {
            if (!hasCall(*BB_it))
                continue;
            for (BasicBlock::iterator I_it = BB_it->begin(); I_it != BB_it->end(); ++I_it) {
                if (I_it->getOpcode() == Instruction::Store) {
                    std::set<Value *>::iterator S_it = NonVolatileAndStackPointers.find(I_it->getOperand(1));
                    if (S_it != NonVolatileAndStackPointers.end()) {
                        replaceLoaded(&*I_it);
                    }
                }
            }
        }

        std::set<Instruction*> toRemove;

        for (auto &ptr : NonVolatilePointers) {

            for (auto ptrUser : ptr->users()) {
                if (StoreInst *storeInst = dyn_cast<StoreInst>(ptrUser)) {
                    toRemove.insert(storeInst);
                }
            }
        }

        for (auto &r : toRemove) {
            r->dropAllReferences();
            r->removeFromParent();
//                r->dump();
        }
    }
}

bool NonVolatileRegistersPass::hasCall(const BasicBlock &BB) {
    for (BasicBlock::const_iterator I_it = BB.begin(); I_it != BB.end(); ++I_it) {
        if (I_it->getOpcode() == Instruction::Call) {
            return true;
        }
    }
    return false;
}

bool NonVolatileRegistersPass::isNonVolatile(uint64_t Idx) {
    switch (Idx) {
        default:
            break;
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
            return true;
    }
    return false;
}

bool NonVolatileRegistersPass::isStack(uint64_t Idx) {
    switch (Idx) {
        default:
            break;
        case 3:
        case 0:
            return true;
    }
    return false;
}

void NonVolatileRegistersPass::replaceLoaded(Instruction *StoreInst) {
    Value *Ptr = StoreInst->getOperand(1);
    Value *V = StoreInst->getOperand(0);
    if (!V)
        StoreInst->dump();
    assert(V);

    BasicBlock *BB = StoreInst->getParent();
    Instruction *Load = nullptr;
    for (BasicBlock::iterator I_it = BB->begin(); I_it != BB->end(); ++I_it) {
        if (I_it->getOpcode() == Instruction::Load && I_it->getOperand(0) == Ptr) {
            Load = &*I_it;
        }
    }

    if (!Load)
        return;

//    for (Value::const_use_iterator U_it = Load->use_begin(); U_it != Load->use_end(); ++U_it) {
//
//    }
    if (Load != V)
        Load->replaceAllUsesWith(V);
}

char NonVolatileRegistersPass::ID = 0;
static RegisterPass<NonVolatileRegistersPass> X("nonvolatile", "", true, false);
