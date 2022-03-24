#ifndef GRAPHICS_SCENE_H
#define GRAPHICS_SCENE_H

#include <vector>
#include <memory>

class ObjRender;
class Camera;

class Scene {
public:
    Scene();
    ~Scene();

    void init();
    void addModel(std::shared_ptr<ObjRender> model);
    void addModel(const ObjRender& model);
    void renderAFrame();

    void draw();

    void setCameraMatrix(const std::array<float, 3>& position,
                         const std::array<float, 3>& target,
                         const std::array<float, 3>& up) const;

    void addLightSource(const std::shared_ptr<ObjRender>& lightModel);

    class GLNativeWindow;
private:
    std::vector<std::shared_ptr<ObjRender>> models;
    std::shared_ptr<ObjRender> lightSource;
    std::unique_ptr<Camera> cam;
    std::unique_ptr<GLNativeWindow> nativeWindow;
};


#endif //GRAPHICS_SCENE_H
