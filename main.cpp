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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char **argv)
{
    GLFWwindow* window = glWindowInit();
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT);

    ObjRender render("resource/cylinder.obj", "resource/light.vs", "resource/light.fs");
    ObjRender lightSource("resource/cube.obj", "resource/plain.vs", "resource/plain.fs");

    render.getShader().use();
    glm::mat4 mvp(1.0f);
    mvp = glm::scale(mvp, glm::vec3(0.5f, 0.5f, 0.5f));
    render.getShader().setMat4("mvp", mvp);
    glm::vec3 lightPos(0, 0, 0);
    render.getShader().setFloatVec3("lightPos", lightPos);

    lightSource.getShader().use();
    glm::mat4 lightMvp(1.0f);
    lightMvp = glm::scale(lightMvp, glm::vec3(0.5f, 0.5f, 0.5f));
    lightMvp = glm::rotate(lightMvp, glm::radians(60.0f), glm::vec3(0.2f, 0.5f, 0.8f));
    lightSource.getShader().setMat4("mvp", lightMvp);

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nChannel;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resource/wall.jpg", &width, &height, &nChannel, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    lightSource.getShader().setInt("texture1", 0);

    float origZ = -5.0f;
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        float angle = glfwGetTime();
        auto trans = glm::rotate(mvp, glm::radians(angle) * 10.0f, glm::vec3(0.4f, 0.5f, 0.3f));
        
        render.getShader().use();
        render.getShader().setMat4("mvp", trans);
        //render.draw();
        

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        lightSource.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

