// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.

#include <llvm/IR/PassManager.h>
#include <llvm/Analysis/Andersen/DetectParametersPass.h>
#include <thread>
#include <future>
#include <utility>
#include <llvm/Support/UniqueLock.h>
#include <signal.h>
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/PassSupport.h"

#include "FunctionStaticSlicer.h"
#include "../Callgraph/Callgraph.h"
#include "../Modifies/Modifies.h"
#include "../PointsTo/PointsTo.h"
#include "../Backtrack/Path.h"
#include "../Backtrack/Rule.h"
#include "../Backtrack/Constraint.h"
#include "llvm/LLVMSlicer/StaticSlicer.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/Debug.h"

#define DEBUG_TYPE "slicer"

using namespace llvm;

static cl::opt<std::string>
        ReportFilename("r", cl::desc("Path to HTML report output file"),
                       cl::value_desc("report"));

namespace llvm { namespace slicing { namespace detail {




    typedef ptr::PointsToSets::Pointee Pointee;
    typedef std::map<const Pointee, const Pointee> ParamsToArgs;
    typedef std::set<Pointee> RelevantSet;

    static void fillParamsToArgs(const CallInst *C, const Function *F,
			  ParamsToArgs &toArgs)
    {
	Function::const_arg_iterator p = F->arg_begin();

	for (unsigned a = 0; a < C->getNumArgOperands(); ++a, ++p) {
	    const Value *P = &*p;

	    const Value *A = C->getArgOperand(a);
	    if (!isConstantValue(A))
		toArgs.insert(ParamsToArgs::value_type(Pointee(P, -1),
					Pointee(A, -1)));
	}
    }

static void getRelevantVarsAtCall(const CallInst *C, const Function *F, DetectParametersPass *DPP, const ptr::PointsToSets &PS,
                                  ValSet::const_iterator b,
                                  const ValSet::const_iterator &e,
                                  RelevantSet &out,
                                  FunctionStaticSlicer *FSS,
                                  InsInfo *entry,
                                  InsInfo::IncMap_t &RCInc,
                                  InsInfo::SliceInfoSetMap_t &structSliceInfos,
                                  InsInfo::ValMapSet_t &RCSources) {
    assert(!isInlineAssembly(C) && "Inline assembly is not supported!");
    DEBUG(errs() << C->getParent()->getParent()->getName() << " -> " << F->getName() << "\n");

    if (C->getParent()->getParent()->getName() == "-[SOEncryptionModel encryptData:withSymetricKey:]") {
        assert(true);
    }

//    if (C->getParent()->getParent()->getName() == "_EXTERNAL_")
//        return;

    ParamsToArgs toArgs;
    fillParamsToArgs(C, F, toArgs);


    DetectParametersPass::ParameterAccessPairSet_t Reg = DPP->getParameterRegisterIndexes((Function*)F);

    for (; b != e; ++b) {
//        if (Instruction *Ref = dyn_cast<Instruction>((Value*)b->first)) {
//            if (Ref->getOpcode() == Instruction::Store) {
//                out.insert(ptr::PointsToSets::Pointee(Ref->getOperand(0), -1));
//            }
//        }
//        if (entry->getRCInc(*b) >= INC_MAX) {
//            continue;
//        }

        const Value *loc = ptr::getAndersen()->getNodeFactory().getLocation(b->first);
        if (loc) {
            if (const Instruction *locInst = dyn_cast<const Instruction>(loc)) {
                if (loc->getName().find("init") != StringRef::npos && locInst->getParent()->getParent() == F) {
                    RCSources.erase(b->first);
                    continue;
                }
            }
        }

        bool foundParameter = false;
        if (const Instruction *I = dyn_cast<const Instruction>(b->first)) {
            if (I->getOpcode() == Instruction::Load) {

                for (DetectParametersPass::ParameterAccessPairSet_t::iterator Reg_it = Reg.begin(); Reg_it != Reg.end(); ++Reg_it) {
                    if (I == Reg_it->second) {
                        DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(Reg_it->first, C, true);
                        for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {

                            Instruction *I = (Instruction*)(*Pre_it);
                            Pointee p(*Pre_it, -1);

                            const InsInfo::ValSet_t src = entry->getRCSource(*b);
                            if (src.size() == 0) {
                                continue;
                            }
                            RCSources[p.first] = src;
                            RCSources[p.first].insert(I);

                            out.insert(p);

                            structSliceInfos[p.first].insert(entry->RCStruct_begin(p.first), entry->RCStruct_end(p.first));

                            RCInc[p.first] = RCInc[(*b).first];

                            foundParameter = true;
                        }
                    }
                }
            }
        }

//        for (DetectParametersPass::ParameterAccessPairSet_t::iterator Pair = Ret.begin(); Pair != Ret.end(); ++Pair) {
//            const ptr::PointsToSets::PointsToSet &PT = ptr::getPointsToSet(Pair->second, PS);
//            for (ptr::PointsToSets::PointsToSet::iterator P_it = PT.begin(); P_it != PT.end(); ++P_it) {
//                P_it->first->dump();
////                if (P_it->first == b->first) {
//                    DetectParametersPass::UserSet_t Pre = DetectParametersPass::getRegisterValuesBeforeCall(Pair->first, (Instruction*)C, true);
//                    for (DetectParametersPass::UserSet_t::iterator Pre_it = Pre.begin(); Pre_it != Pre.end(); ++Pre_it) {
//                        (*Pre_it)->dump();
//                        out.insert(ptr::PointsToSets::Pointee(*Pre_it, -1));
////                    }
//                }
//            }
//        }

        const InsInfo::ValSet_t src = entry->getRCSource(*b);
        if (src.size() == 0) {
            continue;
        }
        RCSources[b->first].insert(src.begin(), src.end());

        //Copy all values that were not matched as parameters too
        structSliceInfos[b->first].insert(entry->RCStruct_begin(b->first), entry->RCStruct_end(b->first));

        if (foundParameter)
            continue;

//
//        bool hasDEF = false;
//
//        for (const_inst_iterator I_it = inst_begin(callingFunction); I_it != inst_end(callingFunction) && !hasDEF; ++I_it) {
//            InsInfo *info = FSS->getInsInfo(&*I_it);
//
//            for (ValSet::const_iterator DEF_it = info->DEF_begin(); DEF_it != info->DEF_end(); ++DEF_it) {
//                if (b->first == DEF_it->first) {
//                    hasDEF = true;
//                    break;
//                }
//
//                for (std::set<StructSliceInfo*>::iterator Struct_it = entry->RCStruct_begin(b->first); Struct_it != entry->RCStruct_end(b->first); ++Struct_it) {
//                    for (auto &base : (*Struct_it)->basePointers) {
//                        if (base.first == DEF_it->first) {
//                            hasDEF = true;
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//
//        if (!hasDEF) {
////            continue;
//        }

//        continue;

        ParamsToArgs::const_iterator it = toArgs.find(*b);
        if (it != toArgs.end())
            out.insert(it->second);
        else if (!isLocalToFunction(b->first, F))
            out.insert(*b);
    }
    return;
}

    static void getRelevantVarsAtExit(const CallInst *C, const ReturnInst *R, DetectParametersPass *DPP, const ptr::PointsToSets &PS,
			       ValSet::const_iterator b,
			       const ValSet::const_iterator &e,
			       RelevantSet &out,
                   InsInfo *succInfo,
                   InsInfo *callInfo,
                   InsInfo::IncMap_t &RCInc,
                   InsInfo::SliceInfoSetMap_t &structSliceInfos,
                   InsInfo::ValMapSet_t &RCSources,
                   const mods::Modifies &MOD) {
        DEBUG(errs() << C->getParent()->getParent()->getName() << " -> " << R->getParent()->getParent()->getName() << "\n");
	assert(!isInlineAssembly(C) && "Inline assembly is not supported!");

        DetectParametersPass::ParameterAccessPairSet_t Ret = DPP->getReturnRegisterIndexes((Function*)R->getParent()->getParent());

//        for (DetectParametersPass::ParameterAccessPairSet_t::iterator Ret_it = Ret.begin(); Ret_it != Ret.end(); ++Ret_it) {
//            out.insert(ptr::PointsToSets::Pointee(Ret_it->second, -1));
//            out.insert(ptr::PointsToSets::Pointee(Ret_it->second->getOperand(0), -1));
//            DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(Ret_it->first, C);
//            for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
//                out.insert(ptr::PointsToSets::Pointee(*Post_it, -1));
//            }
//        }

//        return;

	if (callToVoidFunction(C)) {

//        std::copy(b, e, std::inserter(out, out.begin()));

        for (ValSet::const_iterator it = b; it != e; ++it) {
//            if (succInfo->getRCInc(*it) >= INC_MAX) {
//                continue;
//            }
//            RCSources[it->first].insert(succInfo->getIns());
//            RCSources[it->first].insert(callInfo->getIns());

            //Check if the relevant variable is modified in this function
            bool intersect = false;
            mods::Modifies::mapped_type const& M = getModSet(R->getParent()->getParent(), MOD);
            for (mods::Modifies::mapped_type::const_iterator v = M.begin();
                 v != M.end(); ++v) {
                if (v->first == (*it).first) {
                    intersect = true;
                    break;
                }
            }

            //Relevant variable won't be modified anywhere here...
            if (!intersect) {
                continue;
            }

            RCSources[it->first].insert(R);

            structSliceInfos[it->first].insert(callInfo->RCStruct_begin(it->first), callInfo->RCStruct_end(it->first));

            out.insert(*it);

//            const InsInfo::ValSet_t src = succInfo->getRCSource(*b);
//            RCSources[it->first] = src;
            structSliceInfos[it->first].insert(callInfo->RCStruct_begin(it->first), callInfo->RCStruct_end(it->first));
        }

        return;

        for (DetectParametersPass::ParameterAccessPairSet_t::iterator Ret_it = Ret.begin(); Ret_it != Ret.end(); ++Ret_it) {
            DetectParametersPass::UserSet_t Post = DetectParametersPass::getRegisterValuesAfterCall(Ret_it->first, C);
            for (DetectParametersPass::UserSet_t::iterator Post_it = Post.begin(); Post_it != Post.end(); ++Post_it) {
                for (ValSet::const_iterator b_it = b ; b_it != e; ++b_it) {
                    if (const Instruction *I = dyn_cast<const Instruction>(b_it->first)) {
                        if (I == (*Post_it)) {
                            Pointee p(Ret_it->second, -1);
                            out.insert(p);
                            RCInc[p.first] = RCInc[b_it->first];

                            const InsInfo::ValSet_t src = succInfo->getRCSource(*b);
                            RCSources[p.first] = src;
//                            RCSources[p.first].insert(callInfo->getIns());
                            structSliceInfos[p.first].insert(callInfo->RCStruct_begin(p.first), callInfo->RCStruct_end(p.first));
                        }
                    }
                }
            }
        }
        return;
	}

        llvm_unreachable("Decompiled code uses void functions only");

	for ( ; b != e; ++b)
	    if (b->first == C) {
		Value *ret = R->getReturnValue();
		if (!ret) {
/*		    C->dump();
		    C->getCalledValue()->dump();
		    R->dump();*/
//		    abort();
		    return;
		}
		out.insert(Pointee(R->getReturnValue(), -1));
	    } else
		out.insert(*b);
    }

}}}

namespace llvm { namespace slicing {



        static cl::opt<int> limitCalls("limit-calls", cl::init(0), cl::Hidden);

    class StaticSlicer : public InsInfoProvider {
    public:
        typedef std::map<llvm::Function const*, FunctionStaticSlicer *> Slicers;
        typedef std::multimap<llvm::Function const*,llvm::CallInst const*>
                FuncsToCalls;
        typedef std::multimap<llvm::CallInst const*,llvm::Function const*>
                CallsToFuncs;

        StaticSlicer(ModulePass *MP, Module &M,
		     const ptr::PointsToSets &PS,
                     const callgraph::Callgraph &CG,
                     const mods::Modifies &MOD,
                     std::vector<Rule*> rules);

        ~StaticSlicer();

        void computeSlice();
        void ruleIteration();
        bool sliceModule();

        virtual void addInitialSlicingCriterion(const Instruction *C);

        virtual InsInfo *getInsInfo(const Instruction *I);

        bool addRule(Rule *rule);
        const std::vector<Rule*> getRules() const {
            return rules;
        }

    private:
        typedef llvm::SmallVector<const llvm::Function *, 20> InitFuns;

        legacy::PassManager *PM;
        DetectParametersPass *DPP;
        const ptr::PointsToSets &PS;
        const mods::Modifies &MOD;

        std::vector<Rule*> rules;
        std::vector<Rule*> ruleWorklist;

	void buildDicts(const ptr::PointsToSets &PS, const CallInst *c);
        void buildDicts(const ptr::PointsToSets &PS);

        void findInitialCriterions();

        template<typename OutIterator>
        void emitToCalls(llvm::Function const* const f, OutIterator out);

        template<typename OutIterator>
        void emitToExits(llvm::Function const* const f, OutIterator out);

        void runFSS(Function &F, const ptr::PointsToSets &PS,
                    const callgraph::Callgraph &CG, const mods::Modifies &MOD);

        ModulePass *MP;
        Module &module;
        Slicers slicers;
        std::mutex slicersLock;
        InitFuns initFuns;
        FuncsToCalls funcsToCalls;
        CallsToFuncs callsToFuncs;

        std::set<const Instruction*> InitialCriterions;
    };

    template<typename OutIterator>
    void StaticSlicer::emitToCalls(const Function *f, OutIterator out) {
        const Instruction *entry = getFunctionEntry(f);
        const ValSet::const_iterator relBgn = slicers[f]->relevant_begin(entry);
        const ValSet::const_iterator relEnd = slicers[f]->relevant_end(entry);


        if (relBgn == relEnd) {
//            errs() << "Skip: " << f->getName() << "\n";
            return;
        }

        if (f->getName() == "-[RNCryptorEngine initWithOperation:settings:key:IV:error:]") {
            assert(true);
        }

        bool dump = false;
        if (dump) {
            for (ValSet::const_iterator it = relBgn; it != relEnd; ++it) {
                (*it).first->dump();
            }
        }

        InsInfo::IncMap_t RCInc;
        InsInfo *entryInfo = slicers[f]->getInsInfo(entry);
        for (ValSet::const_iterator i = relBgn; i != relEnd; ++i) {
            double RC_inc = entryInfo->getRCInc(*i);
            RCInc[i->first] = RC_inc;
        }

        FuncsToCalls::const_iterator c, e;
        std::tie(c, e) = funcsToCalls.equal_range(f);

        if (limitCalls && std::distance(c, e) > limitCalls) {
            errs() << "To many calls to function " << f->getName() << " -> skip\n";
            return;
        }

        std::set<std::string> calls;

        for (; c != e; ++c) {
            const CallInst *CI = c->second;
            const Function *g = CI->getParent()->getParent();
            FunctionStaticSlicer *FSS = slicers[g];

            detail::RelevantSet R;
            InsInfo::SliceInfoSetMap_t structSliceInfos;
            InsInfo::ValMapSet_t rcSources;
            detail::getRelevantVarsAtCall(c->second, f, DPP, PS, relBgn, relEnd, R, FSS, entryInfo, RCInc, structSliceInfos, rcSources);

            if (R.begin() == R.end()) {
//                errs() << "Skip: " << f->getName() << "\n";
                continue;
            }

            typedef std::set<const Function*> FunctionSet_t;
            std::function< bool(const Value *, const Function *, FunctionSet_t&) > isInScope = [&](const Value *r, const Function *f, FunctionSet_t &visited) {
                if (visited.find(f) != visited.end()) {
                    return false;
                }
                visited.insert(f);

                //probably a constant inst stored in a register. needs to be kept
                if (dyn_cast<const StoreInst>(r)) {
                    return true;
                }

                if (dyn_cast<const Constant>(r)) {
                    return true;
                }

                mods::Modifies::mapped_type const& M = getModSet(f, MOD);
                bool isModified = false;

                for (mods::Modifies::mapped_type::const_iterator v = M.begin();
                     v != M.end(); ++v) {
                        if (v->first == r) {
                            isModified = true;
                            break;
                        }
                }

                if (isModified) {

                } else {
                    FuncsToCalls::const_iterator calledBy_b, callledBy_e;
                    std::tie(calledBy_b, callledBy_e) = funcsToCalls.equal_range(f);

                    for (; calledBy_b != callledBy_e; ++calledBy_b) {
                        if (isInScope(r, calledBy_b->second->getParent()->getParent(), visited)) {
                            isModified = true;
                            break;
                        }
                    }
                }
//
//                if (!isModified) {
//                    for (auto &infos : structSliceInfos) {
//                        for (auto &ssi : infos.second) {
//                            for (auto &l : ssi->locations) {
//                                if (r == l.first) {
//                                    llvm_unreachable("");
//                                }
//                            }
//                        }
//                    }
//                }
                return isModified;
            };

            detail::RelevantSet toRemove;

            for (auto &r : R) {
                FunctionSet_t visited;
                if (!isInScope(r.first, g, visited)) {
                    toRemove.insert(r);
                }
            }

            for (auto &rem : toRemove) {
                DEBUG(errs() << "Not modified: "; rem.first->dump());
                R.erase(rem);
            }

            if (!R.size()) {
                DEBUG(errs() << "No relevant variables in scope " << f->getName() << "\n");
                continue;
            }

            FSS->initializeInfos();
            InsInfo *CallInfo = FSS->getInsInfo(CI);

            if (FSS->addCriterion(CI, R.begin(), R.end(), this,
                                  rcSources, RCInc, !FSS->shouldSkipAssert(CI))) {
                FSS->addCriterion(CI, FSS->REF_begin(CI), FSS->REF_end(CI));
                *out++ = g;
                calls.insert(c->second->getParent()->getParent()->getName().str());
            }

            for (auto &structInfo_it : structSliceInfos) {
                for (auto &ssi_it : structInfo_it.second) {
                  CallInfo->addRCStruct(structInfo_it.first, ssi_it);
                }
            }
        }
#ifdef DUMP_CALLS
        if (calls.size()) {
            errs() << f->getName() << " called by:\n";
            for (auto &s : calls) {
                errs() << "\t" << s << "\n";
            }
            errs() << "\n";
        }
#endif
    }

    template<typename OutIterator>
    void StaticSlicer::emitToExits(const Function *f, OutIterator out) {
        typedef std::vector<const CallInst *> CallsVec;

        CallsVec C;
        getFunctionCalls(f, std::back_inserter(C));

        std::set<std::string> calls;


        for (CallsVec::const_iterator c = C.begin(); c != C.end(); ++c) {
            const Instruction *succ = getSuccInBlock(*c);
            const ValSet::const_iterator relBgn =
                    slicers[f]->relevant_begin(succ);
            const ValSet::const_iterator relEnd =
                    slicers[f]->relevant_end(succ);

            InsInfo::IncMap_t RCInc;
            InsInfo *succInfo = slicers[f]->getInsInfo(succ);
            InsInfo *callInfo = slicers[f]->getInsInfo(*c);
            for (ValSet::const_iterator i = relBgn; i != relEnd; ++i) {
                double RC_inc = succInfo->getRCInc(*i);
                RCInc[i->first] = RC_inc;
            }

            CallsToFuncs::const_iterator g, e;
            std::tie(g, e) = callsToFuncs.equal_range(*c);

            for (; g != e; ++g) {
                typedef std::vector<const llvm::ReturnInst *> ExitsVec;
                const Function *callie = g->second;



                ExitsVec E;
                getFunctionExits(callie, std::back_inserter(E));

                for (ExitsVec::const_iterator e = E.begin(); e != E.end(); ++e) {

                    detail::RelevantSet R;
                    InsInfo::ValMapSet_t RCSources;
                    InsInfo::SliceInfoSetMap_t structSliceInfos;
                    detail::getRelevantVarsAtExit(*c, *e, DPP, PS, relBgn, relEnd, R, succInfo, callInfo, RCInc, structSliceInfos, RCSources, MOD);

                    if (relBgn == relEnd) {
                        continue;
                    }

                    slicers[g->second]->initializeInfos();
                    InsInfo *exitInfo = slicers[g->second]->getInsInfo(*e);

                    callInfo->addReturnPred(*e);
                    //FIXME: add real rc sources
                    if (slicers[g->second]->addCriterion(*e, R.begin(), R.end(), this, RCSources, RCInc)) {
                        *out++ = g->second;
                        calls.insert(g->second->getName());
                    }

                    for (auto &structInfo_it : structSliceInfos) {
                        for (auto &ssi_it : structInfo_it.second) {
                          exitInfo->addRCStruct(structInfo_it.first, ssi_it);
                        }
                    }
                }
            }
        }
#ifdef DUMP_CALLS
        if (calls.size()) {
            errs() << f->getName() << " calls\n";
            for (auto &s : calls) {
                errs() << "\t" << s << "\n";
            }
            errs() << "\n";
        }
#endif
    }

    void StaticSlicer::buildDicts(const ptr::PointsToSets &PS,
		const CallInst *c) {
	typedef std::vector<const Function *> FunCon;
	FunCon G;
	getCalledFunctions(c, PS, std::back_inserter(G));

	for (FunCon::const_iterator I = G.begin(), E = G.end(); I != E; ++I) {
	    const Function *h = *I;

	    if (!memoryManStuff(h) && !h->isDeclaration()) {
		funcsToCalls.insert(std::make_pair(h, c));
		callsToFuncs.insert(std::make_pair(c, h));
	    }
	}
    }

    void StaticSlicer::buildDicts(const ptr::PointsToSets &PS)
    {
        for (Module::const_iterator f = module.begin(); f != module.end(); ++f)
            if (!f->isDeclaration() && !memoryManStuff(&*f))
                for (const_inst_iterator I = inst_begin(*f), E = inst_end(*f);
			I != E; ++I)
                    if (const CallInst *c = dyn_cast<CallInst>(&*I)) {
			if (isInlineAssembly(c)) {
			    errs() << "ERROR: Inline assembler detected in " <<
				f->getName() << ", skipping\n";
			    continue;
			}

			buildDicts(PS, c);
		    }
    }

    StaticSlicer::StaticSlicer(ModulePass *MP, Module &M,
                               const ptr::PointsToSets &PS,
                               const callgraph::Callgraph &CG,
                               const mods::Modifies &MOD,
                               std::vector<Rule*> rules) :
                               PS(PS), MOD(MOD), MP(MP), module(M), slicers(), initFuns(), funcsToCalls(), callsToFuncs() {
        for (auto &rule : rules) {
            addRule(rule);
        }
        for (Module::iterator f = M.begin(); f != M.end(); ++f)
          if (!f->isDeclaration() && !memoryManStuff(&*f))
            runFSS(*f, PS, CG, MOD);
        buildDicts(PS);
        PM = new legacy::PassManager();
        DPP = new DetectParametersPass();
        PM->add(DPP);

        PM->run(M);
    }

    StaticSlicer::~StaticSlicer() {
      for (Slicers::const_iterator I = slicers.begin(), E = slicers.end();
           I != E; ++I)
        delete I->second;
    }

    void StaticSlicer::runFSS(Function &F, const ptr::PointsToSets &PS,
			      const callgraph::Callgraph &CG,
			      const mods::Modifies &MOD) {

      FunctionStaticSlicer *FSS = new FunctionStaticSlicer(F, MP, PS, MOD, this);

      slicers.insert(Slicers::value_type(&F, FSS));
    }

        void StaticSlicer::ruleIteration() {
            findInitialCriterions();

            struct FunctionCmp {
                bool operator()(const Function* lhs, const Function* rhs) const  { return lhs->getName().str().compare(rhs->getName().str()); }
            };
//        typedef SmallVector<const Function *, 20> WorkSet;
            typedef std::set<const Function *> WorkSet;
//        WorkSet Q(initFuns);
            WorkSet Q;
            for (auto &i : initFuns) {
                Q.insert(i);
            }

            errs() << "Found " << initFuns.size() << " initial criterions\n";

            initFuns.clear();

            uint64_t numSlices = 0;

            while (!Q.empty()) {

                size_t numFunctions = Q.size();

                errs() << "Num functions: " << numFunctions << "\n";


                for (WorkSet::const_iterator f = Q.begin(); f != Q.end(); ++f) {
                slicers[*f]->calculateStaticSlice();
            }

                WorkSet tmp;
                for (WorkSet::const_iterator f = Q.begin(); f != Q.end(); ++f) {
                    emitToCalls(*f, std::inserter(tmp, tmp.end()));
                    emitToExits(*f, std::inserter(tmp, tmp.end()));
                }
                std::swap(tmp,Q);

                std::vector<const Function*> x(Q.begin(), Q.end());
                std::sort(x.begin(), x.end());
                x.erase(std::unique(x.begin(), x.end()), x.end());

                if (x.size() != Q.size()) {
                    Q.clear();
                    Q.insert(x.begin(), x.end());
                }

            }

            errs() << "Num function slices: " << numSlices << "\n";

            std::vector<Rule*> toCheck(ruleWorklist);
            std::vector<Rule*> worklist(ruleWorklist);

            while(!worklist.empty()) {

                std::vector<Rule*> tmp;

                for (auto &w : worklist) {
                    for (auto &c : w->getChildren()) {
                        if (c->getType() == Constraint::RULE) {
                            tmp.push_back((Rule*)c);
                            toCheck.push_back((Rule*)c);
                        }
                    }
                }

                std::swap(worklist, tmp);
            }

            std::vector<Path*> paths;

            auto createPath = [&](const Instruction *call, const Instruction *inst, Rule *rule, Path *parent = nullptr) {
                InsInfo *C_info = getInsInfo(inst);
                assert(C_info);

                PathElement *element = nullptr;
                if (call) {

                } else {

                }

                Path *path = nullptr;
                if (parent) {
                    path = parent;
                } else {
                    path = new Path();
                }

                if (parent) {
                    element = new PathElement(inst, inst);
                    path->getLast()->setNext(element);
                    element->setPrev(path->getLast());
                } else if (call) {
                    PathElement *start = new PathElement(call, inst);
                    element = new PathElement(inst, inst);

                    path->setEntry(start);
                    start->setNext(element);
                    element->setPrev(start);
                } else {
                    element = new PathElement(inst, inst);
                    path->setEntry(element);
                }

                std::vector<Path*> p;
                std::mutex pathLock;
                if (C_info->backtrack(this, element, p, pathLock, *rule)) {
                    pathLock.lock();
                    if (std::find_if(p.begin(), p.end(), [path](const Path *other) { return *path == *other;}) == p.end()) {
                        p.push_back(path);
                    }
                    pathLock.unlock();
                } else {
                    delete(path);
                }
                std::sort(p.begin(), p.end());

                bool modified = false;
                do {
                    modified = false;
                    for (std::vector<Path *>::iterator i = p.begin(); i != p.end() && !modified; ++i) {
                        std::vector<Path *>::iterator j = i;
                        std::advance(j, 1);
                        for (; j != p.end(); ++j) {
                            if (i == j)
                                continue;
                            if (**i == **j) {
                                p.erase(j);
                                modified = true;
                                break;
                            }
                        }
                    }
                }while(modified);

                rule->addPaths(p);
            };

            errs() << "Backtrack\n";
            for (std::vector<Rule*>::iterator rule = toCheck.begin(); rule != toCheck.end(); ++rule) {
                errs() << (*rule)->getRuleTitle() << "\n";
                for (auto &C : (*rule)->getInitialInstruction()) {
                    for (auto &C_pre : C.second) {
                        createPath(C.first.first, C_pre.first, C_pre.second);
                    }

                    if ((*rule)->getParentRuleTitle().size()) {
                        for (auto &p : rules) {
                            if (p->getRuleTitle() == (*rule)->getParentRuleTitle()) {
                                for (auto &path : p->getPaths()) {
                                    if (path->getLast()->getElement() == C.first.first) {
                                        p->setDismissable(path);
                                        createPath(C.first.first, C.first.second, *rule, new Path(*path));
                                    }
                                }
                            }
                        }
                    } else {
                        createPath(C.first.first, C.first.second, *rule);
                    }

                }
            }

            errs() << "Backtrack done\n";


            for (auto &rule : ruleWorklist) {
                rule->removeDismissablePaths();
                errs() << rule->getRuleTitle() << "\n"  ;
                rule->checkRule();
            }

            std::vector<Rule*> checkRules(ruleWorklist);
            ruleWorklist.clear();
            for (auto &rule : checkRules) {
                for (auto &path : rule->getPaths()) {
                    if (path->getLast()->getType() == PathElementBase::ConstAddressElement) {
                        if (((ConstPathElement*)path->getLast())->shouldCreateNewCriterion()) {
                            assert(true);
                            Rule *newRule = new Rule(*rule, path->getLast()->getElement());
                            if (!addRule(newRule))
                                delete(newRule);
                        }
                    }
                }
            }
        }

    void StaticSlicer::computeSlice() {
        while(ruleWorklist.size()) {
            ruleIteration();
        }
    }

    bool StaticSlicer::sliceModule() {
      bool modified = false;
      for (Slicers::iterator s = slicers.begin(); s != slicers.end(); ++s)
        modified |= s->second->slice();
      if (modified)
        for (Module::iterator I = module.begin(), E = module.end(); I != E; ++I)
          if (!I->isDeclaration())
            FunctionStaticSlicer::removeUndefs(MP, *I);

      return modified;
    }

        bool StaticSlicer::addRule(Rule *rule) {
            if (std::find_if(rules.begin(), rules.end(), [&](Rule *other) {return *rule == *other;}) != rules.end())
                return false;
            rules.push_back(rule);
            ruleWorklist.push_back(rule);
            return true;
        }

        void StaticSlicer::findInitialCriterions() {
            for (Module::iterator f = module.begin(); f != module.end(); ++f) {
                if (!f->isDeclaration() && !memoryManStuff(&*f)) {
                    FunctionStaticSlicer *FSS = slicers[f];
                    bool hadAssert = slicing::findInitialCriterion(*f, *FSS, ruleWorklist);

                    if (hadAssert) {
                        initFuns.push_back(f);
                        FSS->initializeInfos();
                    }
                }
            }
        }

        void StaticSlicer::addInitialSlicingCriterion(const Instruction *C) {
            InitialCriterions.insert(C);
        }

        InsInfo *StaticSlicer::getInsInfo(const Instruction *I) {
            if (!I) {
                return nullptr;
            }
            const Function *f = I->getParent()->getParent();
            FunctionStaticSlicer *fss = slicers[f];
            if (!fss->isInitialized())
                return nullptr;
            return fss->getInsInfo(I);
        }
}}


char Slicer::ID = 0;

static RegisterPass<Slicer> X("slice-inter", "View CFG of function", false, true);
static RegisterAnalysisGroup<Slicer> Y(X);


bool Slicer::runOnModule(Module &M) {
  ptr::PointsToSets *PS = new ptr::PointsToSets();
  {
    ptr::ProgramStructure P(M);
    computePointsToSets(P, *PS);
  }

  callgraph::Callgraph CG(M, *PS);

  mods::Modifies MOD;
  {
    mods::ProgramStructure P1(M, *PS);
    computeModifies(P1, CG, *PS, MOD);
  }
    errs() << "done\n";

    using llvm::slicing::Rule;
    using llvm::slicing::Constraint;
    using llvm::slicing::Parameter;

    std::vector<Rule*> rules = llvm::slicing::parseRules();

    for (auto &r : rules) {
        if (r->getParentRuleTitle().size()) {
            for (auto &r2 : rules) {
                if (r2->getRuleTitle() == r->getParentRuleTitle()) {
                    r->setParentRule(r2);
                }
            }
        }
    }

  slicing::StaticSlicer SS(this, M, (*PS), CG, MOD, rules);
  SS.computeSlice();

    free(PS);
    bool s = SS.sliceModule();



    raw_fd_ostream *report_stream = nullptr;
    if (ReportFilename.length()) {
        std::error_code EC;

        report_stream = new raw_fd_ostream(ReportFilename, EC, sys::fs::F_None);

        if (EC) {
            errs() << EC.message() << '\n';
        }
    }

    llvm::slicing::HTMLReportPrinter reportPrinter(report_stream ? *report_stream : nulls());
    for (auto &rule : SS.getRules()) {
        errs() << "Print results of \"" << rule->getRuleTitle() << "\"\n";
        const Rule::CompletePathResultList_t &results = rule->getResults();
        errs() << results.size() << " paths\n";
        reportPrinter.addResults(rule, results);
    }
    reportPrinter.close();

    if (report_stream) {
        report_stream->close();
        delete (report_stream);
    }
  return s;
}

void Slicer::getAnalysisUsage(AnalysisUsage &AU) const {
    AU.addRequired<PostDominatorTree>();
    AU.addRequired<PostDominanceFrontier>();
}