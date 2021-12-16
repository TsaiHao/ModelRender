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
    ~ObjRender();

    void bufferData();
    void draw();

    void attachShader(const std::string& vsFile, const std::string& fsFile);
    std::shared_ptr<Shader> getShader();

public:
    std::shared_ptr<Shader> shader;
    ObjAnimator animator;

private:
    void emplaceAnimator();
    std::unique_ptr<GLObject> glObject;
    std::shared_ptr<ObjLoader> obj;
    std::vector<float> vertices;        // store vertices' position/texture/normal data
};


#endif //GRAPHICS_OBJ_RENDER_H
