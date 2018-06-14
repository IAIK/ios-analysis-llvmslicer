set(LLVM_LINK_COMPONENTS
        BitReader
        Core
        Support
        IRReader
        Analysis
        TransformUtils
)

add_llvm_tool(llvm-testpass
  llvm-testpass.cpp
)

