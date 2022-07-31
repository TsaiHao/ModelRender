add_compile_definitions(USE_GLFW)
add_executable(graphics main.cpp)

target_link_libraries(graphics ${CMAKE_DL_LIBS} glbase)
target_include_directories(graphics PUBLIC "$<TARGET_PROPERTY:glbase,INCLUDE_DIRECTORIES>")

add_custom_command(TARGET graphics POST_BUILD
		COMMENT "copy resource file"
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_LIST_DIR}/../../resource ${CMAKE_CURRENT_BINARY_DIR}/resource
)
