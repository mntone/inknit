enable_language(CXX)

add_library(${PROJECT_NAME}_cxx STATIC)

target_compile_definitions(${PROJECT_NAME}_cxx PRIVATE
	$<$<BOOL:${INKNIT_SHARED}>:INKNIT_CXX_EXPORTS>
	$<$<STREQUAL:${INKNIT_TARGET},pico>:INKNIT_TARGET_IS_PICO>
	)

target_include_directories(${PROJECT_NAME}_cxx PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	)

target_link_libraries(${PROJECT_NAME}_cxx PUBLIC
	inknit
	)

target_sources(${PROJECT_NAME}_cxx PRIVATE
	$<$<BOOL:${UNIX}>:format/auto-posix.cpp>
	$<$<NOT:$<STREQUAL:${INKNIT_TARGET},pico>>:format/block.cpp>
	format/character.cpp
	$<$<NOT:$<STREQUAL:${INKNIT_TARGET},pico>>:format/sixel.cpp>
	)
