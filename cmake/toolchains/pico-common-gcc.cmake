set(INKNIT_BUILD_SHARED_LIBS 0)

set(INKNIT_GCC_LINK_FLAGS "-Wl,--build-id=none")

include(${CMAKE_CURRENT_LIST_DIR}/common-gcc.cmake)

unset(INKNIT_GCC_LINK_FLAGS)


set(CMAKE_EXE_LINKER_FLAGS_INIT "-nostartfiles")
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
foreach(LANG IN ITEMS C CXX ASM)
	if(IS_IN_TRY_COMPILE)
		set(CMAKE_${LANG}_LINK_FLAGS "${CMAKE_${LANG}_LINK_FLAGS} -nostdlib")
	endif()
endforeach()
