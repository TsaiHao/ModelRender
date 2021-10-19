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

    Shader shader("resource/light.vs", "resource/light.fs");
    ObjRender render("resource/cylinder.obj");

    glm::mat4 mvp(1.0f);
    mvp = glm::scale(mvp, glm::vec3(0.5f, 0.5f, 0.5f));
    shader.use();
    shader.setMat4("mvp", mvp);
    glm::vec3 lightPos(0, 0, 0);
    shader.setFloatVec3("lightPos", lightPos);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        float angle = glfwGetTime();
        auto trans = glm::rotate(mvp, glm::radians(angle) * 10.0f, glm::vec3(0.4f, 0.5f, 0.3f));

        shader.use();
        shader.setMat4("mvp", trans);
        render.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

