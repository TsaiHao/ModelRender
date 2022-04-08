#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

class BaseRender;
class Camera;
class GLFWUserInterface;

class Scene {
public:
    Scene();
    ~Scene();

    void init();

    void addModel(std::shared_ptr<BaseRender> model);

    void renderAFrame();

    void draw();

    void setCameraMatrix(const std::array<float, 3>& position,
                         const std::array<float, 3>& target,
                         const std::array<float, 3>& up) const;

    void addLightSource(const std::shared_ptr<BaseRender>& lightModel);

    class GLNativeWindow;
private:
    std::vector<std::shared_ptr<BaseRender>> models;
    std::shared_ptr<BaseRender> lightSource;
    std::shared_ptr<Camera> cam;
    std::unique_ptr<GLNativeWindow> nativeWindow;
    std::unique_ptr<GLFWUserInterface> ui;
};

#endif //GRAPHICS_SCENE_H
