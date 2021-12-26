#include "glbase/glbase.h"
#include <iostream>

int main(int argc, char **argv)
{
    WindowType window = glWindowInit();

    ObjRender lightSource("resource/model/cube.obj", "resource/shader/plain.vert", "resource/shader/plain.frag");
    lightSource.shader->attachTexture("texture1", Texture("resource/texture/wall.jpg"));

    auto trans = AnimatorActor::getActor(AnimationType::Translate, "trans");
    trans->setOrigin({0, 0.2f, 0});
    trans->setSpeed(0);
    auto scale = AnimatorActor::getActor(AnimationType::Scale, "scale");
    scale->setOrigin({ 0.1f, 0.1f, 0.1f });
    scale->setSpeed(0);
    auto rotate = AnimatorActor::getActor(AnimationType::Rotate, "rotate");
    rotate->setOrigin({0.5f, 0.7f, 0.9f});
    rotate->setSpeed(0.9f);
    auto trans2 = AnimatorActor::getActor(AnimationType::Translate, "trans2");
    trans2->setOrigin({0.5f, 0, 0});
    lightSource.animator.addDynamicActor(trans);
    lightSource.animator.addDynamicActor(rotate);
    lightSource.animator.addDynamicActor(trans2);
    lightSource.animator.addDynamicActor(scale);

    ObjRender cylinder("resource/model/cube.obj", "resource/shader/light.vert", "resource/shader/light.frag");
    //cylinder.shader->attachTexture("texture1", Texture("resource/texture/wall.jpg"));
    auto cyScale = AnimatorActor::getActor(AnimationType::Scale, "cylinder-scale");
    cyScale->setOrigin({0.3f, 0.3f, 0.3f});
    cylinder.addAnimationActor(cyScale);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

        lightSource.draw();
        cylinder.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

