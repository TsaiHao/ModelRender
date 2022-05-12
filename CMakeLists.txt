cmake_minimum_required(VERSION 3.12)
project(graphics)

set(CMAKE_CXX_STANDARD 17)
set(GL_LIBRARY glbase)

option(GEN_EXE "generate executable" on)

add_subdirectory(glbase)
include_directories(glbase)

add_compile_definitions(USE_GLFW)
add_executable(graphics main.cpp)
add_executable(utest test/unit_test.cpp)

target_link_libraries(graphics ${CMAKE_DL_LIBS} glbase)
target_link_libraries(utest ${CMAKE_DL_LIBS} glbase)

add_custom_command(TARGET graphics POST_BUILD
		COMMENT "copy resource file"
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}/resource)
