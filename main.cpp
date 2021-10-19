#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#include "shader.h"
#include "obj_loader.h"
#include "obj_render.h"
#include "cgutils.h"

int main(int argc, char **argv)
{
    GLFWwindow* window = glWindowInit();

    Shader shader("resource/plain.vs", "resource/plain.fs");
    std::shared_ptr<ObjLoader> obj = make_shared<ObjLoader>("resource/cylinder.obj");
    ObjRender render(obj);
    render.bufferData();

    glm::mat4 mvp(1.0f);
    mvp = glm::scale(mvp, glm::vec3(0.5f, 0.5f, 0.5f));
    shader.use();
    shader.setMat4("mvp", mvp);

    glViewport(0, 0, 1500.0, 1500.0);
    const int wave = 500;

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        float angle = glfwGetTime();
        const int portSize = sin(angle * 0.2) * wave + GL_WINDOW_WIDTH;
        glViewport(0, 0, portSize, portSize);
        auto trans = glm::rotate(mvp, glm::radians(angle) * 10.0f, glm::vec3(0.4f, 0.5f, 0.3f));

        shader.use();
        shader.setMat4("mvp", trans );
        render.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

