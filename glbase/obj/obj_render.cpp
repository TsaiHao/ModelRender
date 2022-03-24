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
    shader = rhs.shader;
    animator = rhs.animator;
    return *this;
}

void ObjRender::addAnimationActor(const shared_ptr<AnimatorActor> &actor) {
    animator.addDynamicActor(actor);
}

void ObjRender::updateViewMatrix(const float* mat) const {
    if (shader) {
        shader->setViewMatrix(mat);
    }
}

void ObjRender::updateProjectionMatrix(const float *mat) const {
    if (shader) {
        shader->setProjectionMatrix(mat);
    }
}

Vec4 ObjRender::getCurrentPosition() {
    Vec4 origin = Vec4(0, 0, 0, 1.0f);
    auto trans = animator.processSinglePoint(origin);
    return trans;
}

void ObjRender::debugLighting(float x, float y, float z, float w) {
    glm::vec4 lightPos = glm::vec4(x, y, z, w);
    glm::mat4 model = animator.getModelMatrix();

    int nVertices = vertices.size() /  10;
    for (int i = 0; i < nVertices; ++i) {
        glm::vec4 aPos = glm::vec4(vertices[i * 10], vertices[i * 10 + 1], vertices[i * 10 + 2], vertices[i * 10 + 3]);
        glm::vec4 fragPos = model * aPos;
        glm::vec3 normal = glm::normalize(glm::vec3(vertices[i * 10 + 7], vertices[i * 10 + 8], vertices[i * 10 + 9]));
        glm::vec3 lightDir = glm::normalize(lightPos - fragPos);
        float dot = glm::dot(normal, lightDir);
        cout << dot << '\t';
    }
    cout << endl;
}

ObjRender::~ObjRender() = default;
