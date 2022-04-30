set(LIB_NAME glbase)

option(MOBILE_PLATFORM off)
if (Android)
    option(MOBILE_PLATFORM on)
endif()

set(RENDERER_SOURCE_FILES
        models/base_render.cpp
        models/base_render_impl.cpp
        models/geometry_render.cpp
        models/obj_render.cpp
        models/render_helper.cpp)

set(UTIL_SOURCE_FILES
        utils/cgutils.cpp
        utils/shader.cpp
        utils/animator.cpp
        utils/texture.cpp)

set(SCENE_SOURCE_FILES
        scene/camera.cpp
        scene/scene.cpp)

set(MESH_LOADER_FILES
        utils/mesh_loader/obj_loader.cpp)

list(APPEND GL_INCLUDE_DIR
        thirdparty/glm
        thirdparty
        models
        utils
        scene
        utils/mesh_loader
        ${CMAKE_SOURCE_DIR})

if (GEN_EXE)
    add_subdirectory(thirdparty/glfw)
    list(APPEND SCENE_SOURCE_FILES scene/ui/glfw_user_interface.cpp)
endif()

if (MOBILE_PLATFORM)
    set(GLAD_SOURCE_FILES )
else()
    set(GLAD_SOURCE_FILES
            thirdparty/glad/src/glad.c)
    list(APPEND GL_INCLUDE_DIR thirdparty/glad/include)
endif()

add_library(${LIB_NAME} SHARED
        ${UTIL_SOURCE_FILES}
        ${RENDERER_SOURCE_FILES}
        ${GLAD_SOURCE_FILES}
        ${SCENE_SOURCE_FILES}
        ${MESH_LOADER_FILES})

if (${CMAKE_SYSTEM_NAME} MATCHES "Android")
    find_library(log-lib log)
    target_link_libraries(${LIB_NAME} GLESv3 android ${log-lib})
endif()

if (GEN_EXE)
    add_compile_definitions(USE_GLFW)
    add_compile_definitions(GLFW_INCLUDE_NONE)
    target_link_libraries(${LIB_NAME} glfw)
    list(APPEND GL_INCLUDE_DIR thirdparty/glfw/include)
endif()

include_directories(${GL_INCLUDE_DIR})
