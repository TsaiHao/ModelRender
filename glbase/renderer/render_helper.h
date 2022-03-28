#ifndef GRAPHICS_RENDER_HELPER_H
#define GRAPHICS_RENDER_HELPER_H

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "base_render.h"

class ObjRender;

class RenderHelper {
public:
    static glm::vec4 getModelSpacePosition(const BaseRender& render);
};

#endif //GRAPHICS_RENDER_HELPER_H
