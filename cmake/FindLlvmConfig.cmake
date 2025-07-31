# FindLlvmConfig.cmake
# Usage: cmake -P FindLlvmConfig.cmake

set(_llvm_search_paths "/usr/bin" "/usr/local/bin" "/opt/llvm/bin")

set(_found_configs "")
foreach(_path IN LISTS _llvm_search_paths)
	file(GLOB _llvm_bins RELATIVE "${_path}" "${_path}/llvm-config-[0-9]*")
	foreach(_bin IN LISTS _llvm_bins)
		list(APPEND _found_configs "${_path}/${_bin}")
	endforeach()
endforeach()

set(_max_version 0)
set(_selected_config "")

foreach(_config IN LISTS _found_configs)
	string(REGEX MATCH "llvm-config-([0-9]+)" _match "${_config}")
	if(_match)
		string(REGEX REPLACE ".*-([0-9]+)$" "\\1" _ver "${_config}")
		if(_ver GREATER _max_version)
			set(_max_version "${_ver}")
			set(_selected_config "${_config}")
		endif()
	endif()
endforeach()

if(NOT _selected_config)
	find_program(_selected_config NAMES llvm-config)
endif()

if(NOT _selected_config)
	message(FATAL_ERROR "No suitable llvm-config-* found in search paths")
endif()

set(LLVM_CONFIG "${_selected_config}" CACHE FILEPATH "Auto-detected llvm-config")
message(STATUS "Selected llvm-config: ${LLVM_CONFIG}")
