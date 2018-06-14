#ifndef LLVM_NONVOLATILEREGISTERSPASS_H
#define LLVM_NONVOLATILEREGISTERSPASS_H

#include "llvm/Pass.h"

namespace llvm {
    class NonVolatileRegistersPass : public ModulePass {
    public:
        static char ID;

        NonVolatileRegistersPass() : ModulePass(ID) { };

        virtual bool runOnModule(Module &M);

    private:
        bool hasCall(const BasicBlock &BB);
        bool isNonVolatile(uint64_t Idx);
        bool isStack(uint64_t Idx);
        void replaceLoaded(Instruction *StoreInst);
    };
}

#endif //LLVM_NONVOLATILEREGISTERSPASS_H
