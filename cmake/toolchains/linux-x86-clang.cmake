set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)

set(INKNIT_CLANG_CPU_FLAGS  "--target=i686-linux-gnu -m32 -march=pentium4 -mfpmath=sse")
set(INKNIT_CLANG_LINK_FLAGS "-m32")

include(${CMAKE_CURRENT_LIST_DIR}/common-clang.cmake)

unset(INKNIT_CLANG_CPU_FLAGS)
unset(INKNIT_CLANG_LINK_FLAGS)
