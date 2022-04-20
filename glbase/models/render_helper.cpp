#include "render_helper.h"
#include "obj_render.h"
#include "animator.h"

glm::vec4 RenderHelper::getModelSpacePosition(const BaseRenderImpl& render) {
    glm::vec4 origin = glm::vec4(0, 0, 0, 1.0f);
    if (render.animator == nullptr) {
        return origin;
    }
    auto trans = render.animator->processSinglePoint(origin);
    return trans;
}