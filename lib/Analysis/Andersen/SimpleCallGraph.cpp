#include <llvm/IR/Instruction.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/Analysis/Andersen/SimpleCallGraph.h"
#include "llvm/Support/Debug.h"

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "simple-callgraph"

void SimpleCallGraph::addCallEdge(const Instruction *CallInst, std::string Target) {
    std::unique_lock<std::mutex> lock(graphLock);
    shared_ptr<FunctionSet_t> CalledFunctions = CallGraph[CallInst];
    if (!CalledFunctions) {
        CalledFunctions = shared_ptr<FunctionSet_t>(new FunctionSet_t());
        CallGraph[CallInst] = CalledFunctions;
    }

    shared_ptr<InstructionSet_t> Callers = ReverseCallGraph[Target];
    if (!Callers) {
        Callers = shared_ptr<InstructionSet_t>(new InstructionSet_t());
        ReverseCallGraph[Target] = Callers;
    }

    CalledFunctions->insert(Target);
    Callers->insert(CallInst);
}

bool SimpleCallGraph::containtsEdge(const Instruction *Inst, std::string F) {
    std::unique_lock<std::mutex> lock(graphLock);
    FunctionSet_t &Called = getCalled(Inst);
    return Called.find(F) != Called.end();
}

SimpleCallGraph::InstructionSet_t &SimpleCallGraph::getCallers(std::string F) {
    shared_ptr<InstructionSet_t> Callers = ReverseCallGraph[F];
    if (!Callers) {
        Callers = shared_ptr<InstructionSet_t>(new InstructionSet_t());
        ReverseCallGraph[F] = Callers;
    }
    return *Callers;
}

SimpleCallGraph::FunctionSet_t &SimpleCallGraph::getCalled(const Instruction *Inst) {
    shared_ptr<FunctionSet_t> CalledFunctions = CallGraph[Inst];
    if (!CalledFunctions) {
        CalledFunctions = shared_ptr<FunctionSet_t>(new FunctionSet_t());
        CallGraph[Inst] = CalledFunctions;
    }
    return *CalledFunctions;
}

void SimpleCallGraph::finalize() {
    FunctionSet_t External;

    for (Module::iterator F_it = M.begin(); F_it != M.end(); ++F_it) {
        if (F_it->isDeclaration() || F_it->isIntrinsic())
            continue;

        if (getCallers(F_it->getName()).size() == 0) {
            External.insert(F_it->getName());
        }
    }

//    FIXME: necessary???
    return;
    assert(!M.getFunction("_EXTERNAL_"));

    IRBuilder<> *Builder = new IRBuilder<>(getGlobalContext());

    Function *ExternalFunc = dyn_cast<Function>(M.getOrInsertFunction("_EXTERNAL_", AttributeSet(), Type::getVoidTy(getGlobalContext()), nullptr));
    assert(ExternalFunc);
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", ExternalFunc);
    Builder->SetInsertPoint(BB);
    Instruction *First = Builder->CreateCall(ExternalFunc, ArrayRef<Value*>());
    Instruction *Second = Builder->CreateCall(ExternalFunc, ArrayRef<Value*>());
    Builder->CreateRetVoid();
    delete(Builder);


    for (FunctionSet_t::iterator F_it = External.begin(); F_it != External.end(); ++F_it) {
//        getCalled(First).insert(*F_it);
//        getCallers(*F_it).insert(Second);
        if (*F_it == "_EXTERNAL_")
            continue;
        addCallEdge(First, *F_it);
//        addCallEdge(Second, *F_it);
    }
}

void SimpleCallGraph::print(raw_ostream &ostream) {
    std::set<std::string> Edges;

    ostream << "strict digraph g {\n";
    ostream << "overlap=false;\n";
    for (CallGraph_t::iterator C_it = CallGraph.begin(); C_it != CallGraph.end(); ++C_it) {
//        if (!(C_it->first->getParent()->getParent()->getName().startswith("-") || C_it->first->getParent()->getParent()->getName().startswith("+")))
//            continue;
        for (FunctionSet_t::iterator F_it = C_it->second->begin(); F_it != C_it->second->end(); ++F_it) {
//            if (!((*F_it)[0] == '+' || (*F_it)[0] == '-'))
//                continue;
            if ((*F_it).find("objc") == 0) {
                continue;
            }
//            ostream << "\"" << C_it->first->getParent()->getParent()->getName() << "\" -> \"" << *F_it << "\";\n";

            Edges.insert("\"" + C_it->first->getParent()->getParent()->getName().str() + "\" -> \"" + *F_it + "\";\n");
        }
    }

    for (std::set<std::string>::iterator E_it = Edges.begin(); E_it != Edges.end(); ++E_it) {
        ostream << *E_it;
    }

    ostream << "}\n";

    ostream.flush();
}

SimpleCallGraph::FunctionSet_t SimpleCallGraph::getCalled(const std::string &fun) {
    FunctionSet_t called;
    for (auto &call_it : CallGraph) {
        if (call_it.first->getParent()->getParent()->getName() == fun) {
            called.insert(call_it.second->begin(), call_it.second->end());
        }
    }
    return called;
}

bool SimpleCallGraph::hasPath(std::string &from, std::string &to) {
    std::unique_lock<std::mutex> lock(pathCacheLock);
    if (pathCache.find(FromTo_t(from, to)) != pathCache.end()) {
        return pathCache[FromTo_t(from, to)];
    }
    lock.unlock();
    FunctionSet_t worklist(getCalled(from));
    FunctionSet_t visited;
    while (worklist.size()) {
        FunctionSet_t tmp;
        for (auto &w : worklist) {
            if (visited.find(w) != visited.end())
                continue;
            visited.insert(w);
            if (w == to) {
                lock.lock();
                pathCache[FromTo_t(from, to)] = true;
                return true;
            }
            lock.lock();
            pathCache[FromTo_t(from, w)] = true;
            lock.unlock();
            FunctionSet_t called = getCalled(w);
            tmp.insert(called.begin(), called.end());
        }

        std::swap(tmp, worklist);
    }
    lock.lock();
    DEBUG(errs() << "No path from " << from << "to " << to << "\n");
    pathCache[FromTo_t(from, to)] = false;
    return false;
}
