include_guard(GLOBAL)

function(inknit_copy_dlls target)
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		add_custom_command(
			TARGET ${target} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "Running DLL extraction..."
			COMMAND bash "${CMAKE_SOURCE_DIR}/scripts/copy_dlls.sh"
				"$<TARGET_FILE:${target}>"
				"${INKNIT_TOOLCHAIN_PREFIX}"
			COMMENT "Extracting dependent DLLs using objdump and -print-file-name"
			VERBATIM
			)
	endif()
endfunction()

function(inknit_set_file_readonly file)
	if(EXISTS "${file}")
		if(UNIX)
			execute_process(COMMAND chmod a-w "${file}")
		elseif(WIN32)
			execute_process(COMMAND attrib +R "${file}")
		endif()
	else()
		message(WARNING "inknit_set_file_readonly: File does not exist: ${file}")
	endif()
endfunction()

function(inknit_set_file_writable file)
	if(EXISTS "${file}")
		if(UNIX)
			execute_process(COMMAND chmod u+w "${file}")
		elseif(WIN32)
			execute_process(COMMAND attrib -R "${file}")
		endif()
	else()
		message(WARNING "inknit_set_file_writable: File does not exist: ${file}")
	endif()
endfunction()

function(inknit_enable_disassemble target)
	cmake_parse_arguments("INKNIT_OBJDUMP_${target}" "" "" "OPTIONS" ${ARGN})

	set(INKNIT_OBJDUMP_${target}_COMMAND
		"-d"  # --disassemble
		"--no-show-raw-insn"
		)
	if(NOT "NO_SOURCE" IN_LIST INKNIT_OBJDUMP_${target}_OPTIONS)
		list(APPEND INKNIT_OBJDUMP_${target}_COMMAND "-S")  # --source
	endif()
	if("LINES"  IN_LIST INKNIT_OBJDUMP_${target}_OPTIONS)
		list(APPEND INKNIT_OBJDUMP_${target}_COMMAND "-l")  # --line-numbers
	endif()

	add_custom_command(
		TARGET  ${target} POST_BUILD
		COMMAND ${CMAKE_OBJDUMP} ARGS
			${INKNIT_OBJDUMP_${target}_COMMAND}
			"$<TARGET_FILE:${target}>"
			> "$<TARGET_FILE_PREFIX:${target}>$<TARGET_FILE_BASE_NAME:${target}>.dis"
		VERBATIM
		)
endfunction()

function(inknit_homedir outvar)
	if(MSVC)
		cmake_path(CONVERT $ENV{USERPROFILE} TO_CMAKE_PATH_LIST home)
	else()
		set(home $ENV{HOME})
	endif()

	if(home STREQUAL "/")
		set(home "/")
	else()
		string(REGEX REPLACE "[/]+$" "" home "${home}")
	endif()

	set(${outvar} ${home} PARENT_SCOPE)
endfunction()

function(inknit_expand_tilda str outvar)
	inknit_homedir(home)
	if(home STREQUAL "")
		message(FATAL_ERROR "expanduser(): could not get home directory")
		return()
	endif()

	string(SUBSTRING "${str}" 0 1 str_first)
	if(NOT str_first STREQUAL "~")
		set(${outvar} "${str}" PARENT_SCOPE)
		return()
	endif()

	string(LENGTH "${str}" str_length)
	if(str_length EQUAL 1)
		set(${outvar} "${home}" PARENT_SCOPE)
		return()
	endif()

	if(NOT str MATCHES "^~/")
		set(${outvar} "${str}" PARENT_SCOPE)
		return()
	endif()

	string(SUBSTRING "${str}" 1 -1 rest)
	string(CONCAT temp ${home} ${rest})
	set(${outvar} "${temp}" PARENT_SCOPE)
endfunction()

function(inknit_generate_source_files target)
	cmake_parse_arguments(INKNIT "" "FEATURES;OUTPUT_FILES;TEMPLATE_DIR" "PIXEL_LAYOUT" ${ARGN})
	if(NOT INKNIT_TEMPLATE_DIR)
		message(FATAL_ERROR "Missing required argument: TEMPLATE_DIR")
	endif()
	if(NOT INKNIT_PIXEL_LAYOUT)
		message(FATAL_ERROR "Missing required argument: PIXEL_LAYOUT")
	endif()

	if(INKNIT_FEATURES)
		# Locate template files from each feature directory (e.g., draw/*.[ch].in)
		foreach(feature IN LISTS INKNIT_FEATURES)
			file(GLOB RELATIVE "${INKNIT_TEMPLATE_DIR}"
				feature_files
					"${INKNIT_TEMPLATE_DIR}/${feature}/*.h.in"
					"${INKNIT_TEMPLATE_DIR}/${feature}/*.c.in"
				)
			list(APPEND TEMPLATE_FILES ${feature_files})
		endforeach()

		# Also include templates from the root of TEMPLATE_DIR
		file(GLOB RELATIVE "${TEMPLATE_DIR}" root_files "${TEMPLATE_DIR}/*.c.in")
		list(APPEND TEMPLATE_FILES ${root_files})
	else()
		# Locate all template files (*.[ch].in), paths relative to ${INKNIT_TEMPLATE_DIR}
		file(GLOB_RECURSE TEMPLATE_FILES RELATIVE "${INKNIT_TEMPLATE_DIR}"
			"${INKNIT_TEMPLATE_DIR}/*.h.in"
			"${INKNIT_TEMPLATE_DIR}/*.c.in"
			)
	endif()

	if(TEMPLATE_FILES STREQUAL "")
		message(WARNING "No template files found in ${INKNIT_TEMPLATE_DIR}")
		return()
	endif()

	set(GENERATED_SRCS "")
	foreach(layout IN LISTS INKNIT_PIXEL_LAYOUT)
		# Define layout-specific variables for template substitution
		set(layout_name     "${layout}")
		set(public_prefix   "inknit_${layout}_")
		set(internal_prefix "_ikt${layout}_")

		set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/build/generated/${target}/${layout}")
		set(GENERATED_LAYOUT_SRCS "")
		foreach(in_file ${TEMPLATE_FILES})
			set(in_path "${INKNIT_TEMPLATE_DIR}/${in_file}")
			string(REGEX REPLACE "\\.([ch])\\.in$" ".\\1" base_file "${in_file}")
			set(out_path "${GENERATED_DIR}/${base_file}")

			get_filename_component(out_dir "${out_path}" DIRECTORY)
			file(MAKE_DIRECTORY "${out_dir}")

			# Add a custom command to generate the file at build time
			add_custom_command(
				OUTPUT "${out_path}"
				COMMAND ${CMAKE_COMMAND}
					-DINPUT=${in_path}
					-DOUTPUT=${out_path}
					-DLAYOUT_NAME=${layout_name}
					-DPUBLIC_PREFIX=${public_prefix}
					-DINTERNAL_PREFIX=${internal_prefix}
					-P "${CMAKE_SOURCE_DIR}/cmake/scripts/GenerateTemplate.cmake"
				DEPENDS "${in_path}"
				COMMENT "Generating C file ${out_path}"
				VERBATIM
				)

			list(APPEND GENERATED_SRCS        "${out_path}")
			list(APPEND GENERATED_LAYOUT_SRCS "${out_path}")
		endforeach()

		# Register per-layout generation target
		add_custom_target(${target}_generated_${layout} DEPENDS ${GENERATED_LAYOUT_SRCS})
		add_dependencies(${target} ${target}_generated_${layout})
	endforeach()

	# Export output file list to caller or register with target
	if(INKNIT_OUTPUT_FILES)
		set(${INKNIT_OUTPUT_FILES} "${GENERATED_SRCS}" PARENT_SCOPE)
	else()
		target_sources(${target} PRIVATE ${GENERATED_SRCS})
	endif()
endfunction()

function(inknit_load_dotenv path)
	file(READ ${path} _env_content)
	string(REPLACE "\n" ";" _env_lines "${_env_content}")
	foreach(_line IN LISTS _env_lines)
		string(REGEX MATCH "^[ ]*#.*" _is_comment "${_line}")
		if(_is_comment)
			continue()
		endif()

		if(_line MATCHES "^([A-Za-z_][A-Za-z0-9_]*)=(.*)$")
			inknit_expand_tilda("${CMAKE_MATCH_2}" VAR_VALUE)
			set(${CMAKE_MATCH_1} "${VAR_VALUE}" PARENT_SCOPE)
		endif()
	endforeach()
endfunction()
