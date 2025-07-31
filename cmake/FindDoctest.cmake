if(DOCTEST_HOME)
	set(doctest_HOME "${DOCTEST_HOME}")
endif()

if(doctest_HOME AND EXISTS "${doctest_HOME}/doctest/doctest.h")
	message(STATUS "Using doctest from DOCTEST_HOME: ${doctest_HOME}")
	add_library(doctest INTERFACE)
	target_include_directories(doctest INTERFACE "${doctest_HOME}")
	add_library(doctest::doctest ALIAS doctest)

	set(doctest_INCLUDE_DIR "${doctest_HOME}/doctest")
else()
	include(FetchContent)
	find_package(Git REQUIRED)

	FetchContent_Declare(
		doctest
		GIT_REPOSITORY https://github.com/doctest/doctest.git
		GIT_TAG        v2.4.12
		)
	FetchContent_MakeAvailable(doctest)

	set(doctest_INCLUDE_DIR "${doctest_SOURCE_DIR}/doctest")
endif()

if(EXISTS "${doctest_INCLUDE_DIR}/doctest.h")
	file(STRINGS "${doctest_INCLUDE_DIR}/doctest.h" doctest_HEADER_CONTENTS REGEX "#define DOCTEST_VERSION_[A-Z]+ [0-9]+")

	string(REGEX REPLACE ".*#define DOCTEST_VERSION_MAJOR ([0-9]+).*" "\\1" doctest_VERSION_MAJOR "${doctest_HEADER_CONTENTS}")
	string(REGEX REPLACE ".*#define DOCTEST_VERSION_MINOR ([0-9]+).*" "\\1" doctest_VERSION_MINOR "${doctest_HEADER_CONTENTS}")
	string(REGEX REPLACE ".*#define DOCTEST_VERSION_PATCH ([0-9]+).*" "\\1" doctest_VERSION_PATCH "${doctest_HEADER_CONTENTS}")

	set(doctest_VERSION_STRING "${doctest_VERSION_MAJOR}.${doctest_VERSION_MINOR}.${doctest_VERSION_PATCH}")
	set(doctest_VERSION        "${doctest_VERSION_STRING}")
	unset(doctest_HEADER_CONTENTS)
else()
	if(NOT doctest_FIND_QUIETLY)
		message(AUTHOR_WARNING "Failed to find doctest version.")
	endif()
	set(doctest_VERSION 0.0.0)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
	doctest
	REQUIRED_VARS doctest_INCLUDE_DIR
	VERSION_VAR doctest_VERSION
	REASON_FAILURE_MESSAGE "Ensure that doctest is installed on the system."
	)
mark_as_advanced(doctest_INCLUDE_DIR)

include(FeatureSummary)
set_package_properties(
	doctest
	PROPERTIES
		URL "https://github.com/doctest/doctest"
		DESCRIPTION "The fastest feature-rich C++11/14/17/20/23 single-header testing framework"
	)
