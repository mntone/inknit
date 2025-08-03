cmake_minimum_required(VERSION 3.21)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Bootstrap.cmake" NO_POLICY_SCOPE)

project(inknit
	LANGUAGES ASM C
	)

include(CompilerConfig)
include(InknitUtils)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Pico")
	inknit_load_dotenv("${CMAKE_SOURCE_DIR}/.env")
endif()

add_subdirectory(src)
if(NOT INKNIT_BUILD_LIB_ONLY)
	add_subdirectory(tests)
	if(INKNIT_ENABLE_BENCHMARKS)
		add_subdirectory(benchmarks)
	endif()
endif()
