set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(INKNIT_TOOLCHAIN_PREFIX "i686-w64-mingw32" CACHE INTERNAL "")

set(INKNIT_GCC_CPU_FLAGS "-march=pentium4 -mfpmath=sse")

include(${CMAKE_CURRENT_LIST_DIR}/common-gcc.cmake)

unset(INKNIT_GCC_CPU_FLAGS)
