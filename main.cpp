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

    ObjRender render("resource/.obj", "resource/light.vs", "resource/light.fs");
    ObjRender lightSource("resource/cube.obj", "resource/plain.vs", "resource/plain.fs");

    render.getShader().use();
    glm::mat4 mvp(1.0f);
    mvp = glm::scale(mvp, glm::vec3(0.5f, 0.5f, 0.5f));
    render.getShader().setMat4("mvp", mvp);
    glm::vec3 lightPos(0, 0, 0);
    render.getShader().setFloatVec3("lightPos", lightPos);

    lightSource.getShader().use();

    Texture tex("resource/wall.jpg");
    lightSource.getShader().attachTexture("texture1", tex);
    
    float speed = 0.5f;
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        float angle = glfwGetTime();
        auto trans = glm::rotate(mvp, glm::radians(angle) * 10.0f, glm::vec3(0.4f, 0.5f, 0.3f));
        
        render.getShader().use();
        render.getShader().setMat4("mvp", trans);
        //render.draw();

        glm::mat4 lightMvp(1.0f);
        lightMvp = glm::translate(lightMvp, glm::vec3(0, 0.5f, 0));
        lightMvp = glm::rotate(lightMvp, float(glfwGetTime() * speed), glm::vec3(0.2f, 0.5f, 0.8f));
        lightMvp = glm::scale(lightMvp, glm::vec3(0.5f, 0.5f, 0.5f));
        lightSource.shader.setMat4("mvp", lightMvp);

        lightSource.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

