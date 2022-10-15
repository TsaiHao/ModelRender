#include <math.h>
#include "geometry_render.h"
#include "glm/ext/matrix_transform.hpp"
#include "shader.h"
#include "animator.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace std;

FT_Library ft;
GLuint textureId;

int initFreeType() {
    int error;

    error = FT_Init_FreeType(&ft);
    if (error) {
        Logger::error("init freetype failed %d", error);
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


    initFreeType();
	
    FT_Face face;
    int error = FT_New_Face(ft, "resource/font/DejaVuSansMono.ttf", 0, &face);
    if (error) {
        Logger::error("load face failed %d", error);
        exit(1);
    }

    FT_Set_Pixel_Sizes(face, 0, 40);
    error = FT_Load_Char(face, 'X', FT_LOAD_RENDER);
    if (error) {
        Logger::error("load char failed %d", error);
        exit(1);
    }


    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // 设置纹理选项
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void PolygonRender::initRender() {

    initGL();
}

void PolygonRender::draw() {
    shader->use();
    glBindVertexArray(VAO);
    if (animator) {
        animator->doProcess();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    shader->setInt(TEXTURE_UNIFORM, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size() / 5);
    _glCheckError();
}
