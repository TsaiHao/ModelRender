#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

class ObjRender;
class Camera;
class GLContext;

enum class GLTaskCommand {
    Init,
    Draw,
    Pause,
    Quit,
    None,
};

class Scene {
public:
    explicit Scene(std::unique_ptr<Camera>& cam);
    ~Scene();

    void init();

    void draw();

    void initGLContext();

    void initRenders();

    void drawFrame();

    [[nodiscard]] const std::unique_ptr<GLContext>& getGLContext() const;

    void addModels(std::shared_ptr<ObjRender> &models);

private:
    void updateViewMatrix();
    void updateModelMatrix();
    void doDrawFrame();
    std::vector<std::shared_ptr<ObjRender>> models;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<GLContext> context;
};


#endif //GRAPHICS_SCENE_H
