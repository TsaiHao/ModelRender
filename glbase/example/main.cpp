#include "glbase.h"
#include <iostream>

int main(int argc, char **argv)
{
    Scene scene;
    scene.setCameraMatrix({0, 0, -3.0f}, {0, 0, 0}, {0, 1, 0});

    auto lightSource = createModel("resource/model/cube.obj");

    lightSource->rotate(0.4f, 0.8f, 0.3f, 0, 0.8f);
    lightSource->translate(1.0f, 0, 0);
    lightSource->rotate(0.5f, 0.5f, 0, 0, 0.4f);
    lightSource->scale(0.1f, 0.1f, 0.1f);

    auto cylinder= createModel("resource/model/polygon.obj");
    cylinder->attachShaders("resource/shader/mvp.vert", "resource/shader/light.frag");
    cylinder->attachTexture("resource/texture/wall.jpg");
    cylinder->scale(0.4f, 0.4f, 0.4f);

    scene.addModel(cylinder);
    scene.addLightSource(lightSource);

    /*
    auto triangle = createModel(GeometryType::Rectangle);
    triangle->attachTexture("resource/texture/wall.jpg");
    scene.addModel(triangle);
    */

    scene.draw();

    return 0;
}

