cmake_minimum_required(VERSION 3.12)
project(graphics)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)
include_directories(glfw/include
                    glad/include
                    glm
                    obj
                    utils
                    ${CMAKE_SOURCE_DIR})

aux_source_directory(obj OBJ_SOURCE_FILES)
set(GLAD_SOURCE_FILE glad/src/glad.c)
aux_source_directory(utils UTIL_SOURCE_FILES)

add_executable(graphics main.cpp ${GLAD_SOURCE_FILE} ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES})

add_executable(utest test/unit_test.cpp ${GLAD_SOURCE_FILE} ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES})

target_link_libraries(graphics glfw ${CMAKE_DL_LIBS})
target_link_libraries(utest ${CMAKE_DL_LIBS})

file(COPY resource DESTINATION .)