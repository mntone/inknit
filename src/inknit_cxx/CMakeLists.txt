enable_language(CXX)

add_library(${PROJECT_NAME}_cxx STATIC)

target_include_directories(${PROJECT_NAME}_cxx PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	)

target_link_libraries(${PROJECT_NAME}_cxx PUBLIC
	inknit
	)

target_sources(${PROJECT_NAME}_cxx PRIVATE
	$<$<BOOL:${UNIX}>:format/auto-posix.cpp>
	format/block.cpp
	format/character.cpp
	)
