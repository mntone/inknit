set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(INKNIT_CLANG_CPU_FLAGS "--target=aarch64-linux-gnu -march=armv8-a")

include(${CMAKE_CURRENT_LIST_DIR}/common-clang.cmake)

unset(INKNIT_CLANG_CPU_FLAGS)
