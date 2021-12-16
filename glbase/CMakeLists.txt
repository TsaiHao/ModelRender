set(LIB_NAME glbase)

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
set(GLAD_SOURCE_FILES
        glad/src/glad.c)

list(APPEND GL_INCLUDE_DIR
        glad/include
        glm
        obj
        utils
        ${CMAKE_SOURCE_DIR})

add_library(${LIB_NAME} SHARED ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES} ${GLAD_SOURCE_FILES})

if (GEN_EXE)
    add_compile_definitions(USE_GLFW)
    target_link_libraries(${LIB_NAME} glfw)
    list(APPEND GL_INCLUDE_DIR glfw/include)
endif()

include_directories(${GL_INCLUDE_DIR})
set(GL_BASE_INCLUDE ${GL_INCLUDE_DIR} PARENT_SCOPE)