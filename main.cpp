#include <stdlib.h>
#include <iostream>
using namespace std;

#include "glbase/glbase.h"

int main(int argc, char **argv)
{
    WindowType window = glWindowInit();

    ObjRender lightSource("resource/cube.obj", "resource/plain.vs", "resource/plain.fs");
    lightSource.shader->attachTexture("texture", Texture("resource/wall.jpg"));

    auto trans = AnimatorActor::getActor(AnimationType::Translate, "trans");
    trans->setOrigin({0, 0.2f, 0});
    trans->setSpeed(0);
    auto scale = AnimatorActor::getActor(AnimationType::Scale, "scale");
    scale->setOrigin({ 0.25f, 0.25f, 0.25f });
    scale->setSpeed(0);
    auto rotate = AnimatorActor::getActor(AnimationType::Rotate, "rotate");
    rotate->setOrigin({0.5f, 0.7f, 0.9f});
    rotate->setSpeed(0.1f);
    lightSource.animator.addDynamicActor(trans);
    lightSource.animator.addDynamicActor(rotate);
    lightSource.animator.addDynamicActor(scale);

    while (true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        float angle = glfwGetTime();
        auto trans = glm::rotate(mvp, glm::radians(angle) * 10.0f, glm::vec3(0.4f, 0.5f, 0.3f));
        
        render.getShader().use();
        render.getShader().setMat4("mvp", trans);
        //render.draw();

        glm::mat4 lightMvp(1.0f);
        lightMvp = glm::rotate(lightMvp, float(glm::radians(glfwGetTime() * speed * 20)), glm::vec3(0, 0, 1));
        lightMvp = glm::translate(lightMvp, glm::vec3(0, 0.5f, 0));
        lightMvp = glm::rotate(lightMvp, float(glfwGetTime() * speed), glm::vec3(0.2f, 0.5f, 0.8f));
        lightMvp = glm::scale(lightMvp, glm::vec3(0.25f, 0.25f, 0.25f));
        lightSource.shader.setMat4("mvp", lightMvp);

        lightSource.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

