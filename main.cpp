#include "glbase/glbase.h"

int main(int argc, char **argv)
{
    WindowType window = glWindowInit();

    ObjRender lightSource("resource/model/cube.obj", "resource/shader/plain.vs", "resource/shader/plain.fs");
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

    while (true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.f, 0.f, 0.f, 1.0f);

        lightSource.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

