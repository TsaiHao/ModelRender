#ifndef GRAPHICS_OBJ_RENDER_H
#define GRAPHICS_OBJ_RENDER_H

#include <vector>
#include <memory>

#include "obj_loader.h"
#include "obj_animator.h"

class Shader;
class GLObject;

class ObjRender {
public:
    explicit ObjRender(std::shared_ptr<ObjLoader> loader);
    explicit ObjRender(std::string const& objFile, const std::string &vsFile = "", const std::string& fsFile = "");
    ObjRender(const ObjRender& rhs);
    ObjRender& operator=(const ObjRender &rhs);
    ~ObjRender();

    // must call in gl thread
    void bufferData();

    void draw();

    void attachShader(const std::string& vsFile, const std::string& fsFile);
    void setViewMatrix(const float* mat) const;
    void setModelMatrix(const float* mat) const;
    std::shared_ptr<Shader> getShader() const;

    void addAnimationActor(const std::shared_ptr<AnimatorActor> &actor);
    Vec3 getCurrentPosition();
    Vec3 getCurrentRotation();
    Vec3 getCurrentScale();

public:
    std::shared_ptr<Shader> shader;
    ObjAnimator animator;

private:
    void emplaceAnimator();
    std::string vertFile, fragFile;
    std::unique_ptr<GLObject> glObject;
    std::shared_ptr<ObjLoader> obj;
    std::vector<float> vertices;        // store vertices' position/texture/normal data
};


#endif //GRAPHICS_OBJ_RENDER_H
