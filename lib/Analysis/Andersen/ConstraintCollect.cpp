#include <llvm/IR/CFG.h>
#include <llvm/Analysis/Andersen/StackAccessPass.h>
#include <llvm/IR/Dominators.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/StringExtras.h>
#include <thread>
#include <future>
#include <utility>
#include <iostream>
#include <vector>
#include <type_traits>
#include "llvm/Analysis/Andersen/Andersen.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/PatternMatch.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/Analysis/Andersen/ObjCCallHandler.h"
#include "../../LLVMSlicer/Languages/LLVM.h"

#define DECOMPILED
#define DEBUG_TYPE "constraint_collect"


#define CREATE_OBJECT_CONSTRAINT(instruction, type) \
    NodeIndex valIdx = nodeFactory.getValueNodeFor(instruction);\
    if (valIdx == AndersNodeFactory::InvalidIndex)\
        valIdx = nodeFactory.createValueNode(instruction);\
    NodeIndex objIdx = nodeFactory.getObjectNodeFor(instruction);\
    if (objIdx == AndersNodeFactory::InvalidIndex)\
        objIdx = nodeFactory.createObjectNode(instruction);\
    addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);\
    setType(instruction, type);\
//    addDummyHelper(nodeFactory.getAbstractLocation(instruction));

#define FIND_REG_ANDSET(fun, regNo, type)\
    for (auto &i : fun->getEntryBlock()) {\
        if (i.getOpcode() == Instruction::Load) {\
            if (GetElementPtrInst *getElem = dyn_cast<GetElementPtrInst>(i.getOperand(0))) {\
                if (ConstantInt *idx = dyn_cast<ConstantInt>(getElem->getOperand(2))) {\
                    if (idx->getZExtValue() == regNo) {\
                        CREATE_OBJECT_CONSTRAINT(&i, type)\
                        break;\
                    }\
                }\
            }\
        }\
    }

#define SET_PARAM_TYPE(meta, c, m, regNo, type) \
{\
    char buffer[512];\
    sprintf(buffer, "%s[%s %s]", (meta ? "+" : "-"), c, m);\
    Function *_f = Mod->getFunction(StringRef(buffer));\
    if (_f) {\
        FIND_REG_ANDSET(_f, regNo, type)\
    }\
}

#define PROTOCOL_METHOD(meta, c, m, p, regNo, type) \
if (protocolName == p) {\
SET_PARAM_TYPE(meta, c, m, regNo, type)\
}

using namespace llvm;

void Andersen::addProtocolConstraints(std::string className, std::string protocolName) {
//    PROTOCOL_METHOD(false, className.data(), "application:didFinishLaunchingWithOptions:", "UIApplicationDelegate", 7, "NSabc")
//    PROTOCOL_METHOD(false, className.data(), "textFieldDidEndEditing:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textFieldShouldBeginEditing:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textFieldDidBeginEditing:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textFieldShouldEndEditing:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textField:shouldChangeCharactersInRange:replacementString:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textField:shouldChangeCharactersInRange:replacementString:", "UITextFieldDelegate", 9, "NSString")
//    PROTOCOL_METHOD(false, className.data(), "textFieldShouldClear:", "UITextFieldDelegate", 7, "UITextField")
//    PROTOCOL_METHOD(false, className.data(), "textFieldShouldReturn:", "UITextFieldDelegate", 7, "UITextField")

  ObjectiveCBinary::ProtocolMap_t::iterator prot_it = MachO->getProtocols().find(protocolName);
  if (prot_it == MachO->getProtocols().end())
    return;

  for (auto &m : prot_it->second.getInstanceMethods()) {
    for (auto &r : m.getRegTypes()) {
      PROTOCOL_METHOD(false, className.data(), m.getMethodname().data(), protocolName.data(), r.first, r.second.data());
    }
  }
}



// CollectConstraints - This stage scans the program, adding a constraint to the Constraints list for each instruction in the program that induces a constraint, and setting up the initial points-to graph.

void Andersen::collectConstraints(Module &M) {
  errs() << "Collect constraints\n";
  // First, the universal ptr points to universal obj, and the universal obj points to itself
  addConstraint(AndersConstraint::ADDR_OF,
                nodeFactory.getUniversalPtrNode(), nodeFactory.getUniversalObjNode());
  addConstraint(AndersConstraint::STORE,
                nodeFactory.getUniversalObjNode(), nodeFactory.getUniversalObjNode());

  // Next, the null pointer points to the null object.
  addConstraint(AndersConstraint::ADDR_OF,
                nodeFactory.getNullPtrNode(), nodeFactory.getNullObjectNode());

  // Next, add any constraints on global variables. Associate the address of the global object as pointing to the memory for the global: &G = <G memory>
  collectConstraintsForGlobals(M);

  for (auto &c : MachO->getClasses()) {
    if (!c.second || c.second->getType() != ObjectiveC::Initialized) {
      continue;
    }
    ObjectiveCBinary::ClassPtr_t ClassPtr = std::static_pointer_cast<ObjectiveC::Class>(c.second);
    for (auto &p : ClassPtr->getProtocolList()) {
      addProtocolConstraints(c.second->getClassName(), p);
    }
  }

  // Here is a notable points before we proceed:
  // For functions with non-local linkage type, theoretically we should not trust anything that get passed to it or get returned by it. However, precision will be seriously hurt if we do that because if we do not run a -internalize pass before the -anders pass, almost every function is marked external. We'll just assume that even external linkage will not ruin the analysis result first
  size_t handled = 0;
  const Function *currentFunction = nullptr;


  for (auto const &f: M) {
    currentFunction = &f;
    handled++;

    if (f.isDeclaration() || f.isIntrinsic())
      continue;

    if (f.getName() == "main_init_regset" ||
        f.getName() == "main_fini_regset" ||
        f.getName() == "main" ||
        f.getName() == "-[AppDelegate window]")
      continue;

    DEBUG(errs() << "Process function: \"" << f.getName() << "\"\n");

    // Scan the function body
    // A visitor pattern might help modularity, but it needs more boilerplate codes to set up, and it breaks down the main logic into pieces

    // First, create a value node for each instruction with pointer type. It is necessary to do the job here rather than on-the-fly because an instruction may refer to the value node definied before it (e.g. phi nodes)
    for (const_inst_iterator itr = inst_begin(f), ite = inst_end(f); itr != ite; ++itr) {
      auto inst = itr.getInstructionIterator();
      if (inst->getType()->isPointerTy())
        nodeFactory.createValueNode(inst);
    }

    // Now, collect constraint for each relevant instruction
    for (const_inst_iterator itr = inst_begin(f), ite = inst_end(f); itr != ite; ++itr) {
      auto inst = itr.getInstructionIterator();
      collectConstraintsForInstruction(inst);
    }
  }
}

void Andersen::collectConstraintsForGlobals(Module &M) {
  // Create a pointer and an object for each global variable
  for (auto const &globalVal: M.globals()) {
    NodeIndex gVal = nodeFactory.createValueNode(&globalVal);
    NodeIndex gObj = nodeFactory.createObjectNode(&globalVal);
    addConstraint(AndersConstraint::ADDR_OF, gVal, gObj);
  }

  // Functions and function pointers are also considered global
  for (auto const &f: M) {
    // If f is an addr-taken function, create a pointer and an object for it
    if (f.hasAddressTaken()) {
      NodeIndex fVal = nodeFactory.createValueNode(&f);
      NodeIndex fObj = nodeFactory.createObjectNode(&f);
      addConstraint(AndersConstraint::ADDR_OF, fVal, fObj);
    }

    if (f.isDeclaration() || f.isIntrinsic())
      continue;

    // Create return node
    if (f.getFunctionType()->getReturnType()->isPointerTy()) {
      nodeFactory.createReturnNode(&f);
    }

    // Create vararg node
    if (f.getFunctionType()->isVarArg())
      nodeFactory.createVarargNode(&f);

    // Add nodes for all formal arguments.
    for (Function::const_arg_iterator itr = f.arg_begin(), ite = f.arg_end(); itr != ite; ++itr) {
      if (isa<PointerType>(itr->getType()))
        nodeFactory.createValueNode(itr);
    }
  }

  // Init globals here since an initializer may refer to a global var/func below it
  for (auto const &globalVal: M.globals()) {
    NodeIndex gObj = nodeFactory.getObjectNodeFor(&globalVal);
    assert(gObj != AndersNodeFactory::InvalidIndex && "Cannot find global object!");

    if (globalVal.hasDefinitiveInitializer()) {
      addGlobalInitializerConstraints(gObj, globalVal.getInitializer());
    }
    else {
      // If it doesn't have an initializer (i.e. it's defined in another translation unit), it points to the universal set.
      addConstraint(AndersConstraint::COPY,
                    gObj, nodeFactory.getUniversalObjNode());
    }
  }
}

void Andersen::addGlobalInitializerConstraints(NodeIndex objNode, const Constant *c) {
  //errs() << "Called with node# = " << objNode << ", initializer = " << *c << "\n";
  if (c->getType()->isSingleValueType()) {
    if (isa<PointerType>(c->getType())) {
      NodeIndex rhsNode = nodeFactory.getObjectNodeForConstant(c);
      assert(rhsNode != AndersNodeFactory::InvalidIndex && "rhs node not found");
      addConstraint(AndersConstraint::ADDR_OF, objNode, rhsNode);
    }
  }
  else if (c->isNullValue()) {
    addConstraint(AndersConstraint::COPY, objNode, nodeFactory.getNullObjectNode());
  }
  else if (!isa<UndefValue>(c)) {
    // Since we are doing field-insensitive analysis, all objects in the array/struct are pointed-to by the 1st-field pointer
    assert(isa<ConstantArray>(c) || isa<ConstantDataSequential>(c) || isa<ConstantStruct>(c));

    for (unsigned i = 0, e = c->getNumOperands(); i != e; ++i)
      addGlobalInitializerConstraints(objNode, cast<Constant>(c->getOperand(i)));
  }
}

void Andersen::collectConstraintsForInstruction(const Instruction *inst) {
  switch (inst->getOpcode()) {
    case Instruction::Alloca: {
      NodeIndex valNode = nodeFactory.getValueNodeFor(inst);
      assert(valNode != AndersNodeFactory::InvalidIndex && "Failed to find alloca value node");
      NodeIndex objNode = nodeFactory.createObjectNode(inst);
      addConstraint(AndersConstraint::ADDR_OF, valNode, objNode);
      break;
    }
    case Instruction::Call:
    case Instruction::Invoke: {
//			ImmutableCallSite cs(inst);
//			assert(cs && "Something wrong with callsite?");

//			addConstraintForCall(cs);

      addToWorklist((Instruction *) inst);

      break;
    }
    case Instruction::Ret: {
      if (inst->getNumOperands() > 0 && inst->getOperand(0)->getType()->isPointerTy()) {
        NodeIndex retIndex = nodeFactory.getReturnNodeFor(inst->getParent()->getParent());
        assert(retIndex != AndersNodeFactory::InvalidIndex && "Failed to find return node");
        NodeIndex valIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
        assert(valIndex != AndersNodeFactory::InvalidIndex && "Failed to find return value node");
        addConstraint(AndersConstraint::COPY, retIndex, valIndex);
      }
      break;
    }
    case Instruction::Load: {
      if (inst->getName() == "X8_5901") {
        assert(true);
      }
      if (dyn_cast<GetElementPtrInst>(inst->getOperand(0))) {
        break;
      }

      if (inst->getType()->isPointerTy() && false) {
        NodeIndex opIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
        assert(opIndex != AndersNodeFactory::InvalidIndex && "Failed to find load operand node");
        NodeIndex valIndex = nodeFactory.getValueNodeFor(inst);
        assert(valIndex != AndersNodeFactory::InvalidIndex && "Failed to find load value node");
        addConstraint(AndersConstraint::LOAD, valIndex, opIndex);
      } else {

        std::vector<const Value *> operands;

        if (PHINode *phi = dyn_cast<PHINode>(inst->getOperand(0))) {
          for (unsigned i = 0; i < phi->getNumIncomingValues(); ++i) {
            operands.push_back(phi->getIncomingValue(i));
          }
        } else if (SelectInst *selct = dyn_cast<SelectInst>(inst->getOperand(0))) {
          operands.push_back(selct->getOperand(1));
          operands.push_back(selct->getOperand(2));
        } else {
          operands.push_back(inst->getOperand(0));
        }


        for (unsigned i = 0; i < operands.size(); ++i) {
          const Value *op = operands[i];
          //Null pointer does not need to be handled
          if (dyn_cast<const ConstantPointerNull>(op)) {
            continue;
          }

          Value *Base = nullptr;
          Instruction *IVARPtr = nullptr;
          if (PatternMatch::match(op, PatternMatch::m_IntToPtr(PatternMatch::m_BinOp(PatternMatch::m_Value(Base),
                                                                                     PatternMatch::m_SExt(
                                                                                       PatternMatch::m_Instruction(
                                                                                         IVARPtr)))))) {

            std::vector<const Instruction *> ivarOps;
            ivarOps.push_back(IVARPtr);

            std::set<const Value *> visited;
            for (unsigned opNum = 0; opNum < ivarOps.size(); ++opNum) {
              if (visited.find(ivarOps[opNum]) != visited.end())
                continue;
              visited.insert(ivarOps[opNum]);
              if (const PHINode *phi = dyn_cast<const PHINode>(ivarOps[opNum])) {
                for (unsigned i = 0; i < phi->getNumIncomingValues(); ++i) {
                  const Instruction *in = dyn_cast<const Instruction>(phi->getIncomingValue(i));
                  if (!in)
                    continue;
                  ivarOps.push_back(in);
                }
                continue;
              }

              if (!isa<ConstantInt>(Base) && ivarOps[opNum]->getOpcode() == Instruction::Load) {
                NodeIndex srcIndex = nodeFactory.getValueNodeFor(ivarOps[opNum]);
                if (srcIndex == AndersNodeFactory::InvalidIndex)
                  srcIndex = nodeFactory.createValueNode(ivarOps[opNum]);
                assert(srcIndex != AndersNodeFactory::InvalidIndex);
                NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
                if (dstIndex == AndersNodeFactory::InvalidIndex) {
                  dstIndex = nodeFactory.createValueNode(inst);
                }
                addConstraint(AndersConstraint::LOAD, dstIndex, srcIndex);
              }
            }

            continue;
          }


          NodeIndex valIndex = nodeFactory.getValueNodeFor(inst);
          if (valIndex == AndersNodeFactory::InvalidIndex) {
            valIndex = nodeFactory.createValueNode(inst);
//                    addConstraint(AndersConstraint::ADDR_OF, valIndex, nodeFactory.createObjectNode(inst));
          }
          NodeIndex opIndex = nodeFactory.getValueNodeFor(((Instruction *) op)->getOperand(0));
          if (opIndex == AndersNodeFactory::InvalidIndex) {
            //FIXME
            opIndex = nodeFactory.createValueNode(((Instruction *) op)->getOperand(0));
          }
          assert(opIndex != AndersNodeFactory::InvalidIndex && "Failed to find load operand node");

//
          assert(valIndex != AndersNodeFactory::InvalidIndex && "Failed to find load value node");

          ConstantInt *Const;
          if (PatternMatch::match(op, PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(Const)))) {

            addConstraintsForConstIntToPtr((Instruction *) op, Const);
            NodeIndex idx2 = nodeFactory.getValueNodeFor(op);


            uint64_t content = MachO->getRAWData<uint64_t>(Const->getZExtValue());
            if (MachO->isCFString(content)) {

              Value *stringAddress = ConstantInt::get(IntegerType::get(getGlobalContext(), 64), content);
              NodeIndex stringValIdx = nodeFactory.getValueNodeFor(stringAddress);
              if (stringValIdx == AndersNodeFactory::InvalidIndex)
                stringValIdx = nodeFactory.createValueNode(stringAddress);
              NodeIndex stringObjIdx = nodeFactory.getObjectNodeFor(stringAddress);
              if (stringObjIdx == AndersNodeFactory::InvalidIndex) {
                stringObjIdx = nodeFactory.createObjectNodeDummy(stringAddress,
                                                                 *(Module *) inst->getParent()->getParent()->getParent());
//                                stringObjIdx = nodeFactory.createObjectNode(stringAddress);
              }
              addConstraint(AndersConstraint::ADDR_OF, valIndex, stringObjIdx);
//                            addConstraint(AndersConstraint::STORE, idx2, stringValIdx);
//                            addConstraint(AndersConstraint::LOAD, valIndex, idx2);
              setType(stringAddress, "NSString");
              continue;
            }

            std::string sectionName = MachO->getSectionName(Const->getZExtValue());
            //"bss" section?
            if (sectionName == "" || sectionName == "__bss" || sectionName == "__common") {
              addConstraint(AndersConstraint::LOAD, valIndex, idx2);
            } else {
              addConstraint(AndersConstraint::COPY, valIndex, idx2);
            }
            continue;

          }

//                if (!PatternMatch::match(inst->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_Value()))) {
          addConstraint(AndersConstraint::LOAD, valIndex, opIndex);
//                    break;
//                }
        }


      }
      break;
    }
    case Instruction::Store: {
      if (inst->getOperand(0)->getName() == "X8_5901") {
        assert(true);
      }
      if (dyn_cast<GetElementPtrInst>(inst->getOperand(1))) {
        if (dyn_cast<ConstantInt>(inst->getOperand(0))) {
          //Do nothing
        } else {

          //Check if the (maybe) stored parameter is a stack address that does not get accessed
          //in this function (this happens if it is only used as parameter)
          StackAccessPass &SAP = getAnalysis<StackAccessPass>();
          Function *f = (Function *) inst->getParent()->getParent();

          StackAccessPass::OffsetMap_t &Offsets = SAP.getOffsets(f);
          if (Offsets.find(inst->getOperand(0)) != Offsets.end()) {
            if (handledAliases.find(inst->getOperand(0)) == handledAliases.end()) {
              NodeIndex valIdx = nodeFactory.getValueNodeFor(inst->getOperand(0));
              if (valIdx == AndersNodeFactory::InvalidIndex)
                valIdx = nodeFactory.createValueNode(inst->getOperand(0));
              NodeIndex objIdx = nodeFactory.getObjectNodeFor(inst->getOperand(0));
              if (objIdx == AndersNodeFactory::InvalidIndex)
                objIdx = nodeFactory.createObjectNodeDummy(inst->getOperand(0), *Mod);
              addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
              findAliases(inst->getOperand(0), true);
              handledAliases.insert(inst->getOperand(0));
            }
          }

          break;
        }
      }

      ConstantInt *constDst = NULL;
      if (PatternMatch::match(inst->getOperand(1), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(constDst)))) {
        addConstraintsForConstIntToPtr(inst->getOperand(1), constDst);
//                NodeIndex addrValIndex = nodeFactory.getValueNodeFor(constDst);
//                if (addrValIndex == AndersNodeFactory::InvalidIndex)
//                    addrValIndex = nodeFactory.createValueNode(constDst);
//                NodeIndex objIndex = nodeFactory.getObjectNodeFor(constDst);
//                if (objIndex == AndersNodeFactory::InvalidIndex)
//                    objIndex = nodeFactory.createObjectNodeDummy(constDst, *(Module*)inst->getParent()->getParent()->getParent());
//
//                NodeIndex ptrValIndex = nodeFactory.getValueNodeFor(inst->getOperand(1));
//                if (ptrValIndex == AndersNodeFactory::InvalidIndex)
//                    ptrValIndex = nodeFactory.createValueNode(inst->getOperand(1));
//
//                addConstraint(AndersConstraint::ADDR_OF, addrValIndex, objIndex);
//                addConstraint(AndersConstraint::COPY, ptrValIndex, addrValIndex);
      }
      if (inst->getOperand(0)->getType()->isPointerTy()) {
        NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
        assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find store src node");
        NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst->getOperand(1));
        assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find store dst node");
        addConstraint(AndersConstraint::STORE, dstIndex, srcIndex);
      } else {
//
//                if (isa<ConstantInt>(inst->getOperand(0))) {
//                    break;
//                }
//                inst->dump();
//                NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
//                if (srcIndex == AndersNodeFactory::InvalidIndex) {
//                    std::set<const BasicBlock*> V;
//                    Instruction *x = nullptr;
//                    if (findPointer(inst->getOperand(0), inst, V, &x)) {
//                        srcIndex = nodeFactory.createValueNode(x);
//                        addConstraint(AndersConstraint::ADDR_OF, srcIndex, nodeFactory.createObjectNode(inst->getOperand(0)));
//                    } else {
//                        srcIndex = nodeFactory.createValueNode(inst->getOperand(0));
//                        addConstraint(AndersConstraint::ADDR_OF, srcIndex, nodeFactory.createObjectNode(inst->getOperand(0)));
//                    }
//                }
//                assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find store src node");
//                NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst->getOperand(1));
//
//                assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find store dst node");
//                addConstraint(AndersConstraint::STORE, dstIndex, srcIndex);
//                errs() << "STORE: " << srcIndex << "->" << dstIndex << "\n";

        //Addresses should obviously only be integer values...
        if (isa<ConstantFP>(inst->getOperand(0))) {
          break;
        }

        //Assume all constant integers are pointers
        if (isa<ConstantInt>(inst->getOperand(0)) && inst->getOperand(0)->getType()->getPrimitiveSizeInBits() <= 64) {
          ConstantInt *C = dyn_cast<ConstantInt>(inst->getOperand(0));
          //TODO:
          if (C->getZExtValue() >= 0x100000000) {
            if (C->getZExtValue() == 4295082136) {
              assert(true);
            }
            if (MachO->isSelectorRef(C->getZExtValue())) {
              std::string selName = MachO->getString(C->getZExtValue());
              ConstantDataArray *PointsToData = (ConstantDataArray *) ConstantDataArray::getString(getGlobalContext(),
                                                                                                   selName, false);
              NodeIndex dataObject = nodeFactory.getObjectNodeFor(PointsToData);
              if (dataObject == AndersNodeFactory::InvalidIndex) {
                dataObject = nodeFactory.createObjectNode(PointsToData);
              }

              NodeIndex dstIdx = nodeFactory.getValueNodeFor(inst->getOperand(0));
              if (dstIdx == AndersNodeFactory::InvalidIndex) {
                dstIdx = nodeFactory.createValueNode(inst->getOperand(0));
              }

              addConstraint(AndersConstraint::ADDR_OF, dstIdx, dataObject);
            } else {
              NodeIndex valIdx = nodeFactory.getValueNodeFor(inst->getOperand(0));
              if (valIdx == AndersNodeFactory::InvalidIndex) {
                valIdx = nodeFactory.createValueNode(inst->getOperand(0));
              }
              NodeIndex objIdx = nodeFactory.getObjectNodeFor(inst->getOperand(0));
              if (objIdx == AndersNodeFactory::InvalidIndex) {
                objIdx = nodeFactory.createObjectNodeDummy(inst->getOperand(0),
                                                           *(Module *) inst->getParent()->getParent()->getParent());
//                            objIdx = nodeFactory.createObjectNode(inst->getOperand(0));
              }
              addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
              if (MachO->isCFString(C->getZExtValue())) {
                //TODO: should we mark this one as const somehow?
                setType(inst->getOperand(0), "NSString");
              }
            }
          }
        }

        Value *Base = nullptr;
        Instruction *IVARPtr = nullptr;
        if (PatternMatch::match(inst->getOperand(1), PatternMatch::m_IntToPtr(
          PatternMatch::m_BinOp(PatternMatch::m_Value(Base),
                                PatternMatch::m_SExt(PatternMatch::m_Instruction(IVARPtr)))))) {
          if (!isa<ConstantInt>(Base) && IVARPtr->getOpcode() == Instruction::Load) {
            NodeIndex dstIndex = nodeFactory.getValueNodeFor(IVARPtr);
            if (dstIndex == AndersNodeFactory::InvalidIndex)
              dstIndex = nodeFactory.createValueNode(IVARPtr);
            assert(dstIndex != AndersNodeFactory::InvalidIndex);
            NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
            if (srcIndex == AndersNodeFactory::InvalidIndex) {
              srcIndex = nodeFactory.createValueNode(inst->getOperand(0));
            }
            addConstraint(AndersConstraint::STORE, dstIndex, srcIndex);

            NodeIndex iToPIdx = nodeFactory.getValueNodeFor(inst->getOperand(1));
            if (iToPIdx == AndersNodeFactory::InvalidIndex)
              iToPIdx = nodeFactory.createValueNode(inst->getOperand(1));
            addConstraint(AndersConstraint::COPY, iToPIdx, dstIndex);
          }
          break;
        }


        if (PatternMatch::match(inst->getOperand(1), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt()))) {
          //I don't think this is possible!?
          NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
          if (srcIndex == AndersNodeFactory::InvalidIndex) {
            srcIndex = nodeFactory.createValueNode(inst->getOperand(0));
          }
          NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst->getOperand(1));
          if (dstIndex == AndersNodeFactory::InvalidIndex) {
            dstIndex = nodeFactory.createValueNode(inst->getOperand(1));
          }
          addConstraint(AndersConstraint::STORE, dstIndex, srcIndex);
          break;
        }

        if (dyn_cast<GetElementPtrInst>(inst->getOperand(1)))
          break;

        NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
        if (srcIndex == AndersNodeFactory::InvalidIndex) {
          srcIndex = nodeFactory.createValueNode(inst->getOperand(0));
        }
        assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find store dst node");
        NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst->getOperand(1));
        assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find store dst node");

        addConstraint(AndersConstraint::STORE, dstIndex, srcIndex);
      }
      break;
    }
    case Instruction::GetElementPtr: {
      assert(inst->getType()->isPointerTy());
      /*
// P1 = getelementptr P2, ... --> <Copy/P1/P2>
NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find gep src node");
NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find gep dst node");

addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
      */
      break;
    }
    case Instruction::PHI: {
      if (inst->getType()->isPointerTy()) {
//                const PHINode *phiInst = cast<PHINode>(inst);
//                for (unsigned i = 0, e = phiInst->getNumIncomingValues(); i != e; ++i) {
//                    if (const Instruction *inInst = dyn_cast<const Instruction>(phiInst->getIncomingValue(i))) {
//                        if (inInst->getOpcode() != Instruction::Load) {
//                            assert(false);
//                        }
//                    } else {
//                        assert(false);
//                    }
//                }
        break;
      } else {
        const PHINode *phiInst = cast<PHINode>(inst);
        NodeIndex dstIndex = nodeFactory.getValueNodeFor(phiInst);
        if (dstIndex == AndersNodeFactory::InvalidIndex)
          dstIndex = nodeFactory.createValueNode(phiInst);
        for (unsigned i = 0, e = phiInst->getNumIncomingValues(); i != e; ++i) {
          if (isa<ConstantFP>(phiInst->getIncomingValue(i))) {
            continue;
          }
          NodeIndex srcIndex = nodeFactory.getValueNodeFor(phiInst->getIncomingValue(i));
          if (srcIndex == AndersNodeFactory::InvalidIndex) {
            srcIndex = nodeFactory.createValueNode(phiInst->getIncomingValue(i));
          }
          assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find phi src node");
          addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
        }
      }
//			if (inst->getType()->isPointerTy())
//			{
//				const PHINode* phiInst = cast<PHINode>(inst);
//				NodeIndex dstIndex = nodeFactory.getValueNodeFor(phiInst);
//				assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find phi dst node");
//				for (unsigned i = 0, e = phiInst->getNumIncomingValues(); i != e; ++i)
//				{
//					NodeIndex srcIndex = nodeFactory.getValueNodeFor(phiInst->getIncomingValue(i));
//                    if (srcIndex == AndersNodeFactory::InvalidIndex) {
//                        srcIndex = nodeFactory.createValueNode(phiInst->getIncomingValue(i));
//                    }
//					assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find phi src node");
//					addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
//				}
//			}
      break;
    }
    case Instruction::BitCast: {
      if (inst->getType()->isPointerTy()) {
        NodeIndex srcIndex = nodeFactory.getValueNodeFor(inst->getOperand(0));
        assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find bitcast src node");
        NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
        assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find bitcast dst node");
        addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
      }
      break;
    }
    case Instruction::IntToPtr: {
      assert(inst->getType()->isPointerTy());

      // Get the node index for dst
      NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
      assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find inttoptr dst node");

      // We use pattern matching to look for a matching ptrtoint
      Value *operand = inst->getOperand(0);

      // Pointer copy: Y = inttoptr (ptrtoint X)
      Value *srcValue = nullptr;
      if (PatternMatch::match(operand, PatternMatch::m_PtrToInt(PatternMatch::m_Value(srcValue)))) {
        NodeIndex srcIndex = nodeFactory.getValueNodeFor(srcValue);
        assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find inttoptr src node");
        addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
        break;
      }

      // Pointer arithmetic: Y = inttoptr (ptrtoint (X) + offset)
      if (PatternMatch::match(operand,
                              PatternMatch::m_Add(
                                PatternMatch::m_PtrToInt(
                                  PatternMatch::m_Value(srcValue)),
                                PatternMatch::m_Value()))) {
        NodeIndex srcIndex = nodeFactory.getValueNodeFor(srcValue);
        assert(srcIndex != AndersNodeFactory::InvalidIndex && "Failed to find inttoptr src node");
        addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
        break;
      }


      std::vector<const Value *> opList;
      opList.push_back(operand);

      for (unsigned i = 0; i < opList.size(); ++i) {
        if (const Instruction *I = dyn_cast<const Instruction>(opList[i])) {
          if (I->getOpcode() == Instruction::Select) {
            opList.push_back(I->getOperand(1));
            opList.push_back(I->getOperand(2));
            continue;
          }
        }

        const Value *op = opList[i];

        if (const ConstantInt *Const = dyn_cast<const ConstantInt>(op)) {
          addConstraintsForConstIntToPtr((Value *) inst, Const);
          continue;
        }

        //TODO: handle offsets stored in registers...
        uint64_t ConstantOffset = 0;
        if (PatternMatch::match((Value *) op, PatternMatch::m_Add(PatternMatch::m_Value(srcValue),
                                                                  PatternMatch::m_ConstantInt(ConstantOffset))) ||
            PatternMatch::match((Value *) op, PatternMatch::m_Add(PatternMatch::m_Value(srcValue), PatternMatch::m_SExt(
              PatternMatch::m_ConstantInt(ConstantOffset)))) ||
            (PatternMatch::match((Value *) op, PatternMatch::m_Value(srcValue)) &&
             !PatternMatch::match((Value *) op, PatternMatch::m_BinOp()))) {

//                NodeIndex valNode = nodeFactory.getValueNodeFor(inst);
//                assert(valNode != AndersNodeFactory::InvalidIndex);
//                NodeIndex objNode = nodeFactory.getObjectNodeFor(op);
//                if (objNode == AndersNodeFactory::InvalidIndex) {
//                    objNode = nodeFactory.createObjectNode(op);
//                }
//                addConstraint(AndersConstraint::ADDR_OF, valNode, objNode);
//                inst->dump();
//                findAliases(op);
//                break;
          //Check if this pointer value was loaded from memory and then load the pointers that were stored there
//                if (Instruction *SrcInst = dyn_cast<Instruction>(srcValue)) {
//                    if (SrcInst->getOpcode() == Instruction::Load) {
//                        //TODO: handle offsets that differ from zero
//                        if (ConstantOffset == 0) {
//                            NodeIndex srcIndex = nodeFactory.getValueNodeFor(SrcInst->getOperand(0));
//                            assert(srcIndex != AndersNodeFactory::InvalidIndex);
//                            addConstraint(AndersConstraint::LOAD, dstIndex, srcIndex);
//                        } else {
//                            //Overestimate...
//                            if (Instruction *SrcAddress = dyn_cast<Instruction>(SrcInst->getOperand(0))) {
//                                if (SrcAddress->isBinaryOp()) {
//                                    //Let's take the first operand as 'base'
//                                    Value *Base = SrcAddress->getOperand(0);
//                                    assert(false);
//                                }
//                            }
//                        }
//                        break;
//                    }
//                }
          NodeIndex srcIndex = nodeFactory.getValueNodeFor(op);
          if (srcIndex == AndersNodeFactory::InvalidIndex) {
            srcIndex = nodeFactory.createValueNode(op);

          }
          //FIXME: is this only needed if a new object is created?
          NodeIndex objNode = nodeFactory.getObjectNodeFor(op);
          if (objNode == AndersNodeFactory::InvalidIndex && handledAliases.find(op) == handledAliases.end()) {
//                        objNode = nodeFactory.createObjectNode(op);
//                    objNode = nodeFactory.createObjectNodeDummy(op, *Mod);
//                            objNode = nodeFactory.createObjectNode(op);
            objNode = nodeFactory.createObjectNodeDummy(op, getModule());
            findAliases(op);
          }
          if (handledAliases.find(op) == handledAliases.end()) {
            handledAliases.insert(op);
            addConstraint(AndersConstraint::ADDR_OF, srcIndex, objNode);
          }

          addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
          continue;
        } else {
          Value *Offset = NULL;
          if (PatternMatch::match(op, PatternMatch::m_BinOp(
            PatternMatch::m_BinOp(PatternMatch::m_Value(srcValue), PatternMatch::m_Value(Offset)),
            PatternMatch::m_Value()))) {
            Value *Base = ((Instruction *) op)->getOperand(0);

            NodeIndex srcIndex = nodeFactory.getValueNodeFor(Base);
            if (srcIndex == AndersNodeFactory::InvalidIndex) {
              srcIndex = nodeFactory.createValueNode(Base);
            }
            NodeIndex objNode = nodeFactory.getObjectNodeFor(Base);
            if (objNode == AndersNodeFactory::InvalidIndex) {
              objNode = nodeFactory.createObjectNodeDummy(Base, *Mod);
//                            objNode = nodeFactory.createObjectNode(Base);
            }
            addConstraint(AndersConstraint::ADDR_OF, srcIndex, objNode);
            addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
//                    addConstraint(AndersConstraint::COPY, valNode, dstIndex);
            findAliases(Base, true);

            continue;
          } else if (((Instruction *) ((Instruction *) op)->getOperand(0))->getOpcode() == Instruction::PHI) {
            DEBUG_WITH_TYPE("err", errs() << "HANDLE PHI BASE NODE: ";
              ((Instruction *) op)->getOperand(0)->print(errs());
              errs() << "\n";);
//                    assert(false);
          } else {
//                        Instruction *base = nullptr;
//                        Instruction *offset = nullptr;
//                        if (PatternMatch::match(op, PatternMatch::m_BinOp(PatternMatch::m_Instruction(base), PatternMatch::m_Instruction(offset)))) {
//                            NodeIndex valNode = nodeFactory.getValueNodeFor(inst);
//                            assert(valNode != AndersNodeFactory::InvalidIndex);
//                            NodeIndex srcIndex = nodeFactory.getValueNodeFor(base);
//                            if (srcIndex == AndersNodeFactory::InvalidIndex) {
//                                srcIndex = nodeFactory.createValueNode(base);
//                            }
//                            NodeIndex objNode = nodeFactory.getObjectNodeFor(base);
//                            if (objNode == AndersNodeFactory::InvalidIndex) {
//                                objNode = nodeFactory.createObjectNodeDummy(base, *Mod);
////                            objNode = nodeFactory.createObjectNode(Base);
//                            }
//                            addConstraint(AndersConstraint::ADDR_OF, srcIndex, objNode);
//                            addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
////                    addConstraint(AndersConstraint::COPY, valNode, dstIndex);
//                            findAliases(base, true);
//
//                            continue;
//                        }
////                        op->dump();
////                        assert(false);
          }
        }
        // Otherwise, we really don't know what dst points to
        addConstraint(AndersConstraint::COPY, dstIndex, nodeFactory.getUniversalPtrNode());

      }

      break;
    }
    case Instruction::Select: {
      if (inst->getType()->isDoubleTy() || inst->getType()->isFloatTy() || inst->getType()->isFloatingPointTy()) {
        break;
      }
      if (inst->getType()->isPointerTy()) {
        ConstantInt *Const;
        if (PatternMatch::match(inst->getOperand(1), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(Const)))) {
          addConstraintsForConstIntToPtr(inst->getOperand(1), Const);
        }


        if (PatternMatch::match(inst->getOperand(2), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(Const)))) {
          addConstraintsForConstIntToPtr(inst->getOperand(2), Const);
        }

      }
      NodeIndex srcIndex1 = nodeFactory.getValueNodeFor(inst->getOperand(1));
      if (srcIndex1 == AndersNodeFactory::InvalidIndex)
        srcIndex1 = nodeFactory.createValueNode(inst->getOperand(1));
      assert(srcIndex1 != AndersNodeFactory::InvalidIndex && "Failed to find select src node 1");
      NodeIndex srcIndex2 = nodeFactory.getValueNodeFor(inst->getOperand(2));
      if (srcIndex2 == AndersNodeFactory::InvalidIndex)
        srcIndex2 = nodeFactory.createValueNode(inst->getOperand(2));
      assert(srcIndex2 != AndersNodeFactory::InvalidIndex && "Failed to find select src node 2");
      NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
      if (dstIndex == AndersNodeFactory::InvalidIndex)
        dstIndex = nodeFactory.createValueNode(inst);
      assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find select dst node");
      addConstraint(AndersConstraint::COPY, dstIndex, srcIndex1);
      addConstraint(AndersConstraint::COPY, dstIndex, srcIndex2);
      break;
    }
    case Instruction::VAArg: {
      if (inst->getType()->isPointerTy()) {
        NodeIndex dstIndex = nodeFactory.getValueNodeFor(inst);
        assert(dstIndex != AndersNodeFactory::InvalidIndex && "Failed to find va_arg dst node");
        NodeIndex vaIndex = nodeFactory.getVarargNodeFor(inst->getParent()->getParent());
        assert(vaIndex != AndersNodeFactory::InvalidIndex && "Failed to find vararg node");
        addConstraint(AndersConstraint::COPY, dstIndex, vaIndex);
      }
      break;
    }
    case Instruction::ExtractValue:
    case Instruction::InsertValue: {
      if (!inst->getType()->isPointerTy())
        break;
    }
      // We have no intention to support exception-handling in the near future
    case Instruction::LandingPad:
    case Instruction::Resume:
      // Atomic instructions can be modeled by their non-atomic counterparts. To be supported
    case Instruction::AtomicRMW:
    case Instruction::AtomicCmpXchg: {
      DEBUG(errs() << *inst << "\n");
      assert(false && "not implemented yet");
    }
    case Instruction::Add: {
      //Check if a IVAR is accessed
      Instruction *load = nullptr;
      if (PatternMatch::match(inst, PatternMatch::m_BinOp(PatternMatch::m_Value(),
                                                          PatternMatch::m_SExt(PatternMatch::m_Instruction(load))))) {
        ConstantInt *address = nullptr;
        if (PatternMatch::match(load->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(address)))) {
          NodeIndex srcIdx = nodeFactory.getValueNodeFor(load->getOperand(0));
          if (srcIdx == AndersNodeFactory::InvalidIndex)
            srcIdx = nodeFactory.createValueNode(load->getOperand(0));
          NodeIndex dstIdx = nodeFactory.getValueNodeFor(inst);
          if (dstIdx == AndersNodeFactory::InvalidIndex)
            dstIdx = nodeFactory.createValueNode(inst);
          addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
        }
      }
      break;
    }
    case Instruction::SExt: {
      if (const LoadInst *load = dyn_cast<const LoadInst>(inst->getOperand(0))) {
        if (PatternMatch::match(load->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt()))) {
          NodeIndex srcIdx = nodeFactory.getValueNodeFor(inst->getOperand(0));
          if (srcIdx == AndersNodeFactory::InvalidIndex)
            srcIdx = nodeFactory.createValueNode(inst->getOperand(0));
          NodeIndex dstIdx = nodeFactory.getValueNodeFor(inst);
          if (dstIdx == AndersNodeFactory::InvalidIndex)
            dstIdx = nodeFactory.createValueNode(inst);
          addConstraint(AndersConstraint::COPY, dstIdx, srcIdx);
        }
      }
      break;
    }
    default: {
      DEBUG(errs() << *inst << "\n");
      assert(!inst->getType()->isPointerTy() && "pointer-related inst not handled!");
      break;
    }
  }
}

// There are two types of constraints to add for a function call:
// - ValueNode(callsite) = ReturnNode(call target)
// - ValueNode(formal arg) = ValueNode(actual arg)
void Andersen::addConstraintForCall(ImmutableCallSite cs) {
#ifdef DECOMPILED
  if (Function *f = (Function *) cs.getCalledFunction())    // Direct call
  {
    DEBUG(errs() << "Call: " << cs.getCaller()->getName() << " -> " << cs.getCalledFunction()->getName() << "\n");
    if (f->isDeclaration()) // || f->isIntrinsic())    // External library call
    {
      if (!f->isIntrinsic()) {
//            errs() << cs.getCalledFunction()->getName() << "\n";
        if (getSuccInBlock(cs.getInstruction())->getName() == "LR_7786") {
          assert(true);
        }
        std::string functionName = cs.getCalledFunction()->getName().str();
        if (!ObjectiveC::getGlobalCallHandlerManager().handleFunctionCall(cs.getInstruction(),
                                                                          functionName, this)) {
          std::unique_lock<std::mutex> lock(outputLock);
          addUnhandled(cs.getCalledFunction()->getName(), cs.getInstruction());
//                    *unhandledFunctions << "Can't handle call: " << cs.getCalledFunction()->getName() << "\n";
        }
//            assert(false);
      }
    } else { // Internal call
      addToWorklist((Instruction *) cs.getInstruction());
      addConstraintsForCall((CallInst *) cs.getInstruction(), f);
    }
  } else {
    if (const CallInst *Call = dyn_cast<const CallInst>(cs.getInstruction())) {
      if (Call->getCalledValue()->getType()->isPointerTy()) {
        addToWorklist((Instruction *) Call);
        Instruction *LoadInst = nullptr;
        if (PatternMatch::match(Call->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_Instruction(LoadInst))) &&
            LoadInst && LoadInst->getOpcode() == Instruction::Load) {
          Value *BlockBase = nullptr;
          if (PatternMatch::match(LoadInst->getOperand(0), PatternMatch::m_IntToPtr(
            PatternMatch::m_BinOp(PatternMatch::m_Value(BlockBase), PatternMatch::m_ConstantInt())))) {
            const Value *B = nullptr;
            if (isBlock(dyn_cast<Instruction>(BlockBase), B)) {
              assert(B);
              handleBlock(Call, B);
            }
          }
        } else {
          if (const Instruction *CV_Inst = dyn_cast<const Instruction>(Call->getCalledValue())) {

            std::vector<const llvm::Value *> PtsTo;
            getPointsToSet(CV_Inst->getOperand(0), PtsTo);
            for (auto P : PtsTo) {
              const ConstantDataArray *ConstData = dyn_cast<const ConstantDataArray>(P);
              if (!ConstData)
                continue;
              std::string FName = ConstData->getAsString();
              Function *F = Mod->getFunction(FName);
              if (F && !F->isIntrinsic() && !F->isDeclaration()) {
                addConstraintsForCall(Call, F);
              } else {
                if (!ObjectiveC::getGlobalCallHandlerManager().handleFunctionCall(Call, FName, this)) {
                  std::unique_lock<std::mutex> lock(outputLock);
                  addUnhandled(FName, Call);
                }
              }
            }
          }
        }
      }
    }
  }
#else
  if (const Function* f = cs.getCalledFunction())	// Direct call
  {
    if (f->isDeclaration() || f->isIntrinsic())	// External library call
    {
      // Handle libraries separately
      if (addConstraintForExternalLibrary(cs, f))
        return;
      else	// Unresolved library call: ruin everything!
      {
        errs() << "Unresolved ext function: " << f->getName() << "\n";
        if (cs.getType()->isPointerTy())
        {
          NodeIndex retIndex = nodeFactory.getValueNodeFor(cs.getInstruction());
          assert(retIndex != AndersNodeFactory::InvalidIndex && "Failed to find ret node!");
          addConstraint(AndersConstraint::COPY, retIndex, nodeFactory.getUniversalPtrNode());
        }
        for (ImmutableCallSite::arg_iterator itr = cs.arg_begin(), ite = cs.arg_end(); itr != ite; ++itr)
        {
          Value* argVal = *itr;
          if (argVal->getType()->isPointerTy())
          {
            NodeIndex argIndex = nodeFactory.getValueNodeFor(argVal);
            assert(argIndex != AndersNodeFactory::InvalidIndex && "Failed to find arg node!");
            addConstraint(AndersConstraint::COPY, argIndex, nodeFactory.getUniversalPtrNode());
          }
        }
      }
    }
    else	// Non-external function call
    {
      if (cs.getType()->isPointerTy())
      {
        NodeIndex retIndex = nodeFactory.getValueNodeFor(cs.getInstruction());
        assert(retIndex != AndersNodeFactory::InvalidIndex && "Failed to find ret node!");
        //errs() << f->getName() << "\n";
        NodeIndex fRetIndex = nodeFactory.getReturnNodeFor(f);
        assert(fRetIndex != AndersNodeFactory::InvalidIndex && "Failed to find function ret node!");
        addConstraint(AndersConstraint::COPY, retIndex, fRetIndex);
      }
      // The argument constraints
      addArgumentConstraintForCall(cs, f);
    }
  }
  else	// Indirect call
  {
    // We do the simplest thing here: just assume the returned value can be anything :)
    if (cs.getType()->isPointerTy())
    {
      NodeIndex retIndex = nodeFactory.getValueNodeFor(cs.getInstruction());
      assert(retIndex != AndersNodeFactory::InvalidIndex && "Failed to find ret node!");
      addConstraint(AndersConstraint::COPY, retIndex, nodeFactory.getUniversalPtrNode());
    }

    // For argument constraints, first search through all addr-taken functions: any function that takes can take as many variables is a potential candidate
    const Module* M = cs.getInstruction()->getParent()->getParent()->getParent();
    for (auto const& f: *M)
    {
      NodeIndex funPtrIndex = nodeFactory.getValueNodeFor(&f);
      if (funPtrIndex == AndersNodeFactory::InvalidIndex)
        // Not an addr-taken function
        continue;

      if (!f.getFunctionType()->isVarArg() && f.arg_size() != cs.arg_size())
        // #arg mismatch
        continue;

      if (f.isDeclaration() || f.isIntrinsic())	// External library call
      {
        if (addConstraintForExternalLibrary(cs, &f))
          continue;
        else
        {
          // Pollute everything
          for (ImmutableCallSite::arg_iterator itr = cs.arg_begin(), ite = cs.arg_end(); itr != ite; ++itr)
          {
            NodeIndex argIndex = nodeFactory.getValueNodeFor(*itr);
            assert(argIndex != AndersNodeFactory::InvalidIndex && "Failed to find arg node!");
            addConstraint(AndersConstraint::COPY, argIndex, nodeFactory.getUniversalPtrNode());
          }
        }
      }
      else
        addArgumentConstraintForCall(cs, &f);
    }
  }
#endif
}

void Andersen::addConstraintsForCall(const llvm::Instruction *Inst, const llvm::Function *F) {
  if (
    Inst->getParent()->getParent()->getName() == "-[RNEncryptor initWithSettings:encryptionKey:HMACKey:IV:handler:]" &&
    F->getName() == "-[RNCryptorEngine initWithOperation:settings:key:IV:error:]") {
    assert(true);
  }
  if (CallGraph->containtsEdge(Inst, F->getName()))
    return;

  CallGraph->addCallEdge(Inst, F->getName());
  DetectParametersPass *DetectParams = getAnalysisIfAvailable<DetectParametersPass>();
  if (!DetectParams)
    DetectParams = &getAnalysis<DetectParametersPass>();
  StackAccessPass *StackAccess = getAnalysisIfAvailable<StackAccessPass>();
  if (!StackAccess)
    StackAccess = &getAnalysis<StackAccessPass>();
  DetectParametersPass::ParameterAccessPairSet_t S = DetectParams->getParameterStackOffsets(F);
  if (S.size()) {
    int64_t StackSize = StackAccess->getStackPointerValue(Inst);
    for (DetectParametersPass::ParameterAccessPairSet_t::iterator PA_it = S.begin();
         PA_it != S.end(); ++PA_it) {
      if (StackSize != -1U)
        findSetStackParameterInstruction((Instruction *) Inst, *PA_it, StackSize);
    }
  }
  DetectParametersPass::ParameterAccessPairSet_t R = DetectParams->getParameterRegisterIndexes(F);
  for (DetectParametersPass::ParameterAccessPairSet_t::iterator PA_it = R.begin(); PA_it != R.end(); ++PA_it) {
    findSetRegisterParameterInstruction((Instruction *) Inst, *PA_it);
  }

  DetectParametersPass::ParameterAccessPairSet_t Return = DetectParams->getReturnRegisterIndexes(F);
  for (DetectParametersPass::ParameterAccessPairSet_t::iterator Ret_it = Return.begin();
       Ret_it != Return.end(); ++Ret_it) {

    DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(Ret_it->first, Inst);
    for (DetectParametersPass::UserSet_t::iterator P_it = Post.begin(); P_it != Post.end(); ++P_it) {

      assert(isa<StoreInst>(Ret_it->second));
      NodeIndex srcIndex = nodeFactory.getValueNodeFor(Ret_it->second->getOperand(0));
      if (srcIndex == AndersNodeFactory::InvalidIndex)
        srcIndex = nodeFactory.createValueNode(Ret_it->second->getOperand(0));
      NodeIndex dstIndex = nodeFactory.getValueNodeFor(*P_it);
      if (dstIndex == AndersNodeFactory::InvalidIndex)
        dstIndex = nodeFactory.createValueNode(*P_it);
      addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
    }
  }
  /*
  for (unsigned i = 5; i < 14; ++i) {
      if (!copyParameter(Inst, F, i))
          preserveRegisterValue(Inst, i);
  }*/
}

void Andersen::addArgumentConstraintForCall(ImmutableCallSite cs, const Function *f) {
  Function::const_arg_iterator fItr = f->arg_begin();
  ImmutableCallSite::arg_iterator aItr = cs.arg_begin();
  while (fItr != f->arg_end() && aItr != cs.arg_end()) {
    const Argument *formal = fItr;
    const Value *actual = *aItr;

    if (formal->getType()->isPointerTy()) {
      NodeIndex fIndex = nodeFactory.getValueNodeFor(formal);
      assert(fIndex != AndersNodeFactory::InvalidIndex && "Failed to find formal arg node!");
      if (actual->getType()->isPointerTy()) {
        NodeIndex aIndex = nodeFactory.getValueNodeFor(actual);
        assert(aIndex != AndersNodeFactory::InvalidIndex && "Failed to find actual arg node!");
        addConstraint(AndersConstraint::COPY, fIndex, aIndex);
      }
      else
        addConstraint(AndersConstraint::COPY, fIndex, nodeFactory.getUniversalPtrNode());
    }
    ++fItr, ++aItr;
  }

  // Copy all pointers passed through the varargs section to the varargs node
  if (f->getFunctionType()->isVarArg()) {
    while (aItr != cs.arg_end()) {
      const Value *actual = *aItr;
      if (actual->getType()->isPointerTy()) {
        NodeIndex aIndex = nodeFactory.getValueNodeFor(actual);
        assert(aIndex != AndersNodeFactory::InvalidIndex && "Failed to find actual arg node!");
        NodeIndex vaIndex = nodeFactory.getVarargNodeFor(f);
        assert(vaIndex != AndersNodeFactory::InvalidIndex && "Failed to find vararg node!");
        addConstraint(AndersConstraint::COPY, vaIndex, aIndex);
      }

      ++aItr;
    }
  }
}

bool Andersen::findAliases(const llvm::Value *Address, bool Sharp, uint64_t SPIdx) {
  static std::mutex aliasLock;
  std::unique_lock<std::mutex> lock(aliasLock);
  if (const Instruction *Inst = dyn_cast<const Instruction>(Address)) {
    Function &F = *((Function *) Inst->getParent()->getParent());
    StackAccessPass *StackAccess = getAnalysisIfAvailable<StackAccessPass>();
    if (!StackAccess)
      StackAccess = &getAnalysis<StackAccessPass>();
    StackAccessPass::OffsetMap_t OffsetsTmp;
    StackAccessPass::OffsetValueListMap_t OffsetValuesTmp;
    if (SPIdx != 3) {
      std::set<uint64_t> SPSet;
      SPSet.insert(SPIdx);
      StackAccess->runOnFunction(F, OffsetsTmp, OffsetValuesTmp, SPSet);
    }
    StackAccessPass::OffsetMap_t &Offsets = SPIdx == 3 ? StackAccess->getOffsets(&F) : OffsetsTmp;
    StackAccessPass::OffsetValueListMap_t &OffsetValues =
      SPIdx == 3 ? StackAccess->getOffsetValues(&F) : OffsetValuesTmp;

    if (Sharp) {
      if (Offsets[Address] == nullptr) {
        return false;
      }
      for (StackAccessPass::Int64List_t::iterator Offset_it = Offsets[Address]->begin();
           Offset_it != Offsets[Address]->end(); ++Offset_it) {
        for (StackAccessPass::ValueList_t::iterator V_it = OffsetValues[*Offset_it]->begin();
             V_it != OffsetValues[*Offset_it]->end(); ++V_it) {
          if (Address == *V_it)
            continue;
          NodeIndex idxA = nodeFactory.getValueNodeFor(Address);
          assert(idxA != AndersNodeFactory::InvalidIndex);
          NodeIndex idxB = nodeFactory.getValueNodeFor(*V_it);
          if (idxB == AndersNodeFactory::InvalidIndex) {
            idxB = nodeFactory.createValueNode(*V_it);
          }
          addConstraint(AndersConstraint::COPY, idxB, idxA);
          handledAliases.insert(*V_it);
        }
      }
    } else {
      return false;
      int64_t min = INT64_MAX;
      if (!Offsets[Address]) {
        DEBUG_WITH_TYPE("err", errs() << "CANT FIND ANY BASE POINTER: ";
          Address->print(errs());
          errs() << "\n";);
        min = INT64_MIN;
      } else {
        for (StackAccessPass::Int64List_t::iterator Offset_it = Offsets[Address]->begin();
             Offset_it != Offsets[Address]->end(); ++Offset_it) {
          if (*Offset_it < min)
            min = *Offset_it;
        }
      }
      for (StackAccessPass::OffsetValueListMap_t::iterator OV_it = OffsetValues.begin();
           OV_it != OffsetValues.end(); ++OV_it) {
        if (OV_it->first >= min) {
          if (!OffsetValues[OV_it->first]) {
//                        errs() << "Can't find offset values\n";
//                        errs() << F.getName() << "\n";
            continue;
          }
          for (StackAccessPass::ValueList_t::iterator V_it = OffsetValues[OV_it->first]->begin();
               V_it != OffsetValues[OV_it->first]->end(); ++V_it) {
            if (Address == *V_it)
              continue;
            NodeIndex idxA = nodeFactory.getValueNodeFor(Address);
            assert(idxA != AndersNodeFactory::InvalidIndex);
            NodeIndex idxB = nodeFactory.getValueNodeFor(*V_it);
            if (idxB == AndersNodeFactory::InvalidIndex) {
              idxB = nodeFactory.createValueNode(*V_it);
            }
            addConstraint(AndersConstraint::COPY, idxB, idxA);
            //TODO: should we insert this into handled aliases?
          }
        }
      }
    }
  }
  return false;
}


Instruction *Andersen::findSetStackParameterInstruction(Instruction *CallInst,
                                                        DetectParametersPass::ParameterAccessPair_t Parameter,
                                                        int64_t StackSize, int64_t CopyInParent) {
  Value *Address = nullptr;
  if (Parameter.second->getOpcode() == Instruction::Load) {
    if (Instruction *AddressPtr = dyn_cast<Instruction>(Parameter.second->getOperand(0))) {
      if (AddressPtr->getOpcode() == Instruction::IntToPtr) {
        Address = AddressPtr->getOperand(0);
      }
    }
  } else if (Parameter.second->getOpcode() == Instruction::Store) {
    if (Instruction *AddressPtr = dyn_cast<Instruction>(Parameter.second->getOperand(1))) {
      if (AddressPtr->getOpcode() == Instruction::IntToPtr) {
        Address = AddressPtr->getOperand(0);
      }
    }
  }
  assert(Address);
  Function *F = (Function *) CallInst->getParent()->getParent();
  int64_t Offset = Parameter.first + StackSize;
  StackAccessPass *StackAccess = getAnalysisIfAvailable<StackAccessPass>();
  if (!StackAccess)
    StackAccess = &getAnalysis<StackAccessPass>();
  StackAccessPass::OffsetValueListMap_t &ValueMap = StackAccess->getOffsetValues(F);
  if (ValueMap[Offset] == nullptr) {
    return nullptr;
  }
  StackAccessPass::ValueList_t &Values = *ValueMap[Offset];
  if (!Values.size())
    llvm_unreachable("Can't find Instruction that sets the parameter");

  NodeIndex idxA = nodeFactory.getValueNodeFor(*Values.begin());
  if (idxA == AndersNodeFactory::InvalidIndex) {
    idxA = nodeFactory.createValueNode(*Values.begin());
  }
  assert(idxA != AndersNodeFactory::InvalidIndex);
  NodeIndex idxB = nodeFactory.getValueNodeFor(Address);
  if (idxB == AndersNodeFactory::InvalidIndex) {
    idxB = nodeFactory.createValueNode(Address);
  }
  addConstraint(AndersConstraint::COPY, idxB, idxA);

  if (CopyInParent) {
    for (StackAccessPass::ValueList_t::iterator V_it = Values.begin(); V_it != Values.end(); ++V_it) {
      for (Value::const_use_iterator IU_it = (*V_it)->use_begin(); IU_it != (*V_it)->use_end(); ++IU_it) {
        Instruction *ItoP = dyn_cast<Instruction>(IU_it->getUser());
        if (!ItoP || ItoP->getOpcode() != Instruction::IntToPtr)
          continue;
        for (Value::use_iterator SU_it = ItoP->use_begin(); SU_it != ItoP->use_end(); ++SU_it) {
          StoreInst *S = dyn_cast<StoreInst>(SU_it->getUser());
          if (!S)
            continue;

          StackAccessPass::OffsetMap_t &Offsets = StackAccess->getOffsets(F);
          for (StackAccessPass::Int64List_t::iterator O_it = Offsets[S->getOperand(0)]->begin();
               O_it != Offsets[S->getOperand(0)]->end(); ++O_it) {
            int64_t O1 = *O_it + 8;
            int64_t O2 = *O_it + CopyInParent;
            if (ValueMap[O1] == nullptr)
              continue;
            if (ValueMap[O2] == nullptr)
              continue;
            StackAccessPass::ValueList_t &VList1 = *ValueMap[O1];
            StackAccessPass::ValueList_t &VList2 = *ValueMap[O2];
            assert(VList1.size() && VList2.size());

            for (StackAccessPass::ValueList_t::iterator V1_it = VList1.begin(); V1_it != VList1.end(); ++V1_it) {
              for (Value::const_use_iterator V1U_it = (*V1_it)->use_begin(); V1U_it != (*V1_it)->use_end(); ++V1U_it) {
                IntToPtrInst *ItoP = dyn_cast<IntToPtrInst>(V1U_it->getUser());
                if (!ItoP)
                  continue;
                for (Value::use_iterator ItoPU_it = ItoP->use_begin(); ItoPU_it != ItoP->use_end(); ++ItoPU_it) {
                  LoadInst *LI = dyn_cast<LoadInst>(ItoPU_it->getUser());
                  if (!LI)
                    continue;
                  for (Value::use_iterator LIU_it = LI->use_begin(); LIU_it != LI->use_end(); ++LIU_it) {
                    Instruction *AI = dyn_cast<Instruction>(LIU_it->getUser());
                    if (!AI || AI->getOpcode() != Instruction::Add)
                      continue;
                    ConstantInt *C = dyn_cast<ConstantInt>(AI->getOperand(1));
                    if (!C)
                      continue;
                    if ((int64_t) C->getZExtValue() != CopyInParent)
                      continue;
                    NodeIndex dst = nodeFactory.getValueNodeFor(AI);
                    NodeIndex src = nodeFactory.getValueNodeFor(*VList2.begin());
                    addConstraint(AndersConstraint::COPY, dst, src);
                    addConstraint(AndersConstraint::COPY, idxB, src);
                  }
                }
              }
            }

          }
        }
      }
    }
  }
  return Address ? dyn_cast<Instruction>(Address) : nullptr;
}

Instruction *Andersen::findSetRegisterParameterInstruction(Instruction *CallInst,
                                                           DetectParametersPass::ParameterAccessPair_t Parameter) {
  std::unique_lock<std::mutex> lock(paramLock);
  StackAccessPass::OffsetMap_t OffsetMap_param;
  StackAccessPass::OffsetValueListMap_t OffsetValueListMap_param;
  std::set<uint64_t> SPIdx;
  SPIdx.insert(Parameter.first);
  StackAccessPass::runOnFunction(*Parameter.second->getParent()->getParent(), OffsetMap_param,
                                 OffsetValueListMap_param, SPIdx);

  StackAccessPass *SAP = &getAnalysis<StackAccessPass>();

  StackAccessPass::OffsetMap_t &OffsetMap_caller = SAP->getOffsets(CallInst->getParent()->getParent());
  StackAccessPass::OffsetValueListMap_t &OffsetValueListMap_caller = SAP->getOffsetValues(
    CallInst->getParent()->getParent());

  std::set<int64_t> OffsetsToFind;

  //Check if this parameters is used as base pointer (structs do it like this)
  for (auto &OV_it : OffsetValueListMap_param) {
    //The first element in a struct is handled like a 'normal' variable
    if (OV_it.first == 0) {
      continue;
    }
    OffsetsToFind.insert(OV_it.first);
  }

  DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(Parameter.first, CallInst);
  assert(Pre.size() > 0);
  for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {
    NodeIndex dstIndex = nodeFactory.getValueNodeFor(Parameter.second);
    if (dstIndex == AndersNodeFactory::InvalidIndex)
      dstIndex = nodeFactory.createValueNode(Parameter.second);
    NodeIndex srcIndex = nodeFactory.getValueNodeFor(*Pre_it);
    if (srcIndex == AndersNodeFactory::InvalidIndex)
      srcIndex = nodeFactory.createValueNode(*Pre_it);
    addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);

    if (OffsetMap_caller.find(*Pre_it) != OffsetMap_caller.end()) {
      //This means that this is a stack address
      if (!OffsetMap_caller[*Pre_it])
        continue;
      if (OffsetMap_caller[*Pre_it]->size()) {
        std::vector<const Value *> ptsTo;
        getPointsToSet(*Pre_it, ptsTo);
        if (ptsTo.size() == 0) {
          NodeIndex valIdx = nodeFactory.getValueNodeFor(*Pre_it);
          assert(valIdx != AndersNodeFactory::InvalidIndex);
          NodeIndex objIdx = nodeFactory.createObjectNodeDummy(*Pre_it, *Mod);
          addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
          if (handledAliases.find(*Pre_it) == handledAliases.end()) {
            findAliases(*Pre_it, true);
          }
        }
      }
      for (auto &O : OffsetsToFind) {
        for (auto &O_C : *OffsetMap_caller[*Pre_it]) {
          int Find = O_C + O;
          if (OffsetValueListMap_caller[Find]) {
            for (auto &OV_caller : *OffsetValueListMap_caller[Find]) {
              for (auto &OV_param : *OffsetValueListMap_param[O]) {
                NodeIndex dstIndex2 = nodeFactory.getValueNodeFor(OV_param);
                if (dstIndex2 == AndersNodeFactory::InvalidIndex)
                  dstIndex2 = nodeFactory.createValueNode(OV_param);
                NodeIndex srcIndex2 = nodeFactory.getValueNodeFor(OV_caller);
                if (srcIndex2 == AndersNodeFactory::InvalidIndex)
                  srcIndex2 = nodeFactory.createValueNode(OV_caller);
                addConstraint(AndersConstraint::COPY, dstIndex2, srcIndex2);
              }
            }
          } else {
            DEBUG(errs() << "Offset: " << O << "\n";
                    errs() << Parameter.second->getParent()->getParent()->getName() << "\n";);
//                        assert(false && "Struct was not accessed?");
          }
        }
      }
    } else if (OffsetsToFind.size()) {
      //TODO: dynamically allocated structs, "forwarded" structs: structs that are already paramters in the calling function (and the location requested here may not be accessed in the caller)
//            assert(false && "Structs in registers are not handled yet");
    }
  }

////    DominatorTree &DomTree = getAnalysis<DominatorTreeWrapperPass>(*CallInst->getParent()->getParent()).getDomTree();
//    DominatorTreeWrapperPass *DomTreePass = getAnalysisIfAvailable<DominatorTreeWrapperPass>();
//    if (!DomTreePass)
//        DomTreePass = &getAnalysis<DominatorTreeWrapperPass>(*CallInst->getParent()->getParent());
//    DominatorTree &DomTree = DomTreePass->getDomTree();
//    if (Parameter.second->getOpcode() == Instruction::Load) {
//        if (GetElementPtrInst *Inst = dyn_cast<GetElementPtrInst>(Parameter.second->getOperand(0))) {
//            if (ConstantInt *IdxValue = dyn_cast<ConstantInt>(Inst->getOperand(2))) {
//                for (BasicBlock::InstListType::iterator I_it = CallInst->getParent()->getParent()->getEntryBlock().begin();
//                        I_it != CallInst->getParent()->getParent()->getEntryBlock().end();
//                        ++I_it) {
//                    if (I_it->getOpcode() == Instruction::GetElementPtr) {
//                        if (I_it->getOperand(2) == IdxValue) {
//                            Instruction *LastLoad = nullptr;
//                            for (Value::const_use_iterator U_it = I_it->use_begin();
//                                    U_it != I_it->use_end();
//                                    ++U_it) {
//                                if (((Instruction*)U_it->getUser())->getOpcode() == Instruction::Store) {
//                                    if (DomTree.dominates((Instruction*)U_it->getUser(), CallInst)) {
//                                        if (LastLoad == nullptr || DomTree.dominates(LastLoad, (Instruction*)U_it->getUser())) {
//                                            LastLoad = (Instruction*)U_it->getUser();
//                                        }
//                                    }
//                                }
//                            }
//                            assert(LastLoad);
//
//                            NodeIndex idxA = nodeFactory.getValueNodeFor(LastLoad->getOperand(0));
//                            if (idxA == AndersNodeFactory::InvalidIndex) {
//                                idxA = nodeFactory.createValueNode(LastLoad->getOperand(0));
//                            }
//                            assert(idxA != AndersNodeFactory::InvalidIndex);
//                            NodeIndex idxB = nodeFactory.getValueNodeFor(Parameter.second);
//                            if (idxB == AndersNodeFactory::InvalidIndex) {
//                                idxB = nodeFactory.createValueNode(Parameter.second);
//                            }
//                            addConstraint(AndersConstraint::COPY, idxB, idxA);
//                        }
//                    }
//                }
//            }
//        }
//    }
  return nullptr;
}

void Andersen::addConstraintsForConstIntToPtr(const llvm::Value *IntToPtr, const llvm::ConstantInt *Const) {
  uint64_t V = 0;
  if (!MachO->getValue(Const->getZExtValue(), V)) {
//        return;
  }

  if (Const->getZExtValue() == 4295225424) {
    assert(true);
  }
  //FIXME: If no pointer was set (the address referenced here is 0x0) should we use the pointer address instead?
//    ConstantInt *Dummy = ConstantInt::get(getGlobalContext(), APInt(64, V ? V : Const->getZExtValue()));

  bool isIVAR = false;
  if (MachO->isIVAR(Const->getZExtValue())) {
    isIVAR = true;
    ivarMap[Const->getZExtValue()] = IntToPtr;
  }

  std::string Data = MachO->getString(V ? V : Const->getZExtValue());
  Value *PointsToData = nullptr;
  if (Data.size() > 0 && !isIVAR) {
    PointsToData = (ConstantDataArray *) ConstantDataArray::getString(getGlobalContext(), Data, false);
  } else {
    if (nodeFactory.getAbstractLocation(Const) != Const) {

    } else {
      nodeFactory.createObjectNodeDummy(Const, *Mod);
    }
    PointsToData = (Value *) nodeFactory.getAbstractLocation(Const);
//        if (DummyMap.find(Const) == DummyMap.end()) {
//            PointsToData = new llvm::GlobalVariable(*Mod, llvm::IntegerType::get(llvm::getGlobalContext(), 1), false, llvm::GlobalVariable::ExternalLinkage,
//                                                    nullptr);
//
//            DummyMap[Const] = PointsToData;
//        } else {
//            PointsToData = DummyMap[Const];
//        }
  }


  NodeIndex dataObject = nodeFactory.getObjectNodeFor(PointsToData);
  if (dataObject == AndersNodeFactory::InvalidIndex) {
    dataObject = nodeFactory.createObjectNode(PointsToData);
  }

  NodeIndex dstIdx = nodeFactory.getValueNodeFor(IntToPtr);
  if (dstIdx == AndersNodeFactory::InvalidIndex) {
    dstIdx = nodeFactory.createValueNode(IntToPtr);
  }

  addConstraint(AndersConstraint::ADDR_OF, dstIdx, dataObject);

/*

//    if (MachO->isIVAR(V ? V : Const->getZExtValue()))
//        return;



    NodeIndex DV = nodeFactory.getValueNodeFor(Dummy);
    NodeIndex DO = nodeFactory.getObjectNodeFor(PointsToData);
    if (DO == AndersNodeFactory::InvalidIndex) {
        DO = nodeFactory.createObjectNode(PointsToData);
    }
    if (DV == AndersNodeFactory::InvalidIndex) {
        DV = nodeFactory.createValueNode(Dummy);
    }
    addConstraint(AndersConstraint::ADDR_OF, DV, DO);

    NodeIndex  C = nodeFactory.getValueNodeFor(Const);
    if (C == AndersNodeFactory::InvalidIndex) {
        C = nodeFactory.createValueNode(Const);
        addConstraint(AndersConstraint::ADDR_OF, C, nodeFactory.createObjectNode(Const));
    }
    addConstraint(AndersConstraint::STORE, C, DV);
//
//    addConstraint(AndersConstraint::ADDR_OF, C, nodeFactory.getObjectNodeFor(Const) == AndersNodeFactory::InvalidIndex ? nodeFactory.createObjectNode(Const) : nodeFactory.getObjectNodeFor(Const));

    assert(isa<User>(IntToPtr));
    NodeIndex ItoP = nodeFactory.getValueNodeFor(((User*)IntToPtr)->getOperand(0));
    if (ItoP == AndersNodeFactory::InvalidIndex) {
        ItoP = nodeFactory.createValueNode(((User*)IntToPtr)->getOperand(0));
    }
    addConstraint(AndersConstraint::LOAD, ItoP, C);

    NodeIndex dstIdx = nodeFactory.getValueNodeFor(IntToPtr);
    if (dstIdx == AndersNodeFactory::InvalidIndex) {
        dstIdx = nodeFactory.createValueNode(IntToPtr);
    }
    addConstraint(AndersConstraint::COPY, dstIdx, ItoP);*/
}

void Andersen::preserveRegisterValue(llvm::Instruction *CallInst, uint64_t RegNo) {
  DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(RegNo, CallInst);
  DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(RegNo, CallInst);

  for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {
    const Instruction *PreInst = dyn_cast<const Instruction>(*Pre_it);

    if (!PreInst) // || PreInst->getParent() != CallInst->getParent())
      continue;
    for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
      const Instruction *PostInst = dyn_cast<const Instruction>(*Post_it);
      if (!PostInst) // || PostInst->getParent() != CallInst->getParent())
        continue;

      NodeIndex srcIndex = nodeFactory.getValueNodeFor(PreInst);
      if (srcIndex == AndersNodeFactory::InvalidIndex)
        srcIndex = nodeFactory.createValueNode(PreInst);
      NodeIndex dstIndex = nodeFactory.getValueNodeFor(PostInst);
      if (dstIndex == AndersNodeFactory::InvalidIndex)
        dstIndex = nodeFactory.createValueNode(PostInst);
      addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
    }
  }
}


bool Andersen::copyParameter(llvm::Instruction *CallInst, llvm::Function *F, uint64_t RegNo) {
  BasicBlock *ExitBB = nullptr;
  for (Function::iterator BB_it = F->begin(); BB_it != F->end(); ++BB_it) {
    if (BB_it->getTerminator()->getOpcode() == Instruction::Ret) {
//        if (BB_it->getName().startswith("exit_")) {
      ExitBB = &*BB_it;
      break;
    }
  }

  if (!ExitBB) {
//        errs() << "No 'return' found in: " << F->getName() << "\n";
    return false;
  }

  assert(ExitBB);

  Value *Prev = nullptr;

  for (Instruction *I = ExitBB->getTerminator(); I != &ExitBB->front(); I = I->getPrevNode()) {
    if (I->getOpcode() == Instruction::Store) {
      Instruction *Address = dyn_cast<Instruction>(I->getOperand(1));
      if (Address && Address->getOpcode() == Instruction::GetElementPtr) {
        if (ConstantInt *Idx = dyn_cast<ConstantInt>(Address->getOperand(2))) {
          if (Idx->getZExtValue() == RegNo) {
            Prev = I->getOperand(0);
            break;
          }
        }
      }
    }
  }


  if (!Prev)
    return false;

  DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(RegNo, CallInst);

  for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
    NodeIndex srcIndex = nodeFactory.getValueNodeFor(Prev);
    if (srcIndex == AndersNodeFactory::InvalidIndex)
      srcIndex = nodeFactory.createValueNode(Prev);
    NodeIndex dstIndex = nodeFactory.getValueNodeFor(&**Post_it);
    if (dstIndex == AndersNodeFactory::InvalidIndex)
      dstIndex = nodeFactory.createValueNode(&**Post_it);
    addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
  }

  return false;
}

bool Andersen::isBlock(const llvm::Instruction *Inst, const llvm::Value *&B) {
  if (!Inst)
    return false;
  if (dyn_cast<Instruction>(Inst)) {
    std::vector<const Value *> PtsTo;
    getPointsToSet(Inst, PtsTo);

    for (std::vector<const Value *>::iterator PtsTo_it = PtsTo.begin(); PtsTo_it != PtsTo.end(); ++PtsTo_it) {
      const Instruction *I = dyn_cast<const Instruction>(*PtsTo_it);
      if (!I)
        continue;
      StackAccessPass &SAP = getAnalysis<StackAccessPass>();
      StackAccessPass::OffsetMap_t &Offsets = SAP.getOffsets(((Instruction *) (*PtsTo_it))->getParent()->getParent());
      StackAccessPass::OffsetValueListMap_t &OffsetValues = SAP.getOffsetValues(
        ((Instruction *) (*PtsTo_it))->getParent()->getParent());
      if (!Offsets[*PtsTo_it])
        continue;
      for (StackAccessPass::Int64List_t::iterator O_it = Offsets[*PtsTo_it]->begin();
           O_it != Offsets[*PtsTo_it]->end(); ++O_it) {
        for (StackAccessPass::ValueList_t::iterator V_it = OffsetValues[*O_it]->begin();
             V_it != OffsetValues[*O_it]->end(); ++V_it) {
          for (std::set<Value *>::iterator B_it = Blocks.begin(); B_it != Blocks.end(); ++B_it) {
            if (*B_it == *V_it) {
              B = *B_it;
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

void Andersen::addBlock(llvm::Value *B) {
  Blocks.insert(B);
}

bool Andersen::handleBlock(const Instruction *Call, const Value *Block) {
  std::vector<const Value *> PtsTo;
  getPointsToSet(Block, PtsTo);
  for (std::vector<const Value *>::iterator PtsTo_it = PtsTo.begin(); PtsTo_it != PtsTo.end(); ++PtsTo_it) {

    const Function *Func = cast<const Instruction>((*PtsTo_it))->getParent()->getParent();
    StackAccessPass &StackAccess = getAnalysis<StackAccessPass>();

    StackAccessPass::OffsetMap_t &Offsets = StackAccess.getOffsets(Func);
    StackAccessPass::OffsetValueListMap_t &OffsetValues = StackAccess.getOffsetValues(Func);

    StackAccessPass::Int64List_t &BlockOffsets = *Offsets[*PtsTo_it];

    for (StackAccessPass::Int64List_t::iterator BO_it = BlockOffsets.begin(); BO_it != BlockOffsets.end(); ++BO_it) {
      //Address of the function of this block has an offset of 16 to the block object
      int64_t BlockAddress = *BO_it + 16;

      StackAccessPass::ValueList_t &FunctionAddress = *OffsetValues[BlockAddress];

      for (StackAccessPass::ValueList_t::iterator FAV_it = FunctionAddress.begin();
           FAV_it != FunctionAddress.end(); ++FAV_it) {
        std::vector<const Value *> FunctionAddressCandidates;
        getPointsToSet(*FAV_it, FunctionAddressCandidates);
        for (std::vector<const Value *>::iterator FAC_it = FunctionAddressCandidates.begin();
             FAC_it != FunctionAddressCandidates.end(); ++FAC_it) {
          for (Value::const_use_iterator ITPUse_it = (*FAC_it)->use_begin();
               ITPUse_it != (*FAC_it)->use_end(); ++ITPUse_it) {
            if (Instruction *I = dyn_cast<Instruction>(ITPUse_it->getUser())) {
              if (I->getOpcode() != Instruction::IntToPtr)
                continue;
              for (Value::const_use_iterator STUse_it = I->use_begin(); STUse_it != I->use_end(); ++STUse_it) {
                if (Instruction *I2 = dyn_cast<Instruction>(STUse_it->getUser())) {
                  if (I2->getOpcode() != Instruction::Store)
                    continue;
                  if (ConstantInt *Address = dyn_cast<ConstantInt>(I2->getOperand(0))) {

                    std::string FName = getMachO().getFunctionName(Address->getZExtValue());

                    Function *F = getModule().getFunction(FName);
                    if (getCallGraph().containtsEdge(Call, FName))
                      return true;
                    getCallGraph().addCallEdge(Call, FName);

                    if (!F)
                      continue;
                    std::set<uint64_t> SPIdx;
                    SPIdx.insert(5);
                    DetectParametersPass::ParameterAccessPairSet_t StackParams = getAnalysis<DetectParametersPass>(
                      *F).getStackParameters(*F, SPIdx);

                    int64_t StackSize = StackAccess.getStackPointerValue((Instruction *) Block, false);
                    for (DetectParametersPass::ParameterAccessPairSet_t::iterator PA_it = StackParams.begin();
                         PA_it != StackParams.end(); ++PA_it) {
                      if (PA_it->second->getOpcode() == Instruction::Load) {
                        for (Value::use_iterator BU_it = PA_it->second->use_begin();
                             BU_it != PA_it->second->use_end(); ++BU_it) {
                          Instruction *BU = dyn_cast<Instruction>(BU_it->getUser());
                          if (BU->getOpcode() == Instruction::Add) {
                            ConstantInt *O = dyn_cast<ConstantInt>(BU->getOperand(1));
                            if (O && O->getZExtValue() == 8) {
                              for (Value::use_iterator BU2_it = BU->use_begin(); BU2_it != BU->use_end(); ++BU2_it) {
                                Instruction *BU2 = dyn_cast<Instruction>(BU2_it->getUser());
                                if (BU2->getOpcode() != Instruction::IntToPtr)
                                  continue;
                                for (Value::use_iterator BU3_it = BU2->use_begin();
                                     BU3_it != BU2->use_end(); ++BU3_it) {
                                  Instruction *BU3 = dyn_cast<Instruction>(BU3_it->getUser());
                                  if (!BU3 || BU3->getOpcode() != Instruction::Load)
                                    continue;
                                  for (Value::use_iterator BU4_it = BU3->use_begin();
                                       BU4_it != BU3->use_end(); ++BU4_it) {
                                    Instruction *BU4 = dyn_cast<Instruction>(BU4_it->getUser());
                                    if (!BU4 || BU4->getOpcode() != Instruction::Add)
                                      continue;
                                    ConstantInt *O2 = dyn_cast<ConstantInt>(BU4->getOperand(1));
                                    if (!O2)
                                      continue;
                                    for (Value::use_iterator BU5_it = BU4->use_begin();
                                         BU5_it != BU4->use_end(); ++BU5_it) {
                                      Instruction *BU5 = dyn_cast<Instruction>(BU5_it->getUser());
                                      if (BU5->getOpcode() != Instruction::IntToPtr)
                                        continue;
                                      for (Value::use_iterator BU6_it = BU5->use_begin();
                                           BU6_it != BU5->use_end(); ++BU6_it) {
                                        Instruction *BU6 = dyn_cast<Instruction>(BU6_it->getUser());
                                        if (!BU6 || !(BU6->getOpcode() == Instruction::Load ||
                                                      BU6->getOpcode() == Instruction::Store))
                                          continue;
                                        findSetStackParameterInstruction((Instruction *) Block,
                                                                         DetectParametersPass::ParameterAccessPair_t(
                                                                           PA_it->first, BU6), StackSize,
                                                                         O2->getZExtValue());
                                      }
                                    }
                                  }
                                }
                              }
                            }
                          }
                        }
                      }
                      findSetStackParameterInstruction((Instruction *) Block, *PA_it, StackSize);
                    }
                  }
                }
              }
            }

          }
        }
      }


    }
  }

  return false;
}


bool Andersen::isDummyHelper(const llvm::Value *val) {
  return dummyHelpers.find(val) != dummyHelpers.end();
}