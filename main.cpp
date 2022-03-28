#include "glbase/glbase.h"

int main(int argc, char **argv)
{
    Scene scene;

    auto lightSource = createModel("resource/model/cube.obj");

    lightSource->rotate(0.4f, 0.8f, 0.3f, 0, 0.8f);
    lightSource->translate(1.0f, 0, 0);
    lightSource->rotate(0.5f, 0.5f, 0, 0, 0.4f);
    lightSource->scale(0.1f, 0.1f, 0.1f);

    auto cylinder= createModel("resource/model/polygon.obj");
    cylinder->attachShaders("resource/shader/mvp.vert", "resource/shader/light.frag");
    cylinder->scale(0.4f, 0.4f, 0.4f);

    scene.addModel(cylinder);
    scene.addLightSource(lightSource);

    scene.draw();

    return 0;
}

