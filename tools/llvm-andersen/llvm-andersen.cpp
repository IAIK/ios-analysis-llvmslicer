//===-- llvm-dis.cpp - The low-level LLVM disassembler --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This utility may be invoked in the following manner:
//  llvm-dis [options]      - Read LLVM bitcode from stdin, write asm to stdout
//  llvm-dis [options] x.bc - Read LLVM bitcode from the x.bc file, write asm
//                            to the x.ll file.
//  Options:
//      --help   - Output information about command line switches
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/LLVMContext.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/DataStream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/ToolOutputFile.h"
#include <system_error>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/Analysis/Andersen/NonVolatileRegistersPass.h>
#include <llvm/IR/PatternMatch.h>
#include "llvm/Analysis/Andersen/Andersen.h"
#include "llvm/Analysis/Andersen/StackAccessPass.h"
#include "llvm/Analysis/Andersen/DetectParametersPass.h"
#include "llvm/IR/Dominators.h"

using namespace llvm;

static cl::opt<std::string>
        InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));

cl::opt<std::string> CallgraphFile("callgraph", cl::desc(""), cl::init(""), cl::Hidden);

static cl::opt<bool>
        Replace("replace-calls", cl::desc("If there is only a single candidate for a call it gets replaced"),
                     cl::init(false));

static cl::opt<bool>
        NoPrint("no-print", cl::desc(""),
                cl::init(false));

static cl::list<std::string>
        ExtractFuncs("func", cl::desc("Specify function to extract"),
                     cl::ZeroOrMore, cl::value_desc("function"));

int main(int argc, char **argv) {
    sys::PrintStackTraceOnErrorSignal();
    PrettyStackTraceProgram X(argc, argv);

    LLVMContext &Context = getGlobalContext();
    llvm_shutdown_obj Y;  // Call llvm_shutdown() on exit.

    cl::ParseCommandLineOptions(argc, argv, "llvm .bc -> .ll disassembler\n");


    SMDiagnostic Err;
    std::unique_ptr<Module> Mod = parseIRFile(InputFilename, Err, getGlobalContext());
    if (!Mod) {
        Err.print(argv[0], errs());
        return 1;
    }

    raw_ostream *callgraph = nullptr;
    if (!CallgraphFile.length())
        callgraph = &nulls();
    else {
        std::error_code EC;

        callgraph = new raw_fd_ostream(CallgraphFile, EC, sys::fs::F_None);

        if (EC) {
            errs() << EC.message() << '\n';
            callgraph = &nulls();
        }
    }

//    Mod->dump();
    legacy::PassManager *LPM = new legacy::PassManager();
    LPM->add(new LoopInfoWrapperPass());
    LPM->add(new DominatorTreeWrapperPass());
    LPM->add(new StackAccessPass());
    LPM->add(new DetectParametersPass());
    Andersen *andersen = new Andersen();
    LPM->add(andersen);
    LPM->run(*Mod);

    andersen->getCallGraph().print(*callgraph);

    if (Replace) {
        SimpleCallGraph &cg = andersen->getCallGraph();
        for (Module::FunctionListType::iterator F_it = Mod->getFunctionList().begin(); F_it != Mod->getFunctionList().end() ; ++F_it) {
            for (inst_iterator I_it = inst_begin(*F_it); I_it != inst_end(*F_it); ++I_it) {
                if (I_it->getOpcode() == Instruction::Call) {
                    SimpleCallGraph::FunctionSet_t called = cg.getCalled(&*I_it);
                    if (called.size() == 1) {
                        std::string functionName = *called.begin();
                        Function *function = Mod->getFunction(functionName);
                        if (!function) {
                            function = dyn_cast<Function>(Mod->getOrInsertFunction(functionName, Type::getVoidTy(getGlobalContext()), I_it->getParent()->getParent()->getArgumentList().begin()->getType(), NULL));
                        }
                        assert(function);
                        if (function) {
                            CallInst *call = (CallInst*)&*I_it;
                            call->setCalledFunction(function);
                        }
                    }
                }
            }
        }


        std::error_code EC;
        std::unique_ptr<tool_output_file> FDOut = llvm::make_unique<tool_output_file>(InputFilename, EC,
                                                                                      sys::fs::F_None);
        if (EC) {
            errs() << EC.message() << '\n';
            return -1;
        }

        FDOut->os() << *Mod;

        FDOut->keep();

        return 0;
    }

    if(NoPrint) {
        return 0;
    }

    for (Module::FunctionListType::iterator F_it = Mod->getFunctionList().begin(); F_it != Mod->getFunctionList().end() ; ++F_it) {
        bool extract = ExtractFuncs.size() == 0 ? true : false;
        for (auto &e : ExtractFuncs) {
            if (e == F_it->getName()) {
                extract = true;
                break;
            }
        }
        if (!extract)
            continue;
        errs() << "###############################################################################################\n";
        errs() << (F_it->hasName() ? F_it->getName() : "UNNAMED") << "\n";
        for (inst_iterator I_it = inst_begin(*F_it); I_it != inst_end(*F_it); ++I_it) {

            Value *I = &*I_it;
            Instruction *Inst = &*I_it;

            if (Inst->getOpcode() == Instruction::Load) {
                if (PatternMatch::match(Inst->getOperand(0), PatternMatch::m_IntToPtr(PatternMatch::m_Constant()))) {
//                    I = Inst->getOperand(0);
                }
            } else if (Inst->getOpcode() == Instruction::Store) {
//                if (PatternMatch::match(Inst->getOperand(1), PatternMatch::m_IntToPtr(PatternMatch::m_ConstantInt()))) {
                    I = Inst->getOperand(1);
//                }
            }
            Andersen::StringSet_t Types;

            std::vector<const llvm::Value*> PtsTo;
            andersen->getPointsToSet(I, PtsTo);
            if (PtsTo.size()) {
                errs() << "\n";
                Inst->dump();
                errs() << "PtsTo: " << PtsTo.size() << "\n";

                for (unsigned i = 0; i < PtsTo.size(); ++i) {
                    errs() << *(PtsTo[i]);
                    const Value *src = andersen->getNodeFactory().getLocation(PtsTo[i]);
                    errs() << *(PtsTo[i]) << " ";
                    if (src) {
                        errs() << *src;
                        if (const Instruction *srcInst = dyn_cast<const Instruction>(src)) {
                            errs() << "(" << srcInst->getParent()->getParent()->getName() << ")";
                        }
                    }
                    errs() << "\n";
                    if (andersen->getType((Value*)PtsTo[i], Types)) {
                        for (auto &Type : Types) {
                            errs() << "Type: " << Type << "\n";
                        }
                        errs() << "\n";
                    }
                }

            }
        }
    }

    return 0;
}
