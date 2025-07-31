if(BUILD_SHARED_LIBS OR INKNIT_BUILD_SHARED_LIBS)
	set(INKNIT_SHARED 1)
	add_library(${PROJECT_NAME} SHARED)
else()
	set(INKNIT_SHARED 0)
	add_library(${PROJECT_NAME} STATIC)
endif()

target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		INKNIT_EXPORTS
		$<$<BOOL:${INKNIT_DISABLE_LUT}>:INKNIT_DISABLE_LUT>
		$<$<BOOL:${INKNIT_ENABLE_EXPORTS}>:INKNIT_ENABLE_EXPORTS>
		$<$<BOOL:${INKNIT_SHARED}>:INKNIT_EXPORTS>
		$<$<BOOL:${INKNIT_USE_MEMORY_H}>:INKNIT_USE_MEMORY_H>
		$<$<STREQUAL:${INKNIT_TARGET},pico>:INKNIT_TARGET_IS_PICO>
	PUBLIC
		$<$<BOOL:${INKNIT_SHARED}>:INKNIT_SHARED>
		$<$<BOOL:${INKNIT_ENABLE_BENCHMARKS}>:INKNIT_ENABLE_BENCHMARKS>
	)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/inc
	)

target_precompile_headers(${PROJECT_NAME} PRIVATE
	<stdbool.h>
	<stddef.h>
	<stdint.h>
	<memory.h>
	)

inknit_generate_source_files(${PROJECT_NAME}
	PIXEL_LAYOUT "x1" "x1lsb" "x2" "x2lsb" "x4" "x4lsb"
	TEMPLATE_DIR "${CMAKE_CURRENT_LIST_DIR}/templates"
	)

target_sources(${PROJECT_NAME} PRIVATE
	$<$<BOOL:${INKNIT_SHARED}>:x1lsb/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x2lsb/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x4lsb/exports.c>
	)

if(INKNIT_ENABLE_OBJDUMP)
	inknit_enable_disassemble(${PROJECT_NAME})
endif()
