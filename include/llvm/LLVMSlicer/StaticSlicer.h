#ifndef LLVM_STATICSLICER_H_H
#define LLVM_STATICSLICER_H_H

#include "llvm/Pass.h"

namespace llvm {

    namespace slicing {

    }

    class Slicer : public ModulePass {
    public:
        static char ID;

        Slicer() : ModulePass(ID) {}

        virtual bool runOnModule(Module &M);

        void getAnalysisUsage(AnalysisUsage &AU) const;
    };
}


#endif //LLVM_STATICSLICER_H_H
