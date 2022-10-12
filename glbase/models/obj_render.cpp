#include <utility>

#include "obj_render.h"

#include "obj_loader.h"
#include "cgutils.h"
#include "shader.h"
#include "animator.h"
using namespace std;

ObjRender::ObjRender(std::string const &objFile) {
    attachShaders("resource/shader/mvp.vert", "resource/shader/plain.frag");
    obj = make_shared<ObjLoader>(objFile);
}

void ObjRender::initRender() {
    vertices.clear();
    vertices.reserve(100);

    for (auto const &face: obj->faces) {
        for (int i = 0; i < face.verts.size(); ++i) {
            ObjVertex const &vert = obj->vertices[face.verts[i]];
            pushVector(vertices, vert.point);

            if (obj->texCoords.empty()) {
                pushVector(vertices, array<int, 3>({0, 0, 0}));
            } else {
                ObjTextureCoordinate const &tex = obj->texCoords[face.texs[i]];
                pushVector(vertices, tex.coord);
            }

            if (obj->normVecs.empty()) {
                pushVector(vertices, array<int, 3>({0, 0, 0}));
            } else {
                ObjVertexNorm const &norm = obj->normVecs[face.norms[i]];
                pushVector(vertices, norm.norm);
            }
        }
    }

    initGL();
}

void ObjRender::draw() {
    shader->use();
    if (texture != nullptr) {
        texture->bind();
    }

    glBindVertexArray(VAO);
    int offset = 0;

    animator->doProcess();
    for (auto const &face: obj->faces) {
        int vertexCount = face.verts.size();
        glDrawArrays(GL_TRIANGLE_FAN, offset, vertexCount);
        offset += vertexCount;
    }
    _glCheckError();
}

ObjRender::ObjRender(const ObjRender& rhs) {
    *this = rhs;
}

ObjRender &ObjRender::operator=(const ObjRender &rhs) {
    if (&rhs == this) {
        return *this;
    }
    VAO = rhs.VAO;
    VBO = rhs.VBO;
    obj = rhs.obj;
    vertices = rhs.vertices;
    shader = rhs.shader;
    animator = rhs.animator;
    return *this;
}

ObjRender::~ObjRender() = default;
