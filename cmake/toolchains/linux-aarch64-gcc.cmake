set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(INKNIT_TOOLCHAIN_PREFIX "aarch64-linux-gnu" CACHE INTERNAL "")

set(INKNIT_GCC_CPU_FLAGS "-march=armv8-a")

include(${CMAKE_CURRENT_LIST_DIR}/common-gcc.cmake)

unset(INKNIT_GCC_CPU_FLAGS)
