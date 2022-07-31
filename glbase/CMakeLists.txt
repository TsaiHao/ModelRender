cmake_minimum_required(VERSION 3.12)
project(graphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(LIB_NAME glbase)

option(ENABLE_BUILD_TEST "enable build tests" OFF)
option(ENABLE_BUILD_EXAMPLE "build example executable" ON)

set(RENDERER_SOURCE_FILES
        models/base_render.cpp
        models/base_render_impl.cpp
        models/geometry_render.cpp
        models/obj_render.cpp
        models/render_helper.cpp
)

set(UTIL_SOURCE_FILES
        utils/cgutils.cpp
        utils/shader.cpp
        utils/animator.cpp
        utils/texture.cpp
)

set(SCENE_SOURCE_FILES
        scene/camera.cpp
        scene/scene.cpp
)

set(MESH_LOADER_FILES
        utils/mesh_loader/obj_loader.cpp
)

list(APPEND GL_INCLUDE_DIR
        models
        utils
        scene
        utils/mesh_loader
        ${CMAKE_SOURCE_DIR}
)

add_subdirectory(thirdparty)

add_library(
        ${LIB_NAME} STATIC 
        ${UTIL_SOURCE_FILES}
        ${RENDERER_SOURCE_FILES}
        ${SCENE_SOURCE_FILES}
        ${MESH_LOADER_FILES}
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Android")
    find_library(log-lib log)
    target_link_libraries(${LIB_NAME} PRIVATE GLESv3 android ${log-lib})
else()
    add_compile_definitions(USE_GLFW)
    add_compile_definitions(GLFW_INCLUDE_NONE)
    target_sources(${LIB_NAME} PRIVATE scene/ui/glfw_user_interface.cpp)
endif()

target_include_directories(
    ${LIB_NAME} PUBLIC 
    ${GL_INCLUDE_DIR}
    "$<TARGET_PROPERTY:cg3rd,INCLUDE_DIRECTORIES>"
)

target_link_libraries(
    ${LIB_NAME} PUBLIC
    cg3rd
)

if (${ENABLE_BUILD_TEST})
    add_subdirectory(test)
endif()

if (${ENABLE_BUILD_EXAMPLE})
    add_subdirectory(example)
endif()
