using namespace std;
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

        lightSource.draw();
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

