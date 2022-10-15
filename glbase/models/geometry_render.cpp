#include <math.h>
#include "geometry_render.h"
#include "glm/ext/matrix_transform.hpp"
#include "shader.h"
#include "animator.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

int initFreeType() {
    FT_Library ft;
    int error;

    error = FT_Init_FreeType(&ft);
    if (error) {
        Logger::error("init freetype failed %d", error);
        exit(1);
    }

    FT_Face face;
    error = FT_New_Face(ft, "resource/font/DejaVuSansMono.ttf", 0, &face);
    if (error) {
        Logger::error("load face failed %d", error);
        exit(1);
    }

    return 0;
}

std::vector<float> PolygonFactory::createDefaultTriangle() {
    auto leftDownY = -tan(glm::radians(30.0f));
    float upY = 0.667f * cos(1.0f / 6 * M_PI);
    vector<float> vertices = {
            -1.0f,  leftDownY,  0,  0,      0,
            0,      upY,        0,  0.5f,   1.0f,
            1,      leftDownY,  0,  1.0f,   0,
    };

    return vertices;
}

std::vector<float> PolygonFactory::createDefaultRectangle() {
    return vector<float> {
            -1.0f,  -1.0f,  0,  0,      0,
            -1.0f,  1.0f,   0,  0,      1.0f,
            1.0f,   1.0f,   0,  1.0f,   1.0f,
            1.0f,   -1.0f,  0,  1.0f,   0
    };
}

std::vector<float> PolygonFactory::createDefaultCircle() {
    constexpr int nTriangles = 360;
    vector<float> vertices {0, 0,    0, 0.5f, 0.5f,
                            0, 1.0f, 0, 0.5f, 1.0f};
    vertices.reserve((nTriangles + 1) * 5);

    glm::vec3 left {0, 1.0f, 0};
    glm::mat3 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(360.0f / nTriangles), glm::vec3(0, 0, -1.0f));
    for (int i = 0; i < nTriangles; ++i) {
        glm::vec3 right = rotation * left;

        vertices.push_back(right.x);
        vertices.push_back(right.y);
        vertices.push_back(right.z);
        vertices.push_back(right.x / 2.0f + 0.5f);
        vertices.push_back(right.y / 2.0f + 0.5f);

        left = right;
    }

    return vertices;
}

GeometryRenderBase::GeometryRenderBase() {
    attachShaders("resource/shader/polygon.vert", "resource/shader/yuv420_planar_render.frag");
}

PolygonRender::PolygonRender(GeometryType pType): type(pType) {
    bufferFormat = VertexBufferFormat::PT32;

    if (type == GeometryType::Triangle) {
        vertices = PolygonFactory::createDefaultTriangle();
    } else if (type == GeometryType::Rectangle) {
        vertices = PolygonFactory::createDefaultRectangle();
    } else if (type == GeometryType::Circle) {
        vertices = PolygonFactory::createDefaultCircle();
    }
}

void PolygonRender::initRender() {
    initFreeType();
    initGL();
}

void PolygonRender::draw() {
    shader->use();
    glBindVertexArray(VAO);
    if (animator) {
        animator->doProcess();
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 5);
    _glCheckError();
}
