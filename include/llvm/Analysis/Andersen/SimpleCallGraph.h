#ifndef LLVM_SIMPLECALLGRAPH_H
#define LLVM_SIMPLECALLGRAPH_H

#include <set>
#include <map>
#include <mutex>
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/Instruction.h"

namespace llvm {
    class SimpleCallGraph {
    public:
        SimpleCallGraph(Module &M): M(M) {};
        typedef std::set<std::string> FunctionSet_t;
        typedef std::set<const Instruction*> InstructionSet_t;

        void addCallEdge(const Instruction *CallInst, std::string Target);

        InstructionSet_t &getCallers(std::string F);
        FunctionSet_t &getCalled(const Instruction *Inst);
        FunctionSet_t getCalled(const std::string &fun);
        bool containtsEdge(const Instruction *Inst, std::string F);

        void finalize();

        bool hasPath(std::string &from, std::string &to);

        void print(raw_ostream &ostream);
    private:
        typedef std::map<const Instruction*, std::shared_ptr<FunctionSet_t>> CallGraph_t;
        typedef std::map<std::string, std::shared_ptr<InstructionSet_t>> ReverseCallGraph_t;

        CallGraph_t CallGraph;
        ReverseCallGraph_t ReverseCallGraph;

        Module &M;
        std::mutex graphLock;

        typedef std::pair<std::string, std::string> FromTo_t;
        typedef std::map<FromTo_t, bool> PathCache_t;
        std::mutex pathCacheLock;
        PathCache_t pathCache;
    };
}

#endif //LLVM_SIMPLECALLGRAPH_H
