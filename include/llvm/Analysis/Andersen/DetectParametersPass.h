#ifndef LLVM_DETECTPARAMETERSPASS_H
#define LLVM_DETECTPARAMETERSPASS_H

#include "llvm/Pass.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Instruction.h"
#include <vector>
#include <set>
#include <map>
#include <mutex>

namespace llvm {

    typedef std::set<int64_t> Int64Set_t;

    class DetectParametersPass : public ModulePass {
    public:
        typedef std::pair<uint64_t, Instruction*> ParameterAccessPair_t;
        typedef std::set<ParameterAccessPair_t> ParameterAccessPairSet_t;

        static char ID;
        DetectParametersPass() : ModulePass(ID) {}
        virtual void getAnalysisUsage(AnalysisUsage &) const;
        virtual bool runOnModule(Module &M);

        ParameterAccessPairSet_t &getParameterStackOffsets(const Function *F) {
            if (StackOffsets.find(F) == StackOffsets.end())
                StackOffsets[F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
            return *StackOffsets[F];
        };
        ParameterAccessPairSet_t &getParameterRegisterIndexes(const Function *F){
            if (RegisterIndexes.find(F) == RegisterIndexes.end())
                RegisterIndexes[F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
            return *RegisterIndexes[F];
        };

        ParameterAccessPairSet_t &getReturnRegisterIndexes(const Function *F){
            if (ReturnIndexes.find(F) == ReturnIndexes.end())
                ReturnIndexes[F] = std::unique_ptr<ParameterAccessPairSet_t>(new ParameterAccessPairSet_t());
            return *ReturnIndexes[F];
        };

        typedef std::set<llvm::User *> UserSet_t;
        static UserSet_t getRegisterValuesAfterCall(const uint64_t RegNo, const Instruction *Inst);

        static UserSet_t getRegisterValuesBeforeCall(const uint64_t RegNo, const Instruction *Inst, const bool GetStores = false);

        ParameterAccessPairSet_t getStackParameters(Function &F, std::set<uint64_t> SPIdx);

        virtual const char *getPassName() const {
            return "DetectParametersPass";
        }

        static void setSpecialPreSet(const Instruction *inst, uint64_t RegNo, UserSet_t set);

    private:
        static UserSet_t getRegisterValuesAfterCall(const uint64_t RegNo, const Instruction *Inst, std::set<const BasicBlock*> &visited);
        static UserSet_t getRegisterValuesBeforeCall(const uint64_t RegNo, const Instruction *Inst, std::set<const BasicBlock*> &visited, const bool GetStores = false);

        typedef std::vector<Instruction*> InstructionList_t;

        std::map<const Function *, std::unique_ptr<ParameterAccessPairSet_t>> StackOffsets;
        std::map<const Function *, std::unique_ptr<ParameterAccessPairSet_t>> RegisterIndexes;
        std::map<const Function *, std::unique_ptr<ParameterAccessPairSet_t>> ReturnIndexes;

        Function *CurrentFunction;

        bool isParameterRegister(uint64_t Idx);
        bool isStackRegister(uint64_t Idx);
        bool readBeforeWrite(Instruction *Inst, Instruction *&LoadInst);

        void getMemoryOperations(Instruction *Address, InstructionList_t &Load, InstructionList_t &Store);

        typedef std::map<uint64_t, UserSet_t> RegUserSet_t;
        typedef std::map<const Instruction*, RegUserSet_t> InstRegUserSet_t;

        static InstRegUserSet_t specialPreSets;

        static std::mutex passLock;
    };
}

#endif //LLVM_DETECTPARAMETERSPASS_H
