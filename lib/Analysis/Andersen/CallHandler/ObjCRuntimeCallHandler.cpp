#include <llvm/IR/PatternMatch.h>
#include "llvm/Analysis/Andersen/CallHandler/ObjCRuntimeCallHandler.h"

#include "llvm/IR/Constants.h"
#include "llvm/Analysis/Andersen/Andersen.h"

#include <vector>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Analysis/Andersen/SimpleCallGraph.h>
#include <llvm/Analysis/Andersen/StackAccessPass.h>
#include <llvm/ADT/StringExtras.h>
#include <sstream>
#include "llvm/IR/InstIterator.h"

#include "llvm/Support/Debug.h"
#include "ExternalHandler.h"
#include "../../../LLVMSlicer/Languages/LLVM.h"


using namespace llvm;
using namespace llvm::ObjectiveC;

#define DEBUG_TYPE "objc_runtime"

cl::opt<bool> FastEnum("fast-enum", cl::desc("Handles Objective-C fast enum functions"), cl::init(false), cl::Hidden);

bool CallHandlerBase::isSelf(Instruction *v, Andersen *andersen) {
    //This may get called with ConstantInts...
    if (!v)
        return false;

    //When something points to a class it can't point to 'self'...
    if (v->getOpcode() == Instruction::Load &&
        PatternMatch::match(v->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt()))) {
        return false;
    }

    const Function *F = v->getParent()->getParent();

    Value *RegPtr = nullptr;
    Value *Self = nullptr;
    const BasicBlock &EntryBB = F->getEntryBlock();
    for (BasicBlock::const_iterator I_it = EntryBB.getInstList().begin();
         I_it != EntryBB.getInstList().end(); ++I_it) {
        if (I_it->getOpcode() == Instruction::GetElementPtr) {
            if (ConstantInt *Idx = dyn_cast<ConstantInt>(I_it->getOperand(2))) {
                if (Idx->getZExtValue() == 5) {
                    RegPtr = (Value*)&*I_it;
                }
            } else {
                llvm_unreachable("The index should be a constant int...");
            }
        } else if (RegPtr && I_it->getOpcode() == Instruction::Load) {
            if (I_it->getOperand(0) == RegPtr) {
                Self = (Value*)&*I_it;
                break;
            }
        }
    }


    if (!Self) {
        return false;
        llvm_unreachable("Can't find self value");
    }

    if (Self == v) {
        return true;
    }

    Function *f = v->getParent()->getParent();

    StackAccessPass *SAP = andersen->getAnalysisIfAvailable<StackAccessPass>();
    if (!SAP)
        SAP = &andersen->getAnalysis<StackAccessPass>();

    StackAccessPass::OffsetMap_t &Offsets = SAP->getOffsets(v->getParent()->getParent());

    std::vector<int64_t> StoredAtOffsets;

    for (auto &O_it : Offsets) {

        bool self_stored = false;

        for (auto &U_it : O_it.first->uses()) {
            if (const Instruction *ItoP = dyn_cast<const IntToPtrInst>(U_it.getUser())) {
                for (auto &IU_it : ItoP->uses()) {
                    if (const StoreInst *Store = dyn_cast<const StoreInst>(IU_it.getUser())) {
                        if (Store->getOperand(0) == Self) {
                            self_stored = true;
                            if (!O_it.second)
                                continue;
                            for (auto &Off_it : *O_it.second) {
                                StoredAtOffsets.push_back(Off_it);
                            }
                            break;
                        }
                    }
                }
            }
        }

        if (self_stored) {


        }
    }

    StackAccessPass::OffsetValueListMap_t &OffsetValues = SAP->getOffsetValues(f);

    std::set<const Value*> checked;

    for (size_t i = 0; i < StoredAtOffsets.size(); ++i) {
        int64_t O = StoredAtOffsets[i];
//    for (auto &O : StoredAtOffsets) {
        if (OffsetValues.find(O) == OffsetValues.end())
            continue;

        for (auto &V : *OffsetValues[O]) {
            for (auto &U_it : V->uses()) {
                if (const Instruction *ItoP = dyn_cast<const IntToPtrInst>(U_it.getUser())) {
                    for (auto &IU_it : ItoP->uses()) {
                        if (const LoadInst *Load = dyn_cast<const LoadInst>(IU_it.getUser())) {
                            if (checked.find(Load) != checked.end())
                                continue;
                            checked.insert(Load);
                            if (Load == v) {
                                return true;
                            } else {
                                for (auto &LU_it : Load->uses()) {
                                    if (const StoreInst *Store = dyn_cast<const StoreInst>(LU_it.getUser())) {
                                        if (const IntToPtrInst *StoreItoP = dyn_cast<IntToPtrInst>(Store->getOperand(1))) {
                                            if (SAP->getOffsets(f)[StoreItoP->getOperand(0)]) {
                                                StackAccessPass::Int64List_t StoreOffsets = *SAP->getOffsets(
                                                        f)[StoreItoP->getOperand(0)];
                                                StoredAtOffsets.insert(StoredAtOffsets.end(), StoreOffsets.begin(),
                                                                       StoreOffsets.end());
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
    }

    std::vector<const Value*> SelfPtsTo;
    andersen->getPointsToSet(Self, SelfPtsTo);
    std::vector<const Value*> X0PtsTo;
    andersen->getPointsToSet(v, X0PtsTo);


    for (std::vector<const Value*>::iterator Self_it = SelfPtsTo.begin(); Self_it != SelfPtsTo.end(); ++Self_it) {
        for (std::vector<const Value*>::iterator X0_it = X0PtsTo.begin(); X0_it != X0PtsTo.end(); ++X0_it) {
            if (*X0_it == *Self_it)
                return true;
        }
    }


    return false;
}

StringRef CallHandlerBase::getClassname(StringRef F) {
    assert(isObjectiveCMethod(F));
    StringRef Name = F;
    Name = Name.substr(2);
    size_t i = Name.find(" ");
    return Name.substr(0, i);
}

StringRef CallHandlerBase::getMethodname(StringRef F) {
    assert(isObjectiveCMethod(F));
    StringRef Name = F;
    size_t space = Name.find(" ");
    Name = Name.substr(space + 1);
    Name = Name.substr(0, Name.size() - 1);
    return Name;
}

bool CallHandlerBase::isObjectiveCMethod(StringRef F) {
    return (F.startswith_lower("+[") || F.startswith_lower("-["));
}


bool objcMsgSend::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    //TODO: for now we just add all instructions to the worklist. already handled ones should be skipped (check callgraph)
    andersen->addToWorklist((Instruction*)CallInst);

    DetectParametersPass::UserSet_t X0Values = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    DetectParametersPass::UserSet_t X1Values = DetectParametersPass::getRegisterValuesBeforeCall(6, CallInst);


    for (DetectParametersPass::UserSet_t::iterator X0_it = X0Values.begin(); X0_it != X0Values.end(); ++X0_it) {
        User *X0 = *X0_it;
        for (DetectParametersPass::UserSet_t::iterator X1_it = X1Values.begin(); X1_it != X1Values.end(); ++X1_it) {
            User *X1 = *X1_it;

            Instruction *X0Inst = dyn_cast<Instruction>(X0);
            Instruction *X1Inst = dyn_cast<Instruction>(X1);

            std::vector<const Value *> X0PT;
            andersen->getPointsToSet(X0, X0PT);
            std::vector<const Value *> X1PT;
            andersen->getPointsToSet(X1, X1PT);

            for (std::vector<const Value *>::iterator X0PT_it = X0PT.begin(); X0PT_it != X0PT.end(); ++X0PT_it) {

                auto handleLambda = [&](StringRef Classname, bool Meta) {
                    for (std::vector<const Value *>::iterator X1PT_it = X1PT.begin(); X1PT_it != X1PT.end(); ++X1PT_it) {
                        if (ConstantDataArray *ClassData = dyn_cast<ConstantDataArray>((Value*)*X1PT_it)) {
                            StringRef Methodname = ClassData->getAsString();
                            handleCall(Classname, Methodname, Meta, (Instruction*)CallInst, X0, X1, andersen);
                        }
                    }
                };

                StringRef Classname;
                Andersen::StringSet_t Classnames;
                bool Meta = false;
                if (ConstantDataArray *ClassData = dyn_cast<ConstantDataArray>((Value*)*X0PT_it)) {
                    Classname = ClassData->getAsString();
                    Meta = true;
                    handleLambda(Classname, Meta);
                } else if (andersen->getType((Value*)*X0PT_it, Classnames)) {
                    for (auto &className_it : Classnames) {
                        handleLambda(className_it, Meta);
                    }
                } else {
                    continue;
                }

            }


            if (isObjectiveCMethod(CallInst->getParent()->getParent()->getName()) && isSelf(dyn_cast<Instruction>(X0), andersen)) {
                if (X1PT.size()) {
                    StringRef ClassName = getClassname(CallInst->getParent()->getParent()->getName());
                    bool Meta = CallInst->getParent()->getParent()->getName().startswith("+");
                    for (std::vector<const Value *>::iterator X1PT_it = X1PT.begin(); X1PT_it != X1PT.end(); ++X1PT_it) {
                        if (ConstantDataArray *ClassData = dyn_cast<ConstantDataArray>((Value*)*X1PT_it)) {
                            StringRef Methodname = ClassData->getAsString();
                            handleCall(ClassName, Methodname, Meta, (Instruction*)CallInst, X0, X1, andersen);
                        }
                    }
                }
            }

            return true;

            StringRef ClassName;
            bool ClassMethod = false;
            if (X0Inst && X0Inst->getOpcode() == Instruction::Load) {
                uint64_t ClassAddress;
                if (PatternMatch::match(X0Inst->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(ClassAddress)))) {
                    if (andersen->getMachO().getClass(ClassAddress, ClassName)) {
                        ClassMethod = true;
                    } else {
                        //TODO: check if it points to a class
                        return false;
                    }
                }
            }

            StringRef SelectorName;
            if (X1Inst && X1Inst->getOpcode() == Instruction::Load) {
                uint64_t SelectorAddress;
                if (PatternMatch::match(X1Inst->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt(SelectorAddress)))) {
                    if (andersen->getMachO().getSelector(SelectorAddress, SelectorName)) {

                    } else {
                        assert(false);
                    }
                }
            }

            if (ClassMethod && ClassName.size()) {
                if (SelectorName == "alloc" || SelectorName == "new" || SelectorName == "allocWithZone:") {
                    StringRef AllocMethod = (StringRef("+[") + ClassName + StringRef(" ") + SelectorName + StringRef("]")).str();
                    if (!andersen->getCallGraph().containtsEdge(CallInst, AllocMethod)) {
//                        errs() << AllocMethod << "\n";
                        andersen->getCallGraph().addCallEdge(CallInst, AllocMethod);

                        DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);
                        for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
                            NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
                            if (valIndex == AndersNodeFactory::InvalidIndex)
                                valIndex = andersen->getNodeFactory().createValueNode(*X0Post_it);
                            NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*X0Post_it);
                            if (objIndex == AndersNodeFactory::InvalidIndex)
                                objIndex = andersen->getNodeFactory().createObjectNode(*X0Post_it);
                            assert(valIndex != AndersNodeFactory::InvalidIndex && objIndex != AndersNodeFactory::InvalidIndex);
//                            andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                            andersen->setType(*X0Post_it, ClassName);
                            andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                        }
                    }
                }
            } else {
                std::vector<const Value *> PtsTo;
                andersen->getPointsToSet(X0, PtsTo);

                for (unsigned i = 0; i < PtsTo.size(); ++i) {
                    const Value *P = PtsTo[i];
                    Andersen::StringSet_t C;
                    if (!andersen->getType((Value*)P, C)) {
                        continue;
                    }
                    for (auto &type_it : C) {
                        std::deque<std::string> Candidates = andersen->getMachO().getMethodCandidates(type_it, SelectorName, ClassMethod);
                        bool h = false;
                        for (std::deque<std::string>::iterator C_it = Candidates.begin(); C_it != Candidates.end(); ++C_it) {
                            if (Function *F = CallInst->getParent()->getParent()->getParent()->getFunction(*C_it)) {
                                andersen->addConstraintsForCall((Instruction*)CallInst, F);
                                h = true;
                                break;
                            } else if (CallHandlerManager::getInstance().handleFunctionCall(CallInst, *C_it, andersen)) {
                                h = true;
                                break;
                            }
                        }
                        if (!h && Candidates.size()) {
                            errs() << "Can't find method: " << Candidates.back() << "\n";
                        }
                    }

                }
             }

//            if (isObjectiveCMethod(CallInst->getParent()->getParent()->getName()) && isSelf(dyn_cast<Instruction>(X0))) {
//                std::deque<std::string> Candidates = andersen->getMachO().getMethodCandidates(getClassname((Function*)CallInst->getParent()->getParent()->getName()), SelectorName, CallInst->getParent()->getParent()->getName().startswith("+"));
//                bool h = false;
//                for (std::deque<std::string>::iterator C_it = Candidates.begin(); C_it != Candidates.end(); ++C_it) {
//                    if (Function *F = CallInst->getParent()->getParent()->getParent()->getFunction(*C_it)) {
//                        andersen->addConstraintsForCall((Instruction*)CallInst, F);
//                        h = true;
//                        break;
//                    } else if (CallHandlerManager::getInstance().handleFunctionCall(CallInst, *C_it, andersen)) {
//                        h = true;
//                        break;
//                    }
//                }
//                if (!h && Candidates.size()) {
//                    errs() << "Can't find method: " << Candidates.back() << "\n";
//                }
//            }
        }
    }

    return true;
}

void objcMsgSend::handleCall(StringRef ClassName, StringRef MethodName, bool Meta, Instruction *CallInst, Value *X0, Value *X1, Andersen *andersen) {

    if (Meta && (MethodName == "alloc" || MethodName == "new" || MethodName == "allocWithZone:")) {
        StringRef AllocMethod = (StringRef("+[") + ClassName + StringRef(" ") + MethodName + StringRef("]")).str();
        if (!andersen->getCallGraph().containtsEdge(CallInst, AllocMethod)) {
//                        errs() << AllocMethod << "\n";
            andersen->getCallGraph().addCallEdge(CallInst, AllocMethod);

            DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);
            for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
                NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
                if (valIndex == AndersNodeFactory::InvalidIndex)
                    valIndex = andersen->getNodeFactory().createValueNode(*X0Post_it);
                NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*X0Post_it);
                if (objIndex == AndersNodeFactory::InvalidIndex)
                    objIndex = andersen->getNodeFactory().createObjectNode(*X0Post_it);
                assert(valIndex != AndersNodeFactory::InvalidIndex && objIndex != AndersNodeFactory::InvalidIndex);
//                            andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                andersen->setType(*X0Post_it, ClassName);
                andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
            }
        }
        return;
    }

    bool HandledCall = false;
    std::deque<std::string> Candidates = andersen->getMachO().getMethodCandidates(ClassName.str(), MethodName.str(), Meta);

    for (std::deque<std::string>::iterator C_it = Candidates.begin(); C_it != Candidates.end(); ++C_it) {
        if (Function *F = CallInst->getParent()->getParent()->getParent()->getFunction(*C_it)) {
            andersen->addConstraintsForCall((Instruction *) CallInst, F);
            HandledCall = true;
            break;
        } else if (CallHandlerManager::getInstance().handleFunctionCall(CallInst, *C_it, andersen)) {
            HandledCall = true;
            break;
        }
    }

    if (!HandledCall && Candidates.size()) {
        andersen->addUnhandled(Candidates.front(), CallInst);
    }
}



bool objcInit::shouldHandleCall(std::string &F) {
//    return F.startswith("-") && (F.find("init") != StringRef::npos);
    return true;
}


bool objcInit::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst, F))
        return true;
    DetectParametersPass::UserSet_t X0Values = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);

    if (!X0Values.size() || !X0Post.size())
        return false;

//    errs() << F << "\n";

    andersen->getCallGraph().addCallEdge(CallInst, F);

    for (DetectParametersPass::UserSet_t::iterator X0_it = X0Values.begin(); X0_it != X0Values.end(); ++X0_it) {
        for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
            NodeIndex srcIndex = andersen->getNodeFactory().getValueNodeFor(*X0_it);
            if (srcIndex == AndersNodeFactory::InvalidIndex)
                srcIndex = andersen->getNodeFactory().createValueNode(*X0_it);
            NodeIndex dstIndex = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
            if (dstIndex == AndersNodeFactory::InvalidIndex)
                dstIndex = andersen->getNodeFactory().createValueNode(*X0Post_it);
            andersen->addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
        }
    }
    return true;
}

bool objcARC::shouldHandleCall(std::string &F) {
    return false;
    return F == "objc_retain" ||
           F == "objc_release" ||
            F == "objc_retainAutoreleasedReturnValue";
}

bool objcARC::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst, F))
        return true;
    DetectParametersPass::UserSet_t X0Values = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);

    if (!X0Values.size() || !X0Post.size())
        return false;

//    errs() << F << "\n";

    andersen->getCallGraph().addCallEdge(CallInst, F);

    for (DetectParametersPass::UserSet_t::iterator X0_it = X0Values.begin(); X0_it != X0Values.end(); ++X0_it) {
        for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
            NodeIndex srcIndex = andersen->getNodeFactory().getValueNodeFor(*X0_it);
            if (srcIndex == AndersNodeFactory::InvalidIndex)
                srcIndex = andersen->getNodeFactory().createValueNode(*X0_it);
            NodeIndex dstIndex = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
            if (dstIndex == AndersNodeFactory::InvalidIndex)
                dstIndex = andersen->getNodeFactory().createValueNode(*X0Post_it);
            andersen->addConstraint(AndersConstraint::COPY, dstIndex, srcIndex);
        }
    }
    return true;
}


bool dispatchBlock::shouldHandleCall(std::string &F) {
    if (F == "dispatch_async" ||
            F == "dispatch_sync" ||
            F == "dispatch_once" ||
            F == "dispatch_after" ||
            F == "-[NSOperationQueue addOperationWithBlock:]")
        return true;
    return false;
}
bool dispatchBlock::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    std::unique_lock<std::mutex> lock(handlerLock);
    bool handled = false;

    andersen->addToWorklist((Instruction*)CallInst);

    uint64_t blockReg = 0;
    if (F == "dispatch_async" || F == "dispatch_sync" || F == "dispatch_once") {
        blockReg = 6;
    } else if (F == "dispatch_after" || F == "-[NSOperationQueue addOperationWithBlock:]") {
        blockReg = 7;
    }
    assert(blockReg);

    DetectParametersPass::UserSet_t Block = DetectParametersPass::getRegisterValuesBeforeCall(blockReg, CallInst);
    for (DetectParametersPass::UserSet_t::iterator Block_it = Block.begin(); Block_it != Block.end(); ++Block_it) {

        std::vector<const Value*> PtsTo;
        andersen->getPointsToSet(*(Block_it), PtsTo);
        for (std::vector<const Value*>::iterator PtsTo_it = PtsTo.begin(); PtsTo_it != PtsTo.end(); ++PtsTo_it) {
//            (*Block_it)->dump();

            const Value *loc = andersen->getNodeFactory().getLocation(*PtsTo_it);
            if (const ConstantInt *constant = dyn_cast<const ConstantInt>(loc)) {
                int64_t BlockAddressLocation = constant->getZExtValue() + 16;
                uint64_t BlockAddress = andersen->getMachO().getRAWData<uint64_t>(BlockAddressLocation);

                std::string FName = andersen->getMachO().getFunctionName(BlockAddress);

                if (andersen->getCallGraph().containtsEdge(CallInst, FName))
                    return true;
                andersen->getCallGraph().addCallEdge(CallInst, FName);
                handled = true;

                continue;
            }

            Andersen::FunctionIntPairSet_t &fs = andersen->getStackOffsets()[*PtsTo_it];
            StackAccessPass &StackAccess = andersen->getAnalysis<StackAccessPass>();

            for (auto &fs_it : fs) {
                StackAccessPass::OffsetValueListMap_t &OffsetValues = StackAccess.getOffsetValues((Function*)fs_it.first);

                int64_t BlockAddress = fs_it.second + 16;

                if (!OffsetValues[BlockAddress])
                    continue;

                StackAccessPass::ValueList_t &FunctionAddress = *OffsetValues[BlockAddress];

                for (StackAccessPass::ValueList_t::iterator FAV_it = FunctionAddress.begin(); FAV_it != FunctionAddress.end(); ++FAV_it) {
                    std::vector<const Value*> FunctionAddressCandidates;
                    andersen->getPointsToSet(*FAV_it, FunctionAddressCandidates);
                    for (std::vector<const Value*>::iterator FAC_it = FunctionAddressCandidates.begin(); FAC_it != FunctionAddressCandidates.end(); ++FAC_it) {
                        Andersen::FunctionIntPairSet_t blockStackOffsets = andersen->getStackOffsets()[*FAC_it];
                        for (auto &blockOffset_it : blockStackOffsets) {
                            if (!StackAccess.getOffsetValues((Function*)blockOffset_it.first)[blockOffset_it.second])
                                continue;
                            StackAccessPass::ValueList_t &Vals = *StackAccess.getOffsetValues((Function*)blockOffset_it.first)[blockOffset_it.second];
                            for (auto &V_it : Vals) {
                                for (Value::const_use_iterator ITPUse_it = V_it->use_begin();
                                     ITPUse_it != V_it->use_end(); ++ITPUse_it) {
                                    if (Instruction *I = dyn_cast<Instruction>(ITPUse_it->getUser())) {
                                        if (I->getOpcode() != Instruction::IntToPtr)
                                            continue;
                                        for (Value::const_use_iterator STUse_it = I->use_begin();
                                             STUse_it != I->use_end(); ++STUse_it) {
                                            if (Instruction *I2 = dyn_cast<Instruction>(STUse_it->getUser())) {
                                                if (I2->getOpcode() != Instruction::Store)
                                                    continue;
                                                if (ConstantInt *Address = dyn_cast<ConstantInt>(I2->getOperand(0))) {
//                                                    errs() << utohexstr(Address->getZExtValue()) << "\n";
                                                    std::string FName = andersen->getMachO().getFunctionName(
                                                            Address->getZExtValue());
//                                                    errs() << FName << "\n";
                                                    Function *F = andersen->getModule().getFunction(FName);
                                                    if (andersen->getCallGraph().containtsEdge(CallInst, FName))
                                                        return true;
                                                    andersen->getCallGraph().addCallEdge(CallInst, FName);
                                                    handled = true;

                                                    if (!F)
                                                        continue;
                                                    std::set<uint64_t> SPIdx;
                                                    SPIdx.insert(5);
                                                    DetectParametersPass::ParameterAccessPairSet_t StackParams = andersen->getAnalysis<DetectParametersPass>().getStackParameters(*F, SPIdx);

//                                                    int64_t StackSize = StackAccess.getStackPointerValue(
//                                                            (Instruction *) *Block_it, false);
                                                    int64_t StackSize = fs_it.second;
                                                    for (DetectParametersPass::ParameterAccessPairSet_t::iterator PA_it = StackParams.begin();
                                                         PA_it != StackParams.end(); ++PA_it) {
                                                        Instruction *address = andersen->findSetStackParameterInstruction(
                                                                (Instruction *) CallInst, *PA_it, StackSize);
                                                        if (!address)
                                                            continue;
                                                        andersen->findAliases(address, true, 5);
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


            }

            continue;

            Function *Func = (Function*)((Instruction*)(*Block_it))->getParent()->getParent();

            StackAccessPass::OffsetMap_t &Offsets = StackAccess.getOffsets(Func);
            StackAccessPass::OffsetValueListMap_t &OffsetValues = StackAccess.getOffsetValues(Func);

            StackAccessPass::Int64List_t &BlockOffsets = *Offsets[*Block_it];

            for (StackAccessPass::Int64List_t::iterator BO_it = BlockOffsets.begin(); BO_it != BlockOffsets.end(); ++BO_it) {
                int64_t BlockAddress = *BO_it + 16;

                StackAccessPass::ValueList_t &FunctionAddress = *OffsetValues[BlockAddress];

                for (StackAccessPass::ValueList_t::iterator FAV_it = FunctionAddress.begin(); FAV_it != FunctionAddress.end(); ++FAV_it) {
                    std::vector<const Value*> FunctionAddressCandidates;
                    andersen->getPointsToSet(*FAV_it, FunctionAddressCandidates);
                    for (std::vector<const Value*>::iterator FAC_it = FunctionAddressCandidates.begin(); FAC_it != FunctionAddressCandidates.end(); ++FAC_it) {
                        for (Value::const_use_iterator ITPUse_it = (*FAC_it)->use_begin(); ITPUse_it != (*FAC_it)->use_end(); ++ITPUse_it) {
                            if (Instruction *I = dyn_cast<Instruction>(ITPUse_it->getUser())) {
                                if (I->getOpcode() != Instruction::IntToPtr)
                                    continue;
                                for (Value::const_use_iterator STUse_it = I->use_begin(); STUse_it != I->use_end(); ++ STUse_it) {
                                    if (Instruction *I2 = dyn_cast<Instruction>(STUse_it->getUser())) {
                                        if (I2->getOpcode() != Instruction::Store)
                                            continue;
                                        if (ConstantInt *Address = dyn_cast<ConstantInt>(I2->getOperand(0))) {
                                            errs() << utohexstr(Address->getZExtValue()) << "\n";
                                            std::string FName = andersen->getMachO().getFunctionName(Address->getZExtValue());
                                            errs() << FName << "\n";
                                            Function *F = andersen->getModule().getFunction(FName);
                                            if (andersen->getCallGraph().containtsEdge(CallInst, FName))
                                                return true;
                                            andersen->getCallGraph().addCallEdge(CallInst, FName);

                                            if (!F)
                                                continue;
                                            std::set<uint64_t> SPIdx;
                                            SPIdx.insert(5);
                                            DetectParametersPass::ParameterAccessPairSet_t StackParams = andersen->getAnalysis<DetectParametersPass>(*F).getStackParameters(*F, SPIdx);

                                            int64_t StackSize = StackAccess.getStackPointerValue((Instruction*)*Block_it, false);
                                            for (DetectParametersPass::ParameterAccessPairSet_t::iterator PA_it = StackParams.begin(); PA_it != StackParams.end(); ++PA_it) {
                                                andersen->findSetStackParameterInstruction((Instruction*)CallInst, *PA_it, StackSize);
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

    }



    return handled;
}

bool retainBlock::shouldHandleCall(std::string &F) {
    if (F == "objc_retainBlock")
        return true;
    return false;
}
bool retainBlock::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    andersen->addToWorklist((Instruction*)CallInst);

    DetectParametersPass::UserSet_t Block = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    for (DetectParametersPass::UserSet_t::iterator Block_it = Block.begin(); Block_it != Block.end(); ++Block_it) {
        std::vector<const Value*> PtsTo;
        andersen->getPointsToSet(*(Block_it), PtsTo);
        for (std::vector<const Value*>::iterator PtsTo_it = PtsTo.begin(); PtsTo_it != PtsTo.end(); ++PtsTo_it) {
            andersen->addBlock((Value*)*PtsTo_it);
        }
    }
    if (Preserved.find(CallInst) == Preserved.end()) {
        Preserved.insert(CallInst);
        andersen->preserveRegisterValue((Instruction *) CallInst, 5);
    }
    return true;
}

bool specialAllocs::shouldHandleCall(std::string &F) {
    return false;
}
bool specialAllocs::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    return false;
}

bool ClassHandler::shouldHandleCall(std::string &F) {
    if (isObjectiveCMethod(F) && getMethodname(F) == "class")
        return true;
    return false;
}
bool ClassHandler::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst, F)) {
        return true;
    }
    andersen->getCallGraph().addCallEdge(CallInst, F);
    DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);
    ConstantDataArray *ClassName = (ConstantDataArray*)ConstantDataArray::getString(getGlobalContext(), getClassname(F), false);
    NodeIndex DO = andersen->getNodeFactory().getObjectNodeFor(ClassName);
    if (DO == AndersNodeFactory::InvalidIndex) {
        DO = andersen->getNodeFactory().createObjectNode(ClassName);
    }
    for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
        NodeIndex DV = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
        if (DV == AndersNodeFactory::InvalidIndex)
            DV = andersen->getNodeFactory().createValueNode(*X0Post_it);
        andersen->addConstraint(AndersConstraint::ADDR_OF, DV, DO);
    }
    return true;
}

bool ExternalHandler::shouldHandleCall(std::string &F) {
    if (llvm::pointsto::canHandleCall(F) && isObjectiveCMethod(F)) {
        return true;
    }
    if (llvm::pointsto::canHandleCall(F))
        return true;
    return false;
}
bool ExternalHandler::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (!andersen->getCallGraph().containtsEdge(CallInst, F)) {
//                        errs() << AllocMethod << "\n";


        if (llvm::pointsto::handleCall((Instruction *) CallInst, andersen, F)) {
            andersen->getCallGraph().addCallEdge(CallInst, F);
        } else {
            return false;
        }
    }
    return true;
}

bool MsgSendSuper::shouldHandleCall(std::string &F) {
    if (F == "objc_msgSendSuper2") {
        return true;
    }
    return false;
}
bool MsgSendSuper::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    andersen->addToWorklist((Instruction*)CallInst);

    DetectParametersPass::UserSet_t PreX0 = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    DetectParametersPass::UserSet_t PreX1 = DetectParametersPass::getRegisterValuesBeforeCall(6, CallInst);

    DetectParametersPass::UserSet_t PreX0Replace;

    typedef std::tuple<std::string, std::string, bool> CallInfos_t;
    std::vector<CallInfos_t> calls;

    for (auto &pre_it : PreX0) {
        std::vector<const Value*> PtsTo;
        andersen->getPointsToSet(pre_it, PtsTo);
        for (auto &pts_it : PtsTo) {
            bool cond1 = false;
            for (BasicBlock::const_reverse_iterator I_it = CallInst->getParent()->rbegin(); I_it != CallInst->getParent()->rend(); ++I_it) {
                if (&*I_it == CallInst) {
                    cond1 = true;
                }
                if (!cond1) {
                    continue;
                }
                if (const StoreInst *store = dyn_cast<const StoreInst>(&*I_it)) {
                    if (const IntToPtrInst *ItoP = dyn_cast<const IntToPtrInst>(store->getOperand(1))) {
                        if (andersen->pointsTo(ItoP, pts_it)) {
                            if (!andersen->getAnalysis<StackAccessPass>().getOffsets((Function*)ItoP->getParent()->getParent())[ItoP->getOperand(0)]) {
                                llvm_unreachable("'self' not stored on stack???");
                            }

                            StackAccessPass::Int64List_t &Offsets = *andersen->getAnalysis<StackAccessPass>().getOffsets((Function*)ItoP->getParent()->getParent())[ItoP->getOperand(0)];

                            bool cond2 = false;
                            for (BasicBlock::const_iterator I2_it = CallInst->getParent()->begin(); I2_it != CallInst->getParent()->end() && &*I2_it != CallInst; ++I2_it) {
                                if (&*I2_it == store) {
                                    cond2 = true;
                                }
                                if (!cond2) {
                                    continue;
                                }
                                if (const StoreInst *store2 = dyn_cast<const StoreInst>(&*I2_it)) {
                                    if (const IntToPtrInst *ItoP2 = dyn_cast<const IntToPtrInst>(store2->getOperand(1))) {
                                        if (!andersen->getAnalysis<StackAccessPass>().getOffsets(
                                                (Function *) ItoP->getParent()->getParent())[ItoP2->getOperand(0)]) {
                                            continue;
                                        }
                                        StackAccessPass::Int64List_t &Offsets2 = *andersen->getAnalysis<StackAccessPass>().getOffsets(
                                                (Function *) ItoP2->getParent()->getParent())[ItoP2->getOperand(0)];

                                        bool intersect = false;
                                        for (auto O1 : Offsets) {
                                            for (auto O2 : Offsets2) {
                                                if (O1 == (O2 - 8)) {
                                                    intersect = true;
                                                    break;
                                                }
                                            }
                                        }
                                        if (intersect) {
                                            PreX0Replace.insert((Instruction*)store);
                                            std::vector<const Value*> Super;
                                            andersen->getPointsToSet(store2->getOperand(0), Super);

                                            for (auto &super_it : Super) {
                                                if (const ConstantDataArray *superDataArray = dyn_cast<const ConstantDataArray>(super_it)) {
                                                    for (auto &preX1_it : PreX1) {
                                                        std::vector<const Value*> Sel;
                                                        andersen->getPointsToSet(preX1_it, Sel);
                                                        for (auto &sel_it : Sel) {
                                                            if (const ConstantDataArray *selector = dyn_cast<const ConstantDataArray>(sel_it)) {
                                                                std::string methodName = "-[" + superDataArray->getAsString().str() + " " + selector->getAsString().str() + "]";

                                                                CallInfos_t infos;
                                                                std::get<0>(infos) = superDataArray->getAsString();
                                                                std::get<1>(infos) = selector->getAsString().str();

                                                                //FIXME: check for meta class
                                                                std::get<2>(infos) = false;
                                                                calls.push_back(infos);

                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            break;
                        }
                    }
                }
            }
        }
    }

    if (PreX0Replace.size()) {
        DetectParametersPass::setSpecialPreSet(CallInst, 5, PreX0Replace);
    }

    for (auto &infos : calls) {
        bool HandledCall = false;
        std::deque<std::string> Candidates = andersen->getMachO().getMethodCandidates(std::get<0>(infos), std::get<1>(infos), std::get<2>(infos));

        for (std::deque<std::string>::iterator C_it = Candidates.begin(); C_it != Candidates.end(); ++C_it) {
            StringRef MethodName = CallHandlerBase::getMethodname(*C_it);
            if (MethodName == "allocWithZone:") {
                if (!isObjectiveCMethod(CallInst->getParent()->getParent()->getName()))
                    continue;
                StringRef ClassName = CallHandlerBase::getClassname(CallInst->getParent()->getParent()->getName());


                StringRef AllocMethod = (StringRef("+[") + ClassName + StringRef(" ") + MethodName + StringRef("]")).str();
                if (!andersen->getCallGraph().containtsEdge(CallInst, AllocMethod)) {
//                        errs() << AllocMethod << "\n";
                    andersen->getCallGraph().addCallEdge(CallInst, AllocMethod);

                    DetectParametersPass::UserSet_t X0Post = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);
                    for (DetectParametersPass::UserSet_t::iterator X0Post_it = X0Post.begin(); X0Post_it != X0Post.end(); ++X0Post_it) {
                        NodeIndex valIndex = andersen->getNodeFactory().getValueNodeFor(*X0Post_it);
                        if (valIndex == AndersNodeFactory::InvalidIndex)
                            valIndex = andersen->getNodeFactory().createValueNode(*X0Post_it);
                        NodeIndex objIndex = andersen->getNodeFactory().getObjectNodeFor(*X0Post_it);
                        if (objIndex == AndersNodeFactory::InvalidIndex)
                            objIndex = andersen->getNodeFactory().createObjectNode(*X0Post_it);
                        assert(valIndex != AndersNodeFactory::InvalidIndex && objIndex != AndersNodeFactory::InvalidIndex);
//                            andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                        andersen->setType(*X0Post_it, ClassName);
                        andersen->addConstraint(AndersConstraint::ADDR_OF, valIndex, objIndex);
                    }
                    HandledCall = true;
                    continue;
                }
            }
            if (Function *F = CallInst->getParent()->getParent()->getParent()->getFunction(*C_it)) {
                andersen->addConstraintsForCall((Instruction *) CallInst, F);
                HandledCall = true;
                break;
            } else if (CallHandlerManager::getInstance().handleFunctionCall(CallInst, *C_it, andersen)) {
                HandledCall = true;
                break;
            }
        }

        if (!HandledCall && Candidates.size()) {
            std::unique_lock<std::mutex> lock(andersen->getOutputLock());
            andersen->addUnhandled(Candidates.front(), CallInst);
        }
    }

    return true;
}


bool CopyProperty::shouldHandleCall(std::string &F) {
    if (F == "objc_setProperty_nonatomic_copy") {
        return true;
    }
    return false;
}
bool CopyProperty::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    andersen->addToWorklist((Instruction*)CallInst);

    if (!andersen->getCallGraph().containtsEdge(CallInst, F)) {
//        andersen->getCallGraph().addCallEdge(CallInst, F);
    }
    DetectParametersPass::UserSet_t dst = DetectParametersPass::getRegisterValuesBeforeCall(8, CallInst);
    for (auto &pre : DetectParametersPass::getRegisterValuesBeforeCall(7, CallInst)) {
        std::vector<const Value*> prePts;
        andersen->getPointsToSet(pre, prePts);
        for (auto &pts : prePts) {
            if (CallInst->getParent()->getParent()->getName() == "-[FormViewController setCancelPressed:]") {
                assert(true);
            }
            Andersen::StringSet_t types;
            if (andersen->getType((Value*)pts, types)) {
                for (auto &type : types) {
                    std::string copyFunction = "-[" + type + " copy]";
                    if (andersen->getCallGraph().containtsEdge(CallInst, copyFunction)) {
                        continue;
                    }
                    andersen->getCallGraph().addCallEdge(CallInst, copyFunction);

                    Value *dummy = andersen->getNodeFactory().createDummy(andersen->getModule());
                    andersen->setType(dummy, type);

                    NodeIndex objIdx = andersen->getNodeFactory().createObjectNode(dummy);
                    NodeIndex valIdx = andersen->getNodeFactory().createValueNode(dummy);

                    andersen->addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);

                    for (auto &d : dst) {
                        NodeIndex dstIdx = andersen->getNodeFactory().getValueNodeFor(d);
                        assert(dstIdx != AndersNodeFactory::InvalidIndex);
                        andersen->addConstraint(AndersConstraint::STORE, dstIdx, valIdx);

                        NodeIndex dummyLoad = andersen->getNodeFactory().getValueNodeFor(CallInst);
                        if (dummyLoad == AndersNodeFactory::InvalidIndex)
                            dummyLoad = andersen->getNodeFactory().createValueNode(CallInst);
                        andersen->addConstraint(AndersConstraint::LOAD, dummyLoad, dstIdx);
                    }
                }
            }
        }
    }
    return true;
}



bool NSArray::shouldHandleCall(std::string &F) {
    if (F == "+[NSArray arrayWithObjects:count:]"){
        return FastEnum && true;
    }
    if (F == "-[NSArray countByEnumeratingWithState:objects:count:]")
        return FastEnum && true;
    return false;
}
bool NSArray::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {

    andersen->addToWorklist((Instruction*)CallInst);
    if (andersen->getCallGraph().containtsEdge(CallInst, F))
        return true;
    andersen->getCallGraph().addCallEdge(CallInst, F);

    if (CallHandlerBase::getMethodname(F) == "countByEnumeratingWithState:objects:count:") {
        handleFastEnum(CallInst, andersen);
        return true;
    }

    DetectParametersPass::UserSet_t returnValues = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);
    if (returnValues.size() != 1) {
        errs() << "Can't have more than one return value here...\n";
        llvm_unreachable("");
    }

    Value *returnValue = *(returnValues.begin());
    NodeIndex arrayObjIdx = andersen->getNodeFactory().getObjectNodeFor(returnValue);
    if (arrayObjIdx == AndersNodeFactory::InvalidIndex)
        arrayObjIdx = andersen->getNodeFactory().createObjectNode(returnValue);
    NodeIndex arrayValIdx = andersen->getNodeFactory().getValueNodeFor(returnValue);
    if (arrayValIdx == AndersNodeFactory::InvalidIndex)
        arrayValIdx = andersen->getNodeFactory().createValueNode(returnValue);

    andersen->addConstraint(AndersConstraint::ADDR_OF, arrayValIdx, arrayObjIdx);
    andersen->setType(returnValue, "NSArray");

    DetectParametersPass::UserSet_t objects = DetectParametersPass::getRegisterValuesBeforeCall(7, CallInst);
    DetectParametersPass::UserSet_t numObjects = DetectParametersPass::getRegisterValuesBeforeCall(8, CallInst);

    StackAccessPass &SAP = andersen->getAnalysis<StackAccessPass>();

    StackAccessPass::OffsetMap_t &stackOffsets = SAP.getOffsets((Function*)CallInst->getParent()->getParent());
    StackAccessPass::OffsetValueListMap_t &stackOffsetValues = SAP.getOffsetValues((Function*)CallInst->getParent()->getParent());

    for (auto &objectParam : objects) {
        //The passed objects should all be stored on the stack in this case
        if (stackOffsets.find(objectParam) != stackOffsets.end()) {
            for (auto &num : numObjects) {
                const ConstantInt *constNumObjects = dyn_cast<const ConstantInt>(num);

                //We can only handle this correct if this number is static
                //TODO: probably add the first object anyways.
                if (!constNumObjects)
                    continue;

                for (auto &startStackOffset : *stackOffsets[objectParam]) {
                    for (unsigned stackOffset = 0; stackOffset < constNumObjects->getZExtValue(); ++stackOffset) {
                        if (stackOffsetValues.find(startStackOffset + (8 * stackOffset)) == stackOffsetValues.end())
                            continue;
                        for (auto &offsetValue : *stackOffsetValues[startStackOffset + (8 * stackOffset)]) {
                            NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(offsetValue);
                            if (valIdx == AndersNodeFactory::InvalidIndex)
                                valIdx = andersen->getNodeFactory().createValueNode(offsetValue);
                            //COPY because the object is already stored on the stack (STORE constraint)
                            //instead of a LOAD-STORE construct COPY works as well
                            andersen->addConstraint(AndersConstraint::COPY, arrayValIdx, valIdx);
                        }
                    }
                }
            }
        }
    }

    return false;
}

void NSArray::handleFastEnum(const Instruction *CallInst, Andersen *andersen)
{
    DetectParametersPass::UserSet_t contextValues = DetectParametersPass::getRegisterValuesBeforeCall(7, CallInst);
    DetectParametersPass::UserSet_t arrayValues = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);

    StackAccessPass &SAP = andersen->getAnalysis<StackAccessPass>();

    StackAccessPass::OffsetMap_t &stackOffsets = SAP.getOffsets((Function*)CallInst->getParent()->getParent());
    StackAccessPass::OffsetValueListMap_t &stackOffsetValues = SAP.getOffsetValues((Function*)CallInst->getParent()->getParent());

    for (auto &array : arrayValues) {

        NodeIndex arrayValIdx = andersen->getNodeFactory().getValueNodeFor(array);
        if (arrayValIdx == AndersNodeFactory::InvalidIndex)
            arrayValIdx = andersen->getNodeFactory().createValueNode(array);

        for (auto &contextValue : contextValues) {
            if (stackOffsets.find(contextValue) != stackOffsets.end()) {
                for (uint64_t stackOffset : *stackOffsets[contextValue]) {
                    //The passed 'context' struct has this pointer at offset 8
                    uint64_t objectsStart = stackOffset + 8;

                    if (stackOffsetValues.find(objectsStart) == stackOffsetValues.end())
                        continue;

                    //'Forward pattern matching'
                    for (auto &offsetValue : *stackOffsetValues[objectsStart]) {
                        for (auto offsetUser : offsetValue->users()) {
                            const IntToPtrInst *offsetUserInst = dyn_cast<const IntToPtrInst>(offsetUser);
                            if (!offsetUserInst)
                                continue;
                            for (auto iToPUser : offsetUserInst->users()) {
                                const LoadInst *iToPUserInst = dyn_cast<const LoadInst>(iToPUser);
                                if (!iToPUserInst)
                                    continue;
                                for (auto loadUser : iToPUser->users()) {
                                    const AddOperator *loadUserInst = dyn_cast<const AddOperator>(loadUser);
                                    if (!loadUserInst)
                                        continue;
                                    ConstantInt *shl = nullptr;
                                    if (!PatternMatch::match(loadUserInst->getOperand(1),
                                                             PatternMatch::m_Shl(PatternMatch::m_Value(),
                                                                                 PatternMatch::m_ConstantInt())) ||
                                        (shl && shl->getZExtValue() != 3))
                                        continue;
                                    for (auto addUser : loadUserInst->users()) {
                                        const IntToPtrInst *addUserInst = dyn_cast<const IntToPtrInst>(addUser);
                                        if (!addUserInst)
                                            continue;
                                        for (auto iToP2User : addUserInst->users()) {
                                            const LoadInst *loadFromArray = dyn_cast<const LoadInst>(iToP2User);
                                            if (!loadFromArray)
                                                continue;
                                            NodeIndex loadValIdx = andersen->getNodeFactory().getValueNodeFor(loadFromArray);
                                            if (loadValIdx == AndersNodeFactory::InvalidIndex)
                                                loadValIdx = andersen->getNodeFactory().createValueNode(loadFromArray);
                                            andersen->addConstraint(AndersConstraint::LOAD, loadValIdx, arrayValIdx);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                //TODO: but usually this should not happen....
            }
        }
    }
}

bool UIControlTarget::shouldHandleCall(std::string &F) {
    if (F == "-[UIControl addTarget:action:forControlEvents:]"){
        return true;
    }
    return false;
}
bool UIControlTarget::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
//    errs() << CallInst->getParent()->getParent()->getName() << "\n";
    if (andersen->getCallGraph().containtsEdge(CallInst, F))
        return true;
    andersen->getCallGraph().addCallEdge(CallInst, F);

    const Module *module = CallInst->getParent()->getParent()->getParent();

    DetectParametersPass::UserSet_t controls = DetectParametersPass::getRegisterValuesBeforeCall(5, CallInst);
    DetectParametersPass::UserSet_t targets = DetectParametersPass::getRegisterValuesBeforeCall(7, CallInst);
    DetectParametersPass::UserSet_t selectors = DetectParametersPass::getRegisterValuesBeforeCall(8, CallInst);

    Andersen::StringSet_t controlTypes;

    auto setType = [andersen](const Function *fun, uint64_t regNo, std::string type) {
        for (auto &inst : fun->getEntryBlock()) {
            const LoadInst *load = dyn_cast<const LoadInst>(&inst);
            if (!load)
                continue;
            const GetElementPtrInst *getElementPtrInst = dyn_cast<const GetElementPtrInst>(load->getOperand(0));
            if (!getElementPtrInst)
                continue;
            const ConstantInt *idx = dyn_cast<const ConstantInt>(getElementPtrInst->getOperand(2));
            if (!idx || idx->getZExtValue() != regNo)
                continue;

            NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(&inst);
            if (valIdx == AndersNodeFactory::InvalidIndex)
                valIdx = andersen->getNodeFactory().createValueNode(&inst);
            NodeIndex objIdx = andersen->getNodeFactory().getObjectNodeFor(&inst);
            if (objIdx == AndersNodeFactory::InvalidIndex)
                objIdx = andersen->getNodeFactory().createObjectNode(&inst);
            andersen->addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
            andersen->setType((Value*)&inst, type);
            break;
        }
    };

    for (auto &control : controls) {
        std::vector<const Value*> controlPtsToSet;
        andersen->getPointsToSet(control, controlPtsToSet);
        for (auto &controlPtsTo : controlPtsToSet) {
            Andersen::StringSet_t tmpTypes;
            andersen->getType((Value*)controlPtsTo, tmpTypes);
            controlTypes.insert(tmpTypes.begin(), tmpTypes.end());
        }
    }

    for (auto &target : targets) {
        std::vector<const Value*> targetPtsToSet;
        andersen->getPointsToSet(target, targetPtsToSet);
        for (auto &targetPtsTo : targetPtsToSet) {
            Andersen::StringSet_t targetTypes;
            andersen->getType((Value*)targetPtsTo, targetTypes);
            for (auto &targetType : targetTypes) {
                for (auto &selector : selectors) {
                    std::vector<const Value*> selectorPtsToSet;
                    andersen->getPointsToSet(selector, selectorPtsToSet);
                    for (auto &selectorPtsTo : selectorPtsToSet) {
                        const ConstantDataArray *selectorName = dyn_cast<const ConstantDataArray>(selectorPtsTo);
                        if (!selectorName)
                            continue;

                        std::stringstream ss;
                        ss << "-[";
                        ss << targetType;
                        ss << " ";
                        ss << selectorName->getAsString().str();
                        ss << "]";
                        std::string targetMethod = ss.str();

                        const Function *targetFun = module->getFunction(targetMethod);
                        if (!targetFun)
                            continue;
                        for (auto &controlType : controlTypes) {
                            setType(targetFun, 7, controlType);
                        }
                    }
                }
            }
        }
    }

    return false;
}

bool UIAppDelegate::shouldHandleCall(std::string &F) {
    if (F == "-[UIApplication delegate]"){
        return true;
    }
    return false;
}
bool UIAppDelegate::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst ,F))
        return true;
    andersen->getCallGraph().addCallEdge(CallInst, F);

    DetectParametersPass::UserSet_t delegateRegs = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);

    for (auto &c : andersen->getMachO().getClasses()) {
        if (!c.second || c.second->getType() != ObjectiveC::Initialized)
            continue;
        for (auto &p : ((ObjectiveC::Class*)c.second.get())->getProtocolList()) {
            if (p == "UIApplicationDelegate") {

                for (auto delegateReg : delegateRegs) {
                    NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(delegateReg);
                    if (valIdx == AndersNodeFactory::InvalidIndex)
                        valIdx = andersen->getNodeFactory().createValueNode(delegateReg);
                    NodeIndex objIdx = andersen->getNodeFactory().getObjectNodeFor(delegateReg);
                    if (objIdx == AndersNodeFactory::InvalidIndex)
                        objIdx = andersen->getNodeFactory().createObjectNode(delegateReg);
                    andersen->addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
                    andersen->setType(delegateReg, c.second->getClassName().str());
                }
            }
        }
    }
    return true;
}

bool NSUserDefaults::shouldHandleCall(std::string &F) {
    if (F == "+[NSUserDefaults standardUserDefaults]"){
        return true;
    }
    return false;
}
bool NSUserDefaults::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst, F)) {
        return true;
    }
    andersen->getCallGraph().addCallEdge(CallInst, F);

    if (!singleton) {
        singleton = andersen->getNodeFactory().createDummy(*(Module*)CallInst->getParent()->getParent()->getParent());
        andersen->setType(singleton, "NSUserDefaults");
    }

    DetectParametersPass::UserSet_t returnRegs = DetectParametersPass::getRegisterValuesAfterCall(5, CallInst);

    for (auto &returnReg : returnRegs) {
        NodeIndex valIdx = andersen->getNodeFactory().getValueNodeFor(returnReg);
        if (valIdx == AndersNodeFactory::InvalidIndex)
            valIdx = andersen->getNodeFactory().createValueNode(returnReg);
        NodeIndex objIdx = andersen->getNodeFactory().getObjectNodeFor(singleton);
        if (objIdx == AndersNodeFactory::InvalidIndex)
            objIdx = andersen->getNodeFactory().createObjectNode(singleton);
        andersen->addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);
    }

    return true;
}

bool SecItemCopyAdd::shouldHandleCall(std::string &F) {
    if (F == "SecItemCopyMatching"){
        return true;
    }
    return false;
}
bool SecItemCopyAdd::run(const Instruction *CallInst, std::string &F, Andersen *andersen) {
    if (andersen->getCallGraph().containtsEdge(CallInst, F))
        return true;
    andersen->addToWorklist((Instruction*)CallInst);
    DetectParametersPass::UserSet_t PreX1 = DetectParametersPass::getRegisterValuesBeforeCall(6, CallInst);
    for (auto &X1 : PreX1) {
        std::vector<const Value*> ptsToSet1;
        andersen->getPointsToSet(X1, ptsToSet1);
        for (auto &pts1 : ptsToSet1) {
            std::vector<const Value*> ptsToSet2;
            andersen->getPointsToSet(andersen->getNodeFactory().getLocation(pts1), ptsToSet2);

            if (ptsToSet2.size() == 0) {
                Value *dummy = andersen->getNodeFactory().createDummy(*(Module*)CallInst->getParent()->getParent()->getParent());
                NodeIndex objIdx = andersen->getNodeFactory().createObjectNode(dummy);
                NodeIndex valIdx = andersen->getNodeFactory().createValueNode(dummy);
                andersen->addConstraint(AndersConstraint::ADDR_OF, valIdx, objIdx);

                NodeIndex valIdx1 = andersen->getNodeFactory().getValueNodeFor(andersen->getNodeFactory().getLocation(pts1));
                if (valIdx1 == AndersNodeFactory::InvalidIndex) {
                    valIdx1 = andersen->getNodeFactory().createValueNode(andersen->getNodeFactory().getLocation(pts1));
                }

                andersen->addConstraint(AndersConstraint::STORE, valIdx, valIdx1);
                andersen->setType(dummy, "NSData");
                andersen->setType(dummy, "NSDictionary");
                andersen->getCallGraph().addCallEdge(CallInst, F);
            }

            bool hasNSData = false;
            bool hasNSDict = false;
            for (auto &pts2 : ptsToSet2) {
                Andersen::StringSet_t types;
                andersen->getType((Value*)pts2, types);
                for (auto &t : types) {
                    if (t == "NSData") {
                        hasNSData = true;
                    }
                    if (t == "NSDictionary") {
                        hasNSDict = true;
                    }
                }

                if (hasNSData && hasNSDict) {
                    andersen->getCallGraph().addCallEdge(CallInst, F);
                }

                if (!hasNSData)
                    andersen->setType((Value*)pts2, "NSData");
                if (!hasNSDict)
                    andersen->setType((Value*)pts2, "NSDictionary");
            }

        }
    }
    return true;
}
