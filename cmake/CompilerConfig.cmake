include_guard(GLOBAL)

# Set version
set(CMAKE_PROJECT_VERSION "${INKNIT_CMAKE_VERSION}")

# Set build configuration
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
	message(FATAL_ERROR "CMAKE_BUILD_TYPE is not allowed. Please use our custom build system.")
endif()
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo" CACHE STRING "" FORCE)

# Add "d" suffix on debug/relwithdebinfo build
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RELWITHDEBINFO_POSTFIX d)

# Set C and C++ language standards to C17 and C++23
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	# Set symbols to be hidden by default for C and C++
	set(CMAKE_C_VISIBILITY_PRESET hidden)
	set(CMAKE_CXX_VISIBILITY_PRESET hidden)
	set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)
endif()

# Remove prefix "lib" from libraries for Windows w/GNU
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_IMPORT_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_LIBRARY_PREFIX "")
	set(CMAKE_SHARED_MODULE_PREFIX  "")
	set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif()

# Enable security flags
set(SECURITY_STACK_CLASH_PROTECTION_FLAG OFF CACHE INTERNAL "")
set(SECURITY_STRICT_FLEX_ARRAYS_FLAG     0   CACHE INTERNAL "")

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	if(NOT INKNIT_TARGET STREQUAL "pico")
		set(SECURITY_STACK_CLASH_PROTECTION_FLAG ON)
	endif()

	if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "13")
		set(SECURITY_STRICT_FLEX_ARRAYS_FLAG 3)
	endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
	if(NOT CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
		set(SECURITY_STACK_CLASH_PROTECTION_FLAG ON)
	endif()

	if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "16")
		set(SECURITY_STRICT_FLEX_ARRAYS_FLAG 3)
	elseif(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL "15")
		set(SECURITY_STRICT_FLEX_ARRAYS_FLAG 2)
	endif()
endif()

if(APPLE)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
	if(UNIX)
		add_compile_definitions(
			$<$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>:_FORTIFY_SOURCE=1>
			$<$<STREQUAL:${CMAKE_BUILD_TYPE},Debug>:_GLIBCXX_ASSERTIONS>
			)
		add_compile_options(
			$<$<BOOL:${SECURITY_STACK_CLASH_PROTECTION_FLAG}>:-fstack-clash-protection>
			-fstack-protector-strong
			$<$<BOOL:${SECURITY_STRICT_FLEX_ARRAYS_FLAG}>:-fstrict-flex-arrays=${SECURITY_STRICT_FLEX_ARRAYS_FLAG}>
			-fno-plt
			$<$<STREQUAL:CMAKE_SYSTEM_PROCESSOR,x86_64>:-fcf-protection=full>
			$<$<STREQUAL:CMAKE_SYSTEM_PROCESSOR,aarch64>:-mbranch-protection=standard>
			)
		add_link_options(
			-Wl,-z,relro
			-Wl,-z,nodlopen
			-Wl,-z,noexecstack
			-Wl,-z,now
			)
	elseif(WIN32)
		add_compile_options(
			-fno-plt
			)
	else()
		add_compile_options(
			-fno-plt
			)
		add_link_options(
			-Wl,-z,nodlopen
			-Wl,-z,noexecstack
			)
	endif()
endif()

# compiler options for C and C++
if(MSVC)
	add_compile_definitions(
		NOMINMAX
		)
	add_compile_options(
		/wd4100
		/wd4133
		/wd4189
		/wd4244  # conversion from 'type1' to 'type2', possible loss of data
		/wd4267  # conversion from 'size_t' to 'type', possible loss of data
		/wd4305  # conversion from 'double' to 'float', possible loss of data
		/wd4311
		/wd4456
		/wd5054  # deprecated between enumerations of different types
		/wd5262
		$<$<COMPILE_LANGUAGE:CXX>:/EHc>   # disable C++ exceptions
		$<$<COMPILE_LANGUAGE:CXX>:/GR->>  # disable C++ RTTI
		)
else()
	add_compile_options(
		-Wcast-function-type
		-Wconversion
		-Wenum-conversion
		-Wfloat-equal
		-Wformat
		-Wformat-security
		-Wformat-y2k
		-Wint-to-pointer-cast
		-Wunused-parameter
		-Wunused-variable
		$<IF:$<STREQUAL:${CMAKE_C_COMPILER_ID},Clang>,-Wimplicit-fallthrough,-Wimplicit-fallthrough=3>
		$<$<COMPILE_LANGUAGE:C>:-Wpointer-to-int-cast>
		$<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>  # disable C++ exceptions
		$<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>        # disable C++ RTTI
		)
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(
		_DEBUG
		)
endif()

if(NOT DEFINED CMAKE_COMPILE_WARNING_AS_ERROR)
	set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif()

set(INKNIT_DISABLE_LUT       FALSE CACHE BOOL   "Disable internal LUT.")
set(INKNIT_ENABLE_BENCHMARKS TRUE  CACHE BOOL   "Enable benchmarking utilities.")
set(INKNIT_ENABLE_EXPORTS    FALSE CACHE BOOL   "Export c struct vtbl")
set(INKNIT_ENABLE_OBJDUMP    FALSE CACHE BOOL   "Use objdump if specified project")
set(INKNIT_USE_MEMORY_H      TRUE  CACHE BOOL   "Use memory.h like memcpy")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86" OR CMAKE_SYSTEM_PROCESSOR MATCHES "i[3-6]86")
	set(INKNIT_PROCESSOR "x86"     CACHE STRING "System processor (e.g., x86, armv6m)")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	set(INKNIT_PROCESSOR "x86_64"  CACHE STRING "System processor (e.g., x86, armv6m)")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m0plus")
	set(INKNIT_PROCESSOR "armv6m"  CACHE STRING "System processor (e.g., x86, armv6m)")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "cortex-m33")
	set(INKNIT_PROCESSOR "armv8m"  CACHE STRING "System processor (e.g., x86, armv6m)")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
	set(INKNIT_PROCESSOR "aarch64" CACHE STRING "System processor (e.g., x86, armv6m)")
else()
	message(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR is not allowed. Please use our custom build system.")
endif()

message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
