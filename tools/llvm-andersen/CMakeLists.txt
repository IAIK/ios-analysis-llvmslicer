set(LLVM_LINK_COMPONENTS
  BitReader
  Core
  Support
  IRReader
  Analysis
  Slicer
  )

add_llvm_tool(llvm-andersen
        llvm-andersen.cpp
  )
