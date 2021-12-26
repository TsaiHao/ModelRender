#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

#include "camera.h"

class ObjRender;

class Scene {
public:
    Scene();
    ~Scene();

private:
    std::vector<std::shared_ptr<ObjRender>> models;
    Camera cam;
};


#endif //GRAPHICS_SCENE_H
