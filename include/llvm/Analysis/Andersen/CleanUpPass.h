#ifndef LLVM_CLEANUPPASS_H
#define LLVM_CLEANUPPASS_H

#include "llvm/Pass.h"

namespace llvm {
    class CleanUpPass : public ModulePass {
    public:
        static char ID;

        CleanUpPass() : ModulePass(ID) { };

        virtual bool runOnModule(Module &M);

    private:
    };
}

#endif //LLVM_CLEANUPPASS_H
