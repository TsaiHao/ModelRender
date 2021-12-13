cmake_minimum_required(VERSION 3.12)
project(graphics)

set(CMAKE_CXX_STANDARD 20)
set(GL_LIBRARY glbase)

option(GEN_EXE "generate executable" off)

if (GEN_EXE)
	add_subdirectory(glfw)
endif()

set(OBJ_SOURCE_FILES
        obj/obj_render.cpp
        obj/obj_loader.cpp
		obj/obj_animator.cpp)
set(UTIL_SOURCE_FILES
        utils/cgutils.cpp
        utils/shader.cpp)
set(GLAD_SOURCE_FILE glad/src/glad.c)

list(APPEND GL_INCLUDE_DIR glad/include glm obj utils ${CMAKE_SOURCE_DIR})

add_library(${GL_LIBRARY} SHARED ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES} ${GLAD_SOURCE_FILE})

#target_include_directories(${GL_LIBRARY} PRIVATE ${GL_INCLUDE_DIR})

if (GEN_EXE)
	list(APPEND GL_INCLUDE_DIR glfw/include)
	add_executable(graphics main.cpp )
	add_executable(utest test/unit_test.cpp)

	target_link_libraries(graphics glfw ${CMAKE_DL_LIBS} glbase)
	target_link_libraries(utest glfw ${CMAKE_DL_LIBS} glbase)

	add_custom_command(TARGET graphics POST_BUILD
        	COMMENT "copy resource file"
			COMMAND ${CMAKE_COMMAND} -E copy_directory
        	${CMAKE_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}/resource)
endif()

include_directories(${GL_INCLUDE_DIR})