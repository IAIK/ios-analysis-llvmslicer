file(GLOB CallHandlerSRC
        "CallHandler/*.cpp"
        )

add_llvm_loadable_module(LLVMStackAccessPass
        DetectParametersPass.cpp
        Andersen.cpp
        AndersenAA.cpp
        ConstraintCollect.cpp
        ConstraintOptimize.cpp
        ConstraintSolving.cpp
        ExternalLibrary.cpp
        NodeFactory.cpp
        StackAccessPass.cpp
        ObjectiveCBinary.cpp
        ObjectiveCClassInfo.cpp
        ObjCCallHandler.cpp
        CallHandler/ObjCRuntimeCallHandler.cpp
        NonVolatileRegistersPass.cpp
        CleanUpPass.cpp
        SimpleCallGraph.cpp
        )

add_dependencies(LLVMStackAccessPass LLVMAnalysis Object)
