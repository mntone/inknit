include("${CMAKE_CURRENT_LIST_DIR}/../InknitUtils.cmake" NO_POLICY_SCOPE)
inknit_load_dotenv("${CMAKE_CURRENT_LIST_DIR}/../../.env")

set(CMAKE_SYSTEM_NAME Pico)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0plus)

if(PICO_TOOLCHAIN)
	set(INKNIT_TOOLCHAIN_PREFIX "${PICO_TOOLCHAIN}" CACHE INTERNAL "")
else()
	set(INKNIT_TOOLCHAIN_PREFIX "arm-none-eabi" CACHE INTERNAL "")
endif()
set(INKNIT_GCC_CPU_FLAGS "-mcpu=cortex-m0plus -mthumb -mno-unaligned-access")

include(${CMAKE_CURRENT_LIST_DIR}/pico-common-gcc.cmake)

unset(INKNIT_GCC_CPU_FLAGS)
