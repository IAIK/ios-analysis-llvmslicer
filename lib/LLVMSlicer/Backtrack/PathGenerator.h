#ifndef LLVM_PATHGENERATOR_H
#define LLVM_PATHGENERATOR_H

namespace llvm {
    namespace slicing {

        class InsInfo;

        class PathGenerator {
        public:
            static void generate(InsInfo *info);
        };

    }
}

#endif //LLVM_PATHGENERATOR_H
