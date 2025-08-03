set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)
set(INKNIT_OBJDUMP_FLAGS "--x86-asm-syntax=intel" CACHE INTERNAL "")

set(INKNIT_CLANG_CPU_FLAGS "--target=x86_64-linux-gnu -march=x86-64-v3")

include(${CMAKE_CURRENT_LIST_DIR}/common-clang.cmake)

unset(INKNIT_CLANG_CPU_FLAGS)
