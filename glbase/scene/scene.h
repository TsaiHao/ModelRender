#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

class ObjRender;
class Camera;
class GLContext;

class Scene {
public:
    explicit Scene(std::unique_ptr<GLContext>& context, std::unique_ptr<Camera>& cam);
    ~Scene();

    void drawFrame();

    void initGLContext();

    void initRenders();

    const std::unique_ptr<GLContext>& getGLContext() const;

private:
    void updateViewMatrix();
    void updateModelMatrix();
    void doDrawFrame();
    std::vector<std::shared_ptr<ObjRender>> models;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<GLContext> context;
};


#endif //GRAPHICS_SCENE_H
