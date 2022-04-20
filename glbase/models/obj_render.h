#ifndef GRAPHICS_OBJ_RENDER_H
#define GRAPHICS_OBJ_RENDER_H

#include <vector>
#include <memory>
#include <string>

#include "base_render_impl.h"
#include "glad/glad.h"

class Shader;
class GLObject;
class ObjLoader;
class Animator;
class AnimatorActor;

class ObjRender: public BaseRenderImpl {
public:
    explicit ObjRender(std::string const& objFile);

    ObjRender(const ObjRender& rhs);
    ObjRender& operator=(const ObjRender &rhs);

    ~ObjRender();

    void initRender() override;

    void draw() override;

private:
    std::shared_ptr<ObjLoader> obj;
};

#endif //GRAPHICS_OBJ_RENDER_H
