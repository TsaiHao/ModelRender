#include <utility>

#include "obj_render.h"

#include "cgutils.h"
#include "shader.h"

using namespace std;

class GLObject {
public:
    GLuint VAO, VBO;
};

ObjRender::ObjRender(std::shared_ptr<ObjLoader> loader) : obj(std::move(loader)) {
    bufferData();
}

ObjRender::ObjRender(std::string const &objFile, const std::string &vsFile, const std::string &fsFile) {
    attachShader(vsFile, fsFile);
    obj = make_shared<ObjLoader>(objFile);
    bufferData();
}

void ObjRender::bufferData() {
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

    //printVertices("verts.txt", vertices, 10);

    glObject = make_unique<GLObject>();
    glGenVertexArrays(1, &(glObject->VAO));
    glGenBuffers(1, &(glObject->VBO));

    glBindVertexArray(glObject->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, glObject->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    int stride = 4 + 3 + 3;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) 4);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *) (4 + 3));
    glEnableVertexAttribArray(2);
    _glCheckError();

    glBindVertexArray(0);
}

void ObjRender::draw() {
    shader->use();
    glBindVertexArray(glObject->VAO);
    int offset = 0;

    animator.doProcess();
    for (auto const &face: obj->faces) {
        int vertexCount = face.verts.size();
        glDrawArrays(GL_TRIANGLE_FAN, offset, vertexCount);
        offset += vertexCount;
    }
    _glCheckError();
}

void ObjRender::attachShader(const std::string &vsFile, const std::string &fsFile) {
    shader = make_shared<Shader>(vsFile, fsFile);
    emplaceAnimator();
}

shared_ptr<Shader> ObjRender::getShader() const {
    return shader;
}

void ObjRender::emplaceAnimator() {
    animator = ObjAnimator(shader);
}

ObjRender::ObjRender(const ObjRender& rhs) {
    *this = rhs;
}

ObjRender &ObjRender::operator=(const ObjRender &rhs) {
    if (&rhs == this) {
        return *this;
    }
    glObject = make_unique<GLObject>();
    *glObject = *(rhs.glObject);
    obj = rhs.obj;
    vertices = rhs.vertices;
    return *this;
}

void ObjRender::addAnimationActor(const shared_ptr<AnimatorActor> &actor) {
    animator.addDynamicActor(actor);
}

ObjRender::~ObjRender() = default;
