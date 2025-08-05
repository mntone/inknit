enable_language(CXX)
include(FindDoctest)

add_executable(${PROJECT_NAME}_tests
	draw/circle.cpp
	draw/ellipse.cpp
	draw/hline.cpp
	draw/line.cpp
	draw/point.cpp
	draw/rect.cpp
	draw/rectp.cpp
	draw/vline.cpp
	$<$<NOT:$<STREQUAL:${CMAKE_BUILD_TYPE},Release>>:utils/assert_helper.cpp>
	blit.cpp
	clear.cpp
	test_main.cpp
	)

target_compile_definitions(${PROJECT_NAME}_tests PRIVATE
	ENABLED_COLOR_DUMP
	)

target_include_directories(${PROJECT_NAME}_tests PRIVATE
	.
	)

target_link_libraries(${PROJECT_NAME}_tests
	doctest::doctest
	inknit_cxx
	)

target_precompile_headers(${PROJECT_NAME}_tests PRIVATE
	<algorithm>
	<concepts>
	<cstdint>
	<sstream>
	<type_traits>
	<vector>
	)

inknit_copy_dlls(${PROJECT_NAME}_tests)
