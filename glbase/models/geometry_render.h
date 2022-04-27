#ifndef GRAPHICS_GEOMETRY_RENDER_H
#define GRAPHICS_GEOMETRY_RENDER_H

#include <vector>
#include "base_render_impl.h"

class GeometryRenderBase: public BaseRenderImpl {
public:
    GeometryRenderBase();
};

struct PolygonFactory {
    static std::vector<float> createDefaultTriangle();

    static std::vector<float> createDefaultRectangle();

    static std::vector<float> createDefaultCircle();
};

class PolygonRender: public GeometryRenderBase {
public:
    explicit PolygonRender(GeometryType pType);

    void initRender() override;

    void draw() override;

private:
    GeometryType type;
};


#endif //GRAPHICS_GEOMETRY_RENDER_H
