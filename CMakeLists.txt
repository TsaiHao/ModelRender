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

set(OBJ_SOURCE_FILES
        obj/obj_render.cpp
        obj/obj_loader.cpp
		obj/obj_animator.cpp)
set(UTIL_SOURCE_FILES
        utils/cgutils.cpp
        utils/shader.cpp)
set(GLAD_SOURCE_FILE glad/src/glad.c)

add_executable(graphics main.cpp ${GLAD_SOURCE_FILE} ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES})

add_executable(utest test/unit_test.cpp ${GLAD_SOURCE_FILE} ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES})

target_link_libraries(graphics glfw ${CMAKE_DL_LIBS})
target_link_libraries(utest glfw ${CMAKE_DL_LIBS})

message(STATUS ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET graphics POST_BUILD
        COMMENT "copy resource file"
	COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}/resource)