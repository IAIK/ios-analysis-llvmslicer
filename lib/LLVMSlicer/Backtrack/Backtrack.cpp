#include "../Slicing/FunctionStaticSlicer.h"

using namespace llvm;
using namespace slicing;

bool InsInfo::backtrack(InsInfoProvider *provider, PathElementBase *pathElement, std::vector<Path*> &paths, std::mutex &pathLock, Rule &rule) {

    typedef std::pair<InsInfo *, const Value *> Pred_t;
    typedef std::set<Pred_t> PredSet_t;

    PathElementBase *prevElement = pathElement->getPrev();
    const Value *prevRelevantVariable = prevElement ? prevElement->getRelevantVariable() : NULL;

    if (rule.shouldStop(pathElement)) {
        return true;
    }

    if (rule.dismissPath(pathElement)) {
        return false;
    }


    PredSet_t predecessors;


    if (prevElement && Up.find(prevElement->getElement()) != Up.end()) {
        const Value *prevLoc = ptr::getAndersen()->getNodeFactory().getLocation(pathElement->getRelevantVariable());
        if (prevLoc) {
            if (const Instruction *prevLocInst = dyn_cast<const Instruction>(prevLoc)) {
                if (prevLocInst->getName().find("init") != StringRef::npos) {
                    if (const Instruction *prevInst = dyn_cast<const Instruction>(prevElement->getElement())) {
                        if (prevInst->getParent()->getParent()   == prevLocInst->getParent()->getParent()) {
                            return false;
                        }
                    }
                }
            }
        }
        for (auto &p : SlicedPredecessors[pathElement->getRelevantVariable()]) {
            predecessors.insert(Pred_t(provider->getInsInfo(p), pathElement->getRelevantVariable()));
        }
        if (predecessors.size() == 0) {
            return false;
        }
    }

    if (predecessors.size() == 0 && ins->getOpcode() == Instruction::Call && (!prevElement || Up.find(prevElement->getElement()) == Up.end())) {

        const Module *M = ins->getParent()->getParent()->getParent();

        SimpleCallGraph::FunctionSet_t &called = ptr::getSimpleCallGraph().getCalled(ins);
        for (auto &functionName : called) {
            if (functionName == "CCKeyDerivationPBKDF") {
                assert(true);
            }
            if (functionName == "-[NSString dataUsingEncoding:]") {
                assert(true);
            }
            if (functionName == "+[RNEncryptor encryptData:withSettings:password:error:]") {
                assert(true);
            }
            if (functionName == "fn_10006EE1C") {
                assert(true);
            }

            const Function *function = M->getFunction(functionName);
            if (!function || function->isDeclaration() || function->isIntrinsic())
                continue;
            const Instruction *ret = nullptr;
            for (auto &BB_it : function->getBasicBlockList()) {
                if (BB_it.getTerminator() && BB_it.getTerminator()->getOpcode() == Instruction::Ret) {
                    ret = BB_it.getTerminator();
                }
            }
            assert(ret);

            InsInfo *retInfo = provider->getInsInfo(ret);
            if (!retInfo) {
                errs() << "Function " << functionName << " not sliced. Can not continue path!\n";
                continue;
            }
            for (ValSet::const_iterator rc_it = DEF_begin(); rc_it != DEF_end(); ++rc_it) {

                for (ValSet::const_iterator ret_rc_it = retInfo->RC_begin();
                     ret_rc_it != retInfo->RC_end(); ++ret_rc_it) {
                    IncType_t RC_inc = retInfo->getRCInc(*ret_rc_it);
                    if (!(RC_inc < INC_MAX))
                        continue;
                    if (pathElement->getRelevantVariable() != ret_rc_it->first)
                        continue;
                    if (ret_rc_it->first == rc_it->first) {
                        if (retInfo->SlicedPredecessors.find(rc_it->first) != retInfo->SlicedPredecessors.end()) {
                            std::pair<const Instruction *, const Value *> v(ret, prevRelevantVariable);
                            predecessors.insert(Pred_t(retInfo, pathElement->getRelevantVariable()));
                            pathElement->getParent()->addCall(dyn_cast<CallInst>(ins));
//                                visited.insert(v);

                        }
                    }
                }
            }
        }
    }

    //FIXME: always skip this when a return was found???
    if (predecessors.size() == 0) {
        for (auto &RC_it : SlicedPredecessors) {

            if (hasTranslation(pathElement->getRelevantVariable())) {
                if (translations[pathElement->getRelevantVariable()].find((Value*) RC_it.first) ==
                    translations[pathElement->getRelevantVariable()].end()) {
                    continue;
                } else {

                }
            }
            IncType_t RC_inc = getRCInc(Pointee(RC_it.first, -1));
            if (!(RC_inc < INC_MAX))
                continue;
            if (ins->getOpcode() == Instruction::Ret) {
                if (RC_it.first != pathElement->getRelevantVariable())
                    continue;
            }
            for (auto &I_it : RC_it.second) {
                if (I_it == ins) {
                    continue;
                }
                if (hasTranslation(pathElement->getRelevantVariable()) &&
                    translations[pathElement->getRelevantVariable()].find(I_it) != translations[pathElement->getRelevantVariable()].end()) {

                } else {
                    InsInfo *pred = provider->getInsInfo(I_it);
                    std::pair<const Instruction*, const Value*> v(I_it, RC_it.first);
                    predecessors.insert(Pred_t(pred, RC_it.first));
//                            if (pred->Up.find(ins) != pred->Up.end()) {
//                                predecessors.insert(Pred_t(pred, RC_it.first));
//                            } else {
//                                if (RC_it.first == pathElement->getRelevantVariable()) {
//                                    predecessors.insert(Pred_t(pred, RC_it.first));
//                                }
//                            }
                }
            }
        }
    }

    if (ins->getOpcode() == Instruction::Call && prevElement && Up.find(prevElement->getElement()) != Up.end() && predecessors.size() == 0) {
        return false;
    }

    if (predecessors.size() == 0) {
        for (ValSet::const_iterator RC_it = RC_begin(); RC_it != RC_end(); ++RC_it) {
            //This should be a parameter
            if (RC_it->first == ins) {
                for (auto &pred_it : SlicedPredecessors) {
                    const Instruction *I = dyn_cast<const Instruction>(pred_it.first);
                    if (!I) {
                        continue;
                    }
                    std::pair<const Instruction*, const Value*> v(I, I);

                    predecessors.insert(Pred_t(provider->getInsInfo(I), I));
                }
            }
        }
    }

    if (predecessors.size() == 0) {

        if (hasTranslation(pathElement->getRelevantVariable())) {
            for (auto &t : translations[pathElement->getRelevantVariable()]) {
                t->dump();
                ConstPathElement *e = new ConstPathElement(t, pathElement->getRelevantVariable());
                if (pathElement->setNext(e)) {
                    return true;
                } else {
                    delete(e);
                    return false;
                }
            }
        }

        for (auto ref : REF) {
            IncType_t inc = getREFInc(ref);
            if (!(inc < INC_MAX)) {
                continue;
            }
            const Value *orig = ptr::getAndersen()->getNodeFactory().getLocation(ref.first);
            if (!orig) {
                continue;
            }
            if (dyn_cast<const ConstantInt>(orig)) {
                ConstPathElement *e = new ConstPathElement(orig, pathElement->getRelevantVariable());
                if (pathElement->setNext(e)) {
                    return true;
                } else {
                    delete(e);
                    return false;
                }
            }
        }
//            errs() <<"\n";
//            errs()<< ins->getParent()->getParent()->getName() << ": ";ins->dump();

        if (ins->getOpcode() == Instruction::Store) {
            if (const ConstantInt *constantInt = dyn_cast<const ConstantInt>(ins->getOperand(0))) {
                ConstPathElement *constPathElement = new ConstPathElement(constantInt, pathElement->getRelevantVariable());
                pathElement->setNext(constPathElement);
                constPathElement->setPrev(pathElement);
            }
        }
        return true;
    } else {
        PredSet_t::iterator p_it = predecessors.begin();
        PredSet_t::iterator p_first = p_it;



        ++p_it;
        for (; p_it != predecessors.end(); ++p_it) {
//            for (int i = 0; i < 1 && p_it != predecessors.end(); ++i) {
            if (pathElement->getParent()->contains(p_it->first->getIns(), p_it->second))
                continue;
            PathElement *element = new PathElement(p_it->first->getIns(), p_it->second);
            element->setPrev(pathElement);
            Path *newPath = new Path(*pathElement->getParent());
            if (!newPath->getLast()->setNext(element)) {
                delete(newPath);
                continue;
            };

            InsInfo *ii = p_it->first;

            if (ii->backtrack(provider, element, paths, pathLock, rule)) {
                pathLock.lock();
                if (//(std::find_if(paths->begin(), paths->end(), [e](const Path *p) { return e->getParent()->isSub(*p);}) == paths->end()) &&
                        (std::find_if(paths.begin(), paths.end(), [element](const Path *p) { return *element->getParent() == *p;}) == paths.end())) {
                    paths.push_back(element->getParent());
                } else {
                    delete(element->getParent());
                }
                pathLock.unlock();
            } else {
                delete(element->getParent());
            }

        }

        bool result = true;
        if (pathElement->getParent()->contains(p_first->first->getIns(), p_first->second)) {
            result = false;
        }
        if (result) {
            PathElement *element = new PathElement(p_first->first->getIns(), p_first->second);
            if (!pathElement->setNext(element)) {
                delete (element);
                result = false;
            } else {
                element->setPrev(pathElement);
                result = p_first->first->backtrack(provider, element, paths, pathLock, rule);
            }
        }

        return result;
    }
}
