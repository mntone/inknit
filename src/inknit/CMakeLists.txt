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
		$<$<BOOL:${INKNIT_ENABLE_FOLD}>:INKNIT_ENABLE_FOLD>
		$<$<BOOL:${INKNIT_ENABLE_UNROLL}>:INKNIT_ENABLE_UNROLL>
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
	$<$<NOT:$<STREQUAL:${CMAKE_BUILD_TYPE},Release>>:assertion.c>
	shared/draw_line.c
	$<$<BOOL:${INKNIT_SHARED}>:x1/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x1lsb/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x2/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x2lsb/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x4/exports.c>
	$<$<BOOL:${INKNIT_SHARED}>:x4lsb/exports.c>
	)
if(INKNIT_PROCESSOR STREQUAL "x86")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		INKNIT_ENABLE_DRAW_POINT_ASM
		INKNIT_ENABLE_DRAW_VLINE_ASM
		)
	target_sources(${PROJECT_NAME} PRIVATE
		x1/draw_point_x86.s
		x1lsb/draw_point_x86.s
		x2/draw_point_x86.s
		x2lsb/draw_point_x86.s
		x4/draw_point_x86.s
		x4lsb/draw_point_x86.s

		x1/draw_vline_x86.s
		x1lsb/draw_vline_x86.s
		x2/draw_vline_x86.s
		x2lsb/draw_vline_x86.s
		x4/draw_vline_x86.s
		x4lsb/draw_vline_x86.s
		)
elseif(INKNIT_PROCESSOR STREQUAL "armv6m")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		INKNIT_ENABLE_DRAW_POINT_ASM
		INKNIT_ENABLE_DRAW_VLINE_ASM
		)
	target_sources(${PROJECT_NAME} PRIVATE
		x1/draw_point_armv6m.s
		x1lsb/draw_point_armv6m.s
		x2/draw_point_armv6m.s
		x2lsb/draw_point_armv6m.s
		x4/draw_point_armv6m.s
		x4lsb/draw_point_armv6m.s

		x1/draw_vline_armv6m.s
		x1lsb/draw_vline_armv6m.s
		x2/draw_vline_armv6m.s
		x2lsb/draw_vline_armv6m.s
		x4/draw_vline_armv6m.s
		x4lsb/draw_vline_armv6m.s
		)
elseif(INKNIT_PROCESSOR STREQUAL "armv8m")
	target_compile_definitions(${PROJECT_NAME} PRIVATE
		INKNIT_ENABLE_DRAW_POINT_ASM
		INKNIT_ENABLE_DRAW_VLINE_ASM
		)
	target_sources(${PROJECT_NAME} PRIVATE
		x1/draw_point_armv8m.s
		x1lsb/draw_point_armv8m.s
		x2/draw_point_armv8m.s
		x2lsb/draw_point_armv8m.s
		x4/draw_point_armv8m.s
		x4lsb/draw_point_armv8m.s

		x1/draw_vline_armv8m.s
		x1lsb/draw_vline_armv8m.s
		x2/draw_vline_armv8m.s
		x2lsb/draw_vline_armv8m.s
		x4/draw_vline_armv8m.s
		x4lsb/draw_vline_armv8m.s
		)
endif()

if(INKNIT_ENABLE_OBJDUMP)
	inknit_enable_disassemble(${PROJECT_NAME})
endif()
