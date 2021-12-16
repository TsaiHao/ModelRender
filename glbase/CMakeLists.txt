set(LIB_NAME glbase)

if (GEN_EXE)
    add_subdirectory(glfw)
endif()

option(MOBILE_PLATFORM off)
if (Android)
    option(MOBILE_PLATFORM on)
endif()

set(OBJ_SOURCE_FILES
        obj/obj_render.cpp
        obj/obj_loader.cpp
        obj/obj_animator.cpp)
set(UTIL_SOURCE_FILES
        utils/cgutils.cpp
        utils/shader.cpp)

list(APPEND GL_INCLUDE_DIR
        glm
        obj
        utils
        ${CMAKE_SOURCE_DIR})

if (MOBILE_PLATFORM)
    set(GLAD_SOURCE_FILES )
else()
    set(GLAD_SOURCE_FILES
            glad/src/glad.c)
    list(APPEND GL_INCLUDE_DIR glad/include)
endif()

add_library(${LIB_NAME} SHARED ${UTIL_SOURCE_FILES} ${OBJ_SOURCE_FILES} ${GLAD_SOURCE_FILES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Android")
    target_link_libraries(${LIB_NAME} GLESv3)
endif()

if (GEN_EXE)
    add_compile_definitions(USE_GLFW)
    target_link_libraries(${LIB_NAME} glfw)
    list(APPEND GL_INCLUDE_DIR glfw/include)
endif()

include_directories(${GL_INCLUDE_DIR})
