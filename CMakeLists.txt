cmake_minimum_required(VERSION 3.19)
project(graphics)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw-3.3.4)
include_directories(glfw-3.3.4/include
                    glad/include
                    glm
                    ${CMAKE_SOURCE_DIR})

aux_source_directory(. SOURCE_FILE)
message(STATUS ${SOURCE_FILE})
add_executable(graphics ${SOURCE_FILE} glad/src/glad.c)

add_executable(utest test/unit_test.cpp obj_loader.cpp shader.cpp cgutils.cpp glad/src/glad.c)

target_link_libraries(graphics glfw ${CMAKE_DL_LIBS})