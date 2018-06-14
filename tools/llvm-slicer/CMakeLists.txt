set(LLVM_LINK_COMPONENTS
  IRReader
  Core
  Support
  Slicer
  )

add_llvm_tool(llvm-slicer
  llvm-slicer.cpp
  )
