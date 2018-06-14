// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.

#include <algorithm>
#include <iterator>
#include <llvm/Analysis/Andersen/StackAccessPass.h>
#include <llvm/IR/PatternMatch.h>

#include "llvm/IR/Constant.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"

#include "../Callgraph/Callgraph.h"
#include "../PointsTo/PointsTo.h"
#include "Modifies.h"
#include "ExternalHandler.gen"

using namespace llvm;

namespace llvm {
    std::set<std::pair<int64_t, const Value*>> getMemcpyDefs(const Instruction *call, const ptr::PointsToSets &PS) {
        std::set<std::pair<int64_t, const Value*>> defs;


        DetectParametersPass::UserSet_t X2_pre = DetectParametersPass::getRegisterValuesBeforeCall(7, call, false);
        DetectParametersPass::UserSet_t X0_pre = DetectParametersPass::getRegisterValuesBeforeCall(5, call, false);

        Andersen *andersen = ptr::getAndersen();

        Andersen::StackOffsetMap_t stackOffsetMap = andersen->getStackOffsets();

        for (auto &X2_it : X2_pre) {
            if (const ConstantInt *size = dyn_cast<const ConstantInt>(X2_it)) {

                for (auto &X0_it : X0_pre) {

                    ptr::PointsToSets::PointsToSet X0_ptsTo = ptr::getPointsToSet(X0_it, PS);

                    for (auto &X0pts_it : X0_ptsTo) {
                        auto &pairs = stackOffsetMap[X0pts_it.first];

                        for (auto &p_it : pairs) {

                            int64_t lo = p_it.second;
                            int64_t hi = p_it.second + size->getZExtValue();

                            Function *f = (Function*) p_it.first;
                            StackAccessPass::OffsetValueListMap_t &OffsetValues = andersen->getAnalysis<StackAccessPass>().getOffsetValues(f);
                            for (auto &O_it : OffsetValues) {
                                if (O_it.first <= lo || O_it.first >= hi)
                                    continue;

                                if (!O_it.second)
                                    continue;
                                for (auto &V_it: *O_it.second) {
                                    ptr::PointsToSets::PointsToSet defPtsTo = ptr::getPointsToSet(V_it, PS);
                                    for (auto &def_it : defPtsTo) {
                                        int64_t offset = O_it.first - lo;

                                        defs.insert(std::pair<int64_t, const Value*>(offset, def_it.first));
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }

        return defs;
    };
}

namespace llvm { namespace mods {

//  ProgramStructure::ProgramStructure(Module &M) {
//    for (Module::iterator f = M.begin(); f != M.end(); ++f)
//      if (!f->isDeclaration() && !memoryManStuff(&*f))
//        for (inst_iterator i = inst_begin(*f); i != inst_end(*f); ++i)
//          if (const StoreInst *s = dyn_cast<StoreInst>(&*i)) {
////            const Value *l = elimConstExpr(s->getPointerOperand());
////	    this->getContainer()[&*f].push_back(ProgramStructure::Command(
////		  hasExtraReference(l) ? CMD_VAR : CMD_DREF_VAR, l));
//              this->getContainer()[&*f].push_back(ProgramStructure::Command(CMD_DREF_VAR, s));
//          }
//  }

        ProgramStructure::ProgramStructure(Module &M, const llvm::ptr::PointsToSets &PS) {
            auto task = [&](Module::iterator &f, inst_iterator &i) {
                SimpleCallGraph::FunctionSet_t &called = ptr::getAndersen()->getCallGraph().getCalled(&*i);
                for (auto &functioNName : called) {
                    handleCall(&*i, PS, *(std::string*)&functioNName, this->getContainer()[&*f], lockC);
                }
            };
            for (Module::iterator f = M.begin(); f != M.end(); ++f) {

                if (!f->isDeclaration() && !memoryManStuff(&*f)) {
                    for (inst_iterator i = inst_begin(*f); i != inst_end(*f); ++i) {
                        if (const StoreInst *s = dyn_cast<StoreInst>(&*i)) {
//                            const Value *l = elimConstExpr(s->getPointerOperand());
//                            this->getContainer()[&*f].push_back(ProgramStructure::Command(
//                                    hasExtraReference(l) ? CMD_VAR : CMD_DREF_VAR, l));

                            ConstantInt *constantInt = nullptr;
                            if (PatternMatch::match(s->getOperand(1), PatternMatch::m_IntToPtr(
                                    PatternMatch::m_ConstantInt(constantInt)))) {
                                if (constantInt->getZExtValue() == 4301043416) {
                                    assert(true);
                                }
                                ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(s->getOperand(1), PS);
                                for (auto &p : pts) {
                                    lockC.lock();
                                    this->getContainer()[&*f].push_back(
                                            ProgramStructure::Command(CMD_FRC_DEF, p.first));
                                    lockC.unlock();
                                }
                            } else if (const Instruction *locationInstruction = dyn_cast<const Instruction>(
                                    s->getOperand(1))) {
                                if (locationInstruction->getOpcode() != Instruction::GetElementPtr) {
                                    ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(locationInstruction, PS);
                                    for (auto &p : pts) {
                                        lockC.lock();
                                        this->getContainer()[&*f].push_back(
                                                ProgramStructure::Command(CMD_FRC_DEF, p.first));
                                        lockC.unlock();
                                    }
                                }
                            }
//                        } else if (const CallInst *c = dyn_cast<const CallInst>(&*i)) {
                        } else if (i->getOpcode() == Instruction::Call) {
//                            pool.async(task, f, i);
                            task(f, i);
                        } else if (i->getOpcode() == Instruction::Load) {
                            Value *Base = nullptr;
                            Instruction *IVAR = nullptr;
                            if (PatternMatch::match(i->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_BinOp(PatternMatch::m_Value(Base), PatternMatch::m_SExt(PatternMatch::m_Instruction(IVAR)))))) {
                                ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(&*i, PS);
                                for (auto &p : pts) {
                                    lockC.lock();
                                    this->getContainer()[&*f].push_back(
                                            ProgramStructure::Command(CMD_FRC_DEF, p.first));
                                    lockC.unlock();
                                }
                            }
                         }
                    }
                }


                DetectParametersPass *DPP = ptr::getAndersen()->getAnalysisIfAvailable<DetectParametersPass>();
                if (!DPP)
                    DPP = &ptr::getAndersen()->getAnalysis<DetectParametersPass>();

                DetectParametersPass::ParameterAccessPairSet_t &Ret = DPP->getReturnRegisterIndexes(f);
                for (auto &r : Ret) {
                    if (const StoreInst *store = dyn_cast<StoreInst>(r.second)) {
                        this->getContainer()[&*f].push_back(
                                ProgramStructure::Command(CMD_DEF, store->getOperand(0)));
                        ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(store->getOperand(0), PS);
                        for (auto &p : pts) {
                            this->getContainer()[&*f].push_back(
                                    ProgramStructure::Command(CMD_DEF, p.first));
                        }
                    } else {
                        assert(false);
                    }

                }
            }

        }

  const Modifies::ModSet &getModSet(const llvm::Function *const &f,
	    const Modifies &S) {
    static const Modifies::ModSet empty;
    const Modifies::const_iterator it = S.find(f);

    return (it == S.end()) ? empty : it->second;
  }


//  void computeModifies(const ProgramStructure &P,
//	const callgraph::Callgraph &CG, const ptr::PointsToSets &PS,
//	Modifies &MOD) {
//    typedef ptr::PointsToSets::Pointee Pointee;
//
//    for (ProgramStructure::const_iterator f = P.begin(); f != P.end(); ++f)
//      for (ProgramStructure::mapped_type::const_iterator c = f->second.begin();
//	   c != f->second.end(); ++c) {
//          StringRef FName = f->first->getName();
//
//          if (c->getType() == CMD_VAR) {
//              if (!isLocalToFunction(c->getVar(), f->first))
//                  MOD[f->first].insert(Pointee(c->getVar(), -1));
//          } else if (c->getType() == CMD_DREF_VAR) {
//              typedef ptr::PointsToSets::PointsToSet PTSet;
//              const Instruction *Inst = dyn_cast<const Instruction>(c->getVar());
//              assert(Inst);
////              const PTSet &S = ptr::getPointsToSet(c->getVar(), PS);
//              const PTSet &S = ptr::getPointsToSet(Inst->getOperand(1), PS);
//
//              for (PTSet::const_iterator p = S.begin(); p != S.end(); ++p)
//                  if (!isLocalToFunction(p->first, f->first) &&
//                      !isConstantValue(p->first))
//                      MOD[f->first].insert(*p);
//          }
//      }
//
//    typedef callgraph::Callgraph Callgraph;
//    for (Callgraph::const_iterator i = CG.begin_closure();
//	  i != CG.end_closure(); ++i) {
//        StringRef F1 = i->first->getName();
//        StringRef F2 = i->second->getName();
//      const Modifies::mapped_type &src = MOD[i->second];
//      typedef Modifies::mapped_type dst_t;
//      dst_t &dst = MOD[i->first];
//
//      std::copy(src.begin(), src.end(), std::inserter(dst, dst.end()));
//#if 0 /* original boost+STL uncompilable crap */
//      using std::tr1::bind;
//      using std::tr1::placeholders::_1;
//      using std::tr1::cref;
//      dst.erase(std::remove_if(dst.begin(), dst.end(),
//		bind(&ProgramStructure::isLocalToFunction, cref(P), _1, i->first)),
//		dst.end());
//#endif
//      for (dst_t::iterator I = dst.begin(), E = dst.end(); I != E; ) {
//	if (isLocalToFunction(I->first, i->first))
//	  dst.erase(I++);
//	else
//	  ++I;
//      }
//    }
//
//#ifdef DEBUG_DUMP
//    errs() << "\n==== MODSET DUMP ====\n";
//    for (ProgramStructure::const_iterator f = P.begin(); f != P.end(); ++f) {
//	const Function *fun = f->first;
//	const Modifies::ModSet &m = MOD[fun];
//
//	errs() << fun->getName() << "\n";
//	for (Modifies::ModSet::const_iterator I = m.begin(), E = m.end(); I != E; ++I) {
//	    const Instruction *val = dyn_cast<Instruction>(I->first);
//	    errs() << "\tFUN=" << val->getParent()->getParent()->getName() <<
//		" OFF=" << I->second << " ";
//	    val->dump();
//	}
//    }
//    errs() << "==== MODSET END ====\n";
//#endif
//  }

        void computeModifies(const ProgramStructure &P,
                             const callgraph::Callgraph &CG, const ptr::PointsToSets &PS,
                             Modifies &MOD) {
            typedef ptr::PointsToSets::Pointee Pointee;

            DetectParametersPass *DPP = ptr::getAndersen()->getAnalysisIfAvailable<DetectParametersPass>();
            if (!DPP)
                DPP = &ptr::getAndersen()->getAnalysis<DetectParametersPass>();

            errs() << "Compute Modifies\n";


            for (ProgramStructure::const_iterator f = P.begin(); f != P.end(); ++f) {

                std::set<const Value*> passedReferences, workSet, tmp;



                DetectParametersPass::ParameterAccessPairSet_t &regParams = DPP->getParameterRegisterIndexes((Function*)f->first);
                for (auto &regParam : regParams) {
                    ptr::PointsToSets::PointsToSet ps = ptr::getPointsToSet(regParam.second, PS);
                    for (auto &p : ps) {
                        workSet.insert(p.first);
                    }
                }

//                std::set<uint64_t> SPIdx;
//                SPIdx.insert(4);
//                DetectParametersPass::ParameterAccessPairSet_t stackParams = DPP->getStackParameters(*(Function*)f->first, SPIdx);
//                for (auto &stackParam : stackParams) {
//                    ptr::PointsToSets::PointsToSet ps = ptr::getPointsToSet(stackParam.second, PS);
//                    for (auto &p : ps) {
//                        workSet.insert(p.first);
//                    }
//                }

                while (workSet.size()) {
                    for (auto &w : workSet) {
                        if (dyn_cast<const ConstantDataArray>(w)) {
                            continue;
                        }
                        passedReferences.insert(w);

                        ptr::PointsToSets::PointsToSet ps = ptr::getPointsToSet(w, PS);
                        for (auto &p : ps) {
                            if (passedReferences.find(p.first) == passedReferences.end()) {
                                tmp.insert(p.first);
                            }
                        }
                    }
                    std::swap(workSet, tmp);
                    tmp.clear();
                }

                const Instruction *retInst = nullptr;
                for (const_inst_iterator i_it =  inst_begin(*f->first); i_it != inst_end(*f->first); ++i_it) {
                    if (i_it->getOpcode() == Instruction::Ret) {
                        retInst = &*i_it;
                        break;
                    }
                }

                if (retInst) {
                    DetectParametersPass::UserSet_t x8Vals = DetectParametersPass::getRegisterValuesBeforeCall(13, retInst, false);
                    for (auto &v : x8Vals) {
                        ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(v, PS);
                        for (auto &p : pts) {
                            passedReferences.insert(p.first);
                        }
                    }
                }

                DetectParametersPass::ParameterAccessPairSet_t &Ret = DPP->getReturnRegisterIndexes((Function*)f->first);
                for (auto &r : Ret) {
                    if (const StoreInst *store = dyn_cast<StoreInst>(r.second)) {
                        passedReferences.insert(store->getOperand(0));
                        ptr::PointsToSets::PointsToSet pts = ptr::getPointsToSet(store->getOperand(0), PS);
                        for (auto &p : pts) {
                            passedReferences.insert(p.first);
                        }
                    } else {
                        assert(false);
                    }

                }

                for (ProgramStructure::mapped_type::const_iterator c = f->second.begin();
                     c != f->second.end(); ++c) {

                    if (c->getType() == CMD_VAR) {
                        if (!isLocalToFunction(c->getVar(), f->first))
                            MOD[f->first].insert(Pointee(c->getVar(), -1));
                        else {
                            c->getVar()->dump();
                            llvm_unreachable("");
                        }
                    } else if (c->getType() == CMD_DREF_VAR) {
                        typedef ptr::PointsToSets::PointsToSet PTSet;
                        const PTSet &S = ptr::getPointsToSet(c->getVar(), PS);

                        for (PTSet::const_iterator p = S.begin(); p != S.end(); ++p)
                            if (!isLocalToFunction(p->first, f->first) &&
                                !isConstantValue(p->first))
                                MOD[f->first].insert(*p);
                    } else if (c->getType() == CMD_EXT_ARG) {
                        std::set<std::pair<int64_t, const Value*>> defs = llvm::getMemcpyDefs(dyn_cast<const Instruction>(c->getVar()), PS);
                        for (auto &d_it : defs) {
                            MOD[f->first].insert(Pointee(d_it.second, -1));
                        }
                    } else if (c->getType() == CMD_DEF) {
                        if (passedReferences.find(c->getVar()) != passedReferences.end()) {
                            MOD[f->first].insert(Pointee(c->getVar(), -1));
                        }
                    } else if (c->getType() == CMD_FRC_DEF) {
                        MOD[f->first].insert(Pointee(c->getVar(), -1));
                    }
                }
            }

            typedef callgraph::Callgraph Callgraph;
            for (Callgraph::const_iterator i = CG.begin_closure();
                 i != CG.end_closure(); ++i) {
                const Modifies::mapped_type &src = MOD[i->second];
                typedef Modifies::mapped_type dst_t;
                dst_t &dst = MOD[i->first];

                std::copy(src.begin(), src.end(), std::inserter(dst, dst.end()));
#if 0 /* original boost+STL uncompilable crap */
                using std::tr1::bind;
      using std::tr1::placeholders::_1;
      using std::tr1::cref;
      dst.erase(std::remove_if(dst.begin(), dst.end(),
		bind(&ProgramStructure::isLocalToFunction, cref(P), _1, i->first)),
		dst.end());
#endif
                //We don't need to check for local variables since only 'dummy' locations and return values are added anyway.
//                for (dst_t::iterator I = dst.begin(), E = dst.end(); I != E; ) {
//                    if (isLocalToFunction(I->first, i->first))
//                        dst.erase(I++);
//                    else
//                        ++I;
//                }
            }

#ifdef DEBUG_DUMP
            errs() << "\n==== MODSET DUMP ====\n";
    for (ProgramStructure::const_iterator f = P.begin(); f != P.end(); ++f) {
	const Function *fun = f->first;
	const Modifies::ModSet &m = MOD[fun];

	errs() << fun->getName() << "\n" << "----------------------------------------------------------------\n";
	for (Modifies::ModSet::const_iterator I = m.begin(), E = m.end(); I != E; ++I) {
	    const Instruction *val = dyn_cast<Instruction>(I->first);
	    errs() << "\tFUN=" << (val ? val->getParent()->getParent()->getName() : "no parent" )<<
		" OFF=" << I->second << " ";
	    I->first->dump();
	}
	errs() << "\n";
    }
    errs() << "==== MODSET END ====\n";
#endif
        }

}}
