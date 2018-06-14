#include "llvm/Analysis/Andersen/CleanUpPass.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstIterator.h"
#include <set>
#include <string>
#include "llvm/Support/raw_ostream.h"

using namespace llvm;


bool CleanUpPass::runOnModule(Module &M) {

    std::set<std::string> functions;
    functions.insert("objc_release");
    functions.insert("objc_retainAutoreleasedReturnValue");
    functions.insert("objc_autorelease");
    functions.insert("objc_retainAutorelease");
    functions.insert("objc_retain");
    functions.insert("objc_autoreleaseReturnValue");

    std::set<Instruction*> toRemove;

    size_t totalCalls = 0;

    for (auto &f : M.functions()) {

        for (inst_iterator i_it = inst_begin(f); i_it != inst_end(f); ++i_it) {

            if (i_it->getOpcode() == Instruction::Call) {
                totalCalls++;
                CallInst *callInst = dyn_cast<CallInst>(&*i_it);
                Function *calledFunction = callInst->getCalledFunction();
                if (calledFunction) {
                    if (calledFunction->hasName() && functions.find(calledFunction->getName()) != functions.end()) {
                        toRemove.insert(callInst);
                    }
                }
            }


        }
    }

    errs() << "Remove " << toRemove.size() << " calls " <<  (toRemove.size()*100)/totalCalls << "%\n";

    for (auto &r : toRemove) {
//                r->eraseFromParent();
                r->removeFromParent();
    }

    for (auto &f : functions) {
        Function *fun = M.getFunction(f);
        if (fun) {
            fun->removeFromParent();
        }
    }

    return true;
}

char CleanUpPass::ID = 0;
static RegisterPass<CleanUpPass> X("cleanup", "", true, false);
