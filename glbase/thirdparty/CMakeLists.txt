# process third party libraries

set(GLAD_SRC glad/src/glad.c)

add_subdirectory(glfw)

add_library(
    cg3rd STATIC
    ${GLAD_SRC}
)

target_include_directories(
    cg3rd PUBLIC
    glad/include
    glfw/include
    glm
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    cg3rd PRIVATE
    glfw
)