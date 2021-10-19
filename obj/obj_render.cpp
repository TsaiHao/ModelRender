#include "obj_render.h"
#include "cgutils.h"

using namespace std;

ObjRender::ObjRender(std::shared_ptr<ObjLoader> loader) : obj(loader) {

}

void ObjRender::bufferData() {
    vertices.clear();
    vertices.reserve(100);

    for (auto const& face : obj->faces) {
        for (int i = 0; i < face.verts.size(); ++i) {
            ObjVertex const& vert = obj->vertices[face.verts[i]];
            pushVector(vertices, vert.point);

            if (obj->texCoords.empty()) {
                pushVector(vertices, array<int, 3>({ 0, 0, 0 }));
            }
            else {
                ObjTextureCoordinate const& tex = obj->texCoords[face.texs[i]];
                pushVector(vertices, tex.coord);
            }

            if (obj->normVecs.empty()) {
                pushVector(vertices, array<int, 3>({ 0, 0, 0 }));
            }
            else {
                ObjVertexNorm const& norm = obj->normVecs[face.norms[i]];
                pushVector(vertices, norm.norm);
            }
        }
    }

    printVertices("verts.txt", vertices, 10);
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    int stride = 4 + 3 + 3;
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)4);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(4 + 3));
    glEnableVertexAttribArray(2);
    _glCheckError();

    glBindVertexArray(0);
}

void ObjRender::draw() {
    glBindVertexArray(VAO);
    int offset = 0;

    for (auto const& face : obj->faces) {
        int vertexCount = face.verts.size();
        glDrawArrays(GL_TRIANGLE_FAN, offset, vertexCount);
        offset += vertexCount;
    }
    _glCheckError();
}
