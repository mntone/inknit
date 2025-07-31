add_library(${PROJECT_NAME}_cxx INTERFACE)

target_include_directories(${PROJECT_NAME}_cxx INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/inc
	)

target_link_libraries(${PROJECT_NAME}_cxx INTERFACE
	inknit
	)
