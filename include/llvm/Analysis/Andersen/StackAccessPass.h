#ifndef LLVM_STACKACCESSPASS_H
#define LLVM_STACKACCESSPASS_H

#include "llvm/Pass.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"

#include <map>
#include <vector>
#include <set>
#include <deque>
#include <tuple>
#include <mutex>

namespace llvm {

  class StackAccessPass : public ModulePass {
  public:
    static char ID;

    StackAccessPass() : ModulePass(ID) {
    }

    typedef std::set<const Value *> ValueList_t;
    typedef std::set<int64_t> Int64List_t;
    typedef std::map<const Value *, std::shared_ptr<Int64List_t>> OffsetMap_t;
    typedef std::map<int64_t, std::shared_ptr<ValueList_t>> OffsetValueListMap_t;

    virtual bool runOnModule(Module &M);

    static void runOnFunction(Function &F, OffsetMap_t &OffsetMap, OffsetValueListMap_t &OffsetValueListMap,
                              std::set<uint64_t> SPIdx);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const;

    virtual const char *getPassName() const {
      return "StackAccessPass";
    }

    OffsetMap_t &getOffsets(const Function *F) {
      if (Offsets.find(F) == Offsets.end())
        Offsets[F] = std::shared_ptr<OffsetMap_t>(new OffsetMap_t());
      return *Offsets[F];
    };

    OffsetValueListMap_t &getOffsetValues(const Function *F) {
      if (ValuesForOffset.find(F) == ValuesForOffset.end())
        ValuesForOffset[F] = std::shared_ptr<OffsetValueListMap_t>(new OffsetValueListMap_t());
      return *ValuesForOffset[F];
    };

    /**
     * findStackPointer defines if the last store of the stackpointer should be used as starting point
     * or if false the passed instrution is used
     */

    /**
     * findStackPointer defines if the last store of the stackpointer should be used as starting point
     * or if false the passed instrution is used
     * @param  Inst             The call instruction for which the stack offset should be calculated
     * @param  findStackPointer
     * @return                  The stack offset
     */
    int64_t getStackPointerValue(const Instruction *Inst, bool findStackPointer = true);

  private:

    typedef std::map<const Function *, std::shared_ptr<OffsetMap_t>> FunctionOffsetMap_t;
    typedef std::map<const Function *, std::shared_ptr<OffsetValueListMap_t>> FunctionOffsetValueListMap_t;

    typedef std::pair<Instruction *, int64_t> InstructionOffsetPair_t;
    typedef std::tuple<Instruction *, int64_t, Instruction *> InstructionOffsetTuple_t;

    static bool isStackPointer(Value *Ptr, std::set<uint64_t> SPIdx);

    FunctionOffsetMap_t Offsets;
    FunctionOffsetValueListMap_t ValuesForOffset;

    Function *CurrentFunction;

    std::deque<InstructionOffsetTuple_t> Worklist;

    /**
     * Returns the instruction that points to the stack pointer element in the register set
     * @param  F The function of interest
     * @return   The pointer to the stack pointer register
     */
    const Instruction *getStackPointer(const Function *F);

    /**
     * @param  Inst     The initial instruction to start backtracking
     * @param  InstList A queue containing all instructions to backtrack
     * @param  SPIdx    A set containing the indexes of the registers in the register file
     * @return          All offset candidates at this point (Inst)
     */
    static std::set<int64_t> backtrackInstruction(const Instruction *Inst, std::deque<const Instruction *> &InstList, const std::set<uint64_t> SPIdx);
  };
}

#endif //LLVM_STACKACCESSPASS_H
