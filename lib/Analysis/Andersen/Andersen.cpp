#include <llvm/IR/Dominators.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Object/MachO.h>
#include <llvm/Analysis/Andersen/ObjectiveCBinary.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/Support/Debug.h>
#include <llvm/IR/PatternMatch.h>
#include "llvm/Analysis/Andersen/Andersen.h"

#include "llvm/ADT/Statistic.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Analysis/Andersen/StackAccessPass.h"
#include "llvm/Analysis/Andersen/DetectParametersPass.h"

#include "llvm/IR/Instructions.h"

using namespace llvm;

cl::opt<bool> DumpDebugInfo("dump-debug", cl::desc("Dump debug info into stderr"), cl::init(false), cl::Hidden);
cl::opt<bool> DumpResultInfo("dump-result", cl::desc("Dump result info into stderr"), cl::init(false), cl::Hidden);
cl::opt<bool> DumpConstraintInfo("dump-cons", cl::desc("Dump constraint info into stderr"), cl::init(false),
                                 cl::Hidden);

cl::opt<std::string> BinaryFile("binary", cl::desc(""), cl::init(""), cl::Hidden);

cl::opt<std::string> UnhandledFile("unhandled", cl::desc(""), cl::init(""), cl::Hidden);

Andersen::Andersen() : llvm::ModulePass(ID) {
}


void Andersen::getAnalysisUsage(AnalysisUsage &AU) const {
  AU.setPreservesAll();
//	AU.addRequired<DataLayoutPass>();
  AU.addRequired<StackAccessPass>();
  AU.addRequired<DetectParametersPass>();
  AU.addRequired<DominatorTreeWrapperPass>();
  AU.addRequired<LoopInfoWrapperPass>();
}

void Andersen::getAllAllocationSites(std::vector<const llvm::Value *> &allocSites) const {
  nodeFactory.getAllocSites(allocSites);
}

bool Andersen::getPointsToSet(const llvm::Value *v, std::vector<const llvm::Value *> &ptsSet) const {
  NodeIndex ptrIndex = nodeFactory.getValueNodeFor(v);
  if (ptrIndex == AndersNodeFactory::InvalidIndex) {
    ptrIndex = nodeFactory.getObjectNodeFor(v);
  }
  // We have no idea what v is...
  if (ptrIndex == AndersNodeFactory::InvalidIndex || ptrIndex == nodeFactory.getUniversalPtrNode())
    return false;

  NodeIndex ptrTgt = nodeFactory.getMergeTarget(ptrIndex);
  ptsSet.clear();

  auto ptsItr = ptsGraph.find(ptrTgt);
  if (ptsItr == ptsGraph.end()) {
    // Can't find ptrTgt. The reason might be that ptrTgt is an undefined pointer. Dereferencing it is undefined behavior anyway, so we might just want to treat it as a nullptr pointer
    return true;
  }
  for (auto v: ptsItr->second) {
    if (v == nodeFactory.getNullObjectNode())
      continue;

    const llvm::Value *val = nodeFactory.getValueForNode(v);
    if (val != nullptr)
      ptsSet.push_back(val);
  }
  return true;
}

bool Andersen::runOnModule(Module &M) {
  Mod = &M;
  CallGraph = std::unique_ptr<SimpleCallGraph>(new SimpleCallGraph(M));
//	dataLayout = &(getAnalysis<DataLayoutPass>().getDataLayout());

  if (!BinaryFile.length())
    llvm_unreachable("Binary file needs to be specified");
  this->MachO = std::unique_ptr<ObjectiveCBinary>(new ObjectiveCBinary(BinaryFile));

  if (!UnhandledFile.length())
    unhandledFunctions = &nulls();
  else {
    std::error_code EC;

    unhandledFunctions = new raw_fd_ostream(UnhandledFile, EC, sys::fs::F_None);

    if (EC) {
      errs() << EC.message() << '\n';
      unhandledFunctions = &nulls();
    }
  }

  nodeFactory.setDataLayout(dataLayout);

  collectConstraints(M);

  uint64_t NumConstraints = constraints.size();

  for (auto &fun : M) {
    if (ObjectiveC::CallHandlerBase::isObjectiveCMethod(fun.getName())) {
      for (auto &i : fun.getEntryBlock()) {
        if (i.getOpcode() != Instruction::Load)
          continue;
        const GetElementPtrInst *getElementPtrInst = dyn_cast<GetElementPtrInst>(i.getOperand(0));
        if (!getElementPtrInst)
          continue;
        const ConstantInt *idx = dyn_cast<const ConstantInt>(getElementPtrInst->getOperand(2));
        if (!idx)
          continue;
        if (idx->getZExtValue() != 5)
          continue;
        StringRef typeName = ObjectiveC::CallHandlerBase::getClassname(fun.getName());
        NodeIndex valNode = nodeFactory.getValueNodeFor(&i);
        if (valNode == AndersNodeFactory::InvalidIndex)
          valNode = nodeFactory.createValueNode(&i);
        NodeIndex objNode = nodeFactory.createObjectNode(&i);
        if (objNode == AndersNodeFactory::InvalidIndex)
          objNode = nodeFactory.createObjectNode(&i);
        addConstraint(AndersConstraint::ADDR_OF, valNode, objNode);
        setType((Value *) &i, typeName);
        break;
      }
    }
    for (auto &bb : fun) {
      for (auto &i : bb) {
        if (i.getOpcode() == Instruction::Load) {

          Instruction *sext = nullptr;
          if (PatternMatch::match(i.getOperand(0), PatternMatch::m_IntToPtr(
            PatternMatch::m_BinOp(PatternMatch::m_Value(), PatternMatch::m_Instruction(sext))))) {
            if (sext->getOpcode() != Instruction::SExt)
              continue;
            if (const LoadInst *loadInst = dyn_cast<const LoadInst>(sext->getOperand(0))) {
              ConstantInt *constantInt = nullptr;
              if (PatternMatch::match(loadInst->getOperand(0),
                                      PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(constantInt)))) {

                std::map<uint64_t, ObjectiveC::IVAR>::iterator ivar_it = getMachO().getIVARs().find(
                  constantInt->getZExtValue());
                if (ivar_it == getMachO().getIVARs().end()) {
                  continue;
                }

                if (ivar_it->second.getType().size() == 0) {
                  continue;
                }

                bool foundType = false;
                std::vector<const Value *> ptsTo;
                getPointsToSet(&i, ptsTo);

                for (auto &p : ptsTo) {
                  StringSet_t types;
                  if (getType((Value *) p, types)) {
                    for (auto &t : types) {
                      if (t == ivar_it->second.getType()) {
                        foundType = true;
                        break;
                      }
                    }
                  }
                }

                if (!foundType) {
                  NodeIndex objIndex = nodeFactory.getObjectNodeFor(&i);
                  if (objIndex == AndersNodeFactory::InvalidIndex) {
                    objIndex = nodeFactory.createObjectNode(&i);
                  }
                  NodeIndex valIndex = nodeFactory.getValueNodeFor(&i);
                  if (valIndex == AndersNodeFactory::InvalidIndex) {
                    valIndex = nodeFactory.createValueNode(&i);
                  }
                  addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                  setType(&i, ivar_it->second.getType());
                }

              }
            }
          }
        } else if (i.getOpcode() == Instruction::Call) {
          const CallInst *call = (const CallInst *) &i;
          if (call->getCalledFunction() && call->getCalledFunction()->hasName() &&
              call->getCalledFunction()->getName() == "objc_loadWeakRetained") {
            DetectParametersPass::UserSet_t post_X0s = DetectParametersPass::getRegisterValuesAfterCall(5, call);
            DetectParametersPass::UserSet_t pre_X0s = DetectParametersPass::getRegisterValuesBeforeCall(5, call);

            for (auto &pre_x0 : pre_X0s) {
              Instruction *loadInst = nullptr;
              ConstantInt *constAddr = nullptr;
              if (PatternMatch::match(pre_x0, PatternMatch::m_BinOp(PatternMatch::m_Value(),
                                                                    PatternMatch::m_SExt(PatternMatch::m_Instruction(
                                                                      loadInst)))) &&
                  loadInst->getOpcode() == Instruction::Load &&
                  PatternMatch::match(loadInst->getOperand(0),
                                      PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(constAddr)))) {

                std::map<uint64_t, ObjectiveC::IVAR>::iterator ivar_it = getMachO().getIVARs().find(
                  constAddr->getZExtValue());
                if (ivar_it == getMachO().getIVARs().end()) {
                  continue;
                }

                if (ivar_it->second.getType().size() == 0) {
                  continue;
                }

                for (auto &post_x0 : post_X0s) {
                  bool foundType = false;
                  std::vector<const Value *> ptsTo;
                  getPointsToSet(post_x0, ptsTo);

                  for (auto &p : ptsTo) {
                    StringSet_t types;
                    if (getType((Value *) p, types)) {
                      for (auto &t : types) {
                        if (t == ivar_it->second.getType()) {
                          foundType = true;
                          break;
                        }
                      }
                    }
                  }

                  if (!foundType) {
                    NodeIndex objIndex = nodeFactory.getObjectNodeFor(post_x0);
                    if (objIndex == AndersNodeFactory::InvalidIndex) {
                      objIndex = nodeFactory.createObjectNode(post_x0);
                    }
                    NodeIndex valIndex = nodeFactory.getValueNodeFor(post_x0);
                    if (valIndex == AndersNodeFactory::InvalidIndex) {
                      valIndex = nodeFactory.createValueNode(post_x0);
                    }
                    addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                    setType(post_x0, ivar_it->second.getType());
                  }
                }
              }
            }

          }
        }
      }
    }
  }
  do {

    errs() << "Optimize and solve constraints\n";


    optimizeConstraints();


    solveConstraints();


    StackAccessPass *SAP = getAnalysisIfAvailable<StackAccessPass>();
    if (!SAP)
      SAP = &getAnalysis<StackAccessPass>();

    stackOffsetMap.clear();

    for (Function &f : M) {
      if (f.isDeclaration() || f.isIntrinsic())
        continue;

      StackAccessPass::OffsetMap_t &Offsets = SAP->getOffsets(&f);

      StackAccessPass::OffsetMap_t::iterator end = Offsets.end();

      for (inst_iterator I_it = inst_begin(f); I_it != inst_end(f); ++I_it) {
        const Instruction *I = &*I_it;
        if (Offsets.find(I) == end)
          continue;
        if (!Offsets[I])
          continue;
        StackAccessPass::Int64List_t &OffsetList = *Offsets[I];

        std::vector<const Value *> ptsTo;
        getPointsToSet(I, ptsTo);
        for (auto &ptsTo_it : ptsTo) {
          for (int64_t O : OffsetList) {
            stackOffsetMap[ptsTo_it].insert(std::pair<const Function *, int64_t>(&f, O));
          }
        }
      }
    }


    std::deque<Instruction *> CallInsts = CallInstWorklist;
    CallInstWorklist.clear();

    std::deque<Function *> Functions = FunctionWorklist;
    FunctionWorklist.clear();

    errs() << "Add function call constraints\n";
    while (CallInsts.size()) {
      Instruction *i = CallInsts.front();
      CallInsts.pop_front();

      ImmutableCallSite cs(i);
      addConstraintForCall(cs);
    }
    std::sort(constraints.begin(), constraints.end());
    constraints.erase(std::unique(constraints.begin(), constraints.end()), constraints.end());

    errs() << constraints.size() << " constraints\n";

    if (constraints.size() == NumConstraints) {
      errs() << "NO NEW CONSTRAINTS!!!\n";
      break;
    }
    NumConstraints = constraints.size();

  } while (CallInstWorklist.size() || FunctionWorklist.size());

  if (DumpDebugInfo) {
    errs() << "Unoptimized constraints\n";
    dumpConstraintsPlainVanilla();
  }

  if (DumpConstraintInfo) {
    errs() << "Optimized constraints\n";
    dumpConstraints();
  }

  if (DumpDebugInfo) {
    errs() << "\n";
    errs() << "Solved constraints\n";
    dumpPtsGraphPlainVanilla();
  }

  if (DumpResultInfo) {
    nodeFactory.dumpNodeInfo();
    errs() << "\n";
    errs() << "Results\n";
    dumpPtsGraphPlainVanilla();
  }

//    CallGraph->finalize();

  DEBUG_WITH_TYPE("simple-callgraph",
                  CallGraph->print(errs());
  );
//    assert(false);


  unhandledFunctions->flush();

  if (UnhandledFile.length())
    delete (unhandledFunctions);


  constraints.clear();


  return false;
}

void Andersen::releaseMemory() {
}

void Andersen::dumpConstraint(const AndersConstraint &item) const {
  NodeIndex dest = item.getDest();
  NodeIndex src = item.getSrc();

  switch (item.getType()) {
    case AndersConstraint::COPY: {
      nodeFactory.dumpNode(dest);
      errs() << " = ";
      nodeFactory.dumpNode(src);
      break;
    }
    case AndersConstraint::LOAD: {
      nodeFactory.dumpNode(dest);
      errs() << " = *";
      nodeFactory.dumpNode(src);
      break;
    }
    case AndersConstraint::STORE: {
      errs() << "*";
      nodeFactory.dumpNode(dest);
      errs() << " = ";
      nodeFactory.dumpNode(src);
      break;
    }
    case AndersConstraint::ADDR_OF: {
      nodeFactory.dumpNode(dest);
      errs() << " = &";
      nodeFactory.dumpNode(src);
    }
  }

  errs() << "\n";
}

void Andersen::dumpConstraints() const {
  errs() << "\n----- Constraints -----\n";
  for (auto const &item: constraints)
    dumpConstraint(item);
  errs() << "----- End of Print -----\n";
}

void Andersen::dumpConstraintsPlainVanilla() const {
  for (auto const &item: constraints) {
    errs() << item.getType() << " " << item.getDest() << " " << item.getSrc() << " 0\n";
  }
}

void Andersen::dumpPtsGraphPlainVanilla() const {
  for (unsigned i = 0, e = nodeFactory.getNumNodes(); i < e; ++i) {
    NodeIndex rep = nodeFactory.getMergeTarget(i);
    auto ptsItr = ptsGraph.find(rep);
    if (ptsItr != ptsGraph.end()) {
      errs() << i << " ";
      for (auto v: ptsItr->second)
        errs() << v << " ";
      errs() << "\n";
    }
  }
}

void Andersen::setType(const llvm::Value *V, llvm::StringRef Typename) {
  if (!Typename.size())
    return;
  typeLock.lock();
  assert(V && Typename.size());
  V = (Value *) nodeFactory.getAbstractLocation(V);
  ObjectTypes[V].insert(Typename.str());
  typeLock.unlock();
}

bool Andersen::getType(const llvm::Value *V, StringSet_t &Typename) {
  std::map<const Value *, StringSet_t>::iterator O_it = ObjectTypes.find(V);
  if (O_it == ObjectTypes.end())
    return false;
  Typename = O_it->second;
  return true;
}

char Andersen::ID = 0;

static RegisterPass<Andersen> X("anders", "Andersen's inclusion-based points-to analysis", true, true);
