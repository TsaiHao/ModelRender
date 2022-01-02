#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

class ObjRender;
class Camera;

class Scene {
public:
    explicit Scene( std::unique_ptr<Camera> cam = nullptr);
    ~Scene();

    void drawFrame();

private:
    void updateViewMatrix();
    void updateModelMatrix();
    void doDrawFrame();
    std::vector<std::shared_ptr<ObjRender>> models;
    std::unique_ptr<Camera> camera;
};


#endif //GRAPHICS_SCENE_H
