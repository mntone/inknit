enable_language(CXX)

add_executable(${PROJECT_NAME}_benchmarks
	utils/monitors/quantiler.cpp
	utils/monitors/welford.cpp
	utils/base.cpp
	utils/boxplot.cpp
	utils/executor.cpp
	utils/registry.cpp
	utils/scale.cpp
	$<IF:$<BOOL:${WIN32}>,utils/platform-windows.cpp,utils/platform-unix.cpp>

	benchmark_main.cpp
	blit.cpp
	circle.cpp
	hline.cpp
	line.cpp
	vline.cpp
	)

# Enable link-time optimization on release build
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	include(CheckIPOSupported)
	check_ipo_supported(RESULT ipo_supported OUTPUT ipo_output)
	if(ipo_supported)
		set_property(TARGET ${PROJECT_NAME}_benchmarks PROPERTY
			INTERPROCEDURAL_OPTIMIZATION TRUE
			)
	else()
		message(WARNING "IPO is not supported: ${output}")
	endif()
endif()

target_include_directories(${PROJECT_NAME}_benchmarks PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
	)

target_link_libraries(${PROJECT_NAME}_benchmarks PRIVATE
	inknit_cxx
	)

if(MINGW)
	# On MinGW environments, weak symbols such as std::__is_constant_evaluated
	# may cause multiple definition errors during linking, especially when
	# statically linking libstdc++ or using multiple translation units that
	# inline standard library functions.
	#
	# This issue is specific to MinGW and does not occur on Linux or macOS,
	# where weak symbols are handled correctly by the linker via COMDAT folding.
	#
	# To work around this problem, we allow multiple definitions during linking.
	target_link_options(${PROJECT_NAME}_benchmarks PRIVATE
		-Wl,--allow-multiple-definition
		)
endif()

target_precompile_headers(${PROJECT_NAME}_benchmarks PRIVATE
	<any>
	<array>
	<cstdint>
	<cstdio>
	<functional>
	<memory>
	<string>
	<string_view>
	<type_traits>
	<unordered_map>
	<variant>
	<vector>
	)

inknit_copy_dlls(${PROJECT_NAME}_benchmarks)
