#include "obj_render.h"
#include "cgutils.h"

using namespace std;

ObjRender::ObjRender(std::shared_ptr<ObjLoader> loader) : obj(loader) {

}

void ObjRender::bufferData() {
    vertices.clear();
    vertices.reserve(100);

    for (auto const& face : obj->faces) {
        ObjVertex const& vert0 = obj->vertices[face.verts[0] - 1];
        ObjTextureCoordinate const& tex0 = obj->texCoords[face.texs[0] - 1];
        ObjVertexNorm const& norm0 = obj->normVecs[face.norms[0] - 1];

        for (int i = 1; i < face.verts.size() - 1; ++i) {
            pushVector(vertices, vert0.point);
            pushVector(vertices, tex0.coord);
            pushVector(vertices, norm0.norm);

            ObjVertex const& vert1 = obj->vertices[face.verts[i] - 1];
            ObjTextureCoordinate const& tex1 = obj->texCoords[face.texs[i] - 1];
            ObjVertexNorm const& norm1 = obj->normVecs[face.norms[i] - 1];

            ObjVertex const& vert2 = obj->vertices[face.verts[i + 1] - 1];
            ObjTextureCoordinate const& tex2 = obj->texCoords[face.texs[i + 1] - 1];
            ObjVertexNorm const& norm2 = obj->normVecs[face.norms[i + 1] - 1];

            pushVector(vertices, vert1.point);
            pushVector(vertices, tex1.coord);
            pushVector(vertices, norm1.norm);

            pushVector(vertices, vert2.point);
            pushVector(vertices, tex2.coord);
            pushVector(vertices, norm2.norm);
        }
    }

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
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / (4 + 3 + 3));
    _glCheckError();
}
