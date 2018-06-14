//===-- llc.cpp - Implement the LLVM Native Code Generator ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This is the llc code generator driver. It provides a convenient
// command-line interface for generating native assembly-language code
// or C code, given LLVM bitcode.
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
#include <llvm/Analysis/Andersen/NonVolatileRegistersPass.h>
#include "llvm/Analysis/Andersen/StackAccessPass.h"
#include "llvm/Analysis/Andersen/DetectParametersPass.h"
#include "llvm/IR/Dominators.h"
using namespace llvm;

// General options for llc.  Other pass-specific options are specified
// within the corresponding llc passes, and target-specific options
// and back-end code generation options are specified with the target machine.
//
static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input bitcode>"), cl::init("-"));


// main - Entry point for the llc compiler.
//
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

    legacy::PassManager *PM = new legacy::PassManager();
//    PM->add(new LoopInfoWrapperPass());
//    PM->add(new StackAccessPass());
//
//    PM->add(new DetectParametersPass());
//    PM->add(new DominatorTreeWrapperPass());
    PM->add(new NonVolatileRegistersPass());

    PM->run(*Mod);

    Mod->dump();
  return 0;
}

