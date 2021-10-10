#pragma once
#include <string>
#include <vector>
#include <array>
#include <memory>

#include "glad/glad.h"

struct ObjBase {
    ObjBase() = default;
    ObjBase(const std::string& line) {}
    virtual ~ObjBase() {}

    virtual void parseObjLine(const std::string& line) = 0;
};

struct ObjVertex: public ObjBase {
    ObjVertex() = default;
    ObjVertex(float x, float y, float z, float w = 1.0f);
    void parseObjLine(const std::string& line) override;

    std::array<float, 4> point;
};

struct ObjTextureCoordinate: public ObjBase {
    ObjTextureCoordinate() = default;
    explicit ObjTextureCoordinate(float x, float y = 0, float z = 0);
    void parseObjLine(const std::string& line) override; 

    std::array<float, 3> coord;
};

struct ObjVertexNorm: public ObjBase {
    ObjVertexNorm() = default;
    ObjVertexNorm(float x, float y, float z, bool normalize = true);
    void parseObjLine(const std::string& line) override; 
    void normalize();
    std::array<float, 3> norm;
};

struct ObjFace: public ObjBase {
    ObjFace() = default;
    void parseObjLine(const std::string& line) override; 
    void parseObjWord(const std::string& word);
    void bufferToVertices(std::vector<float> &data);

    std::vector<int> verts;
    std::vector<int> texs;
    std::vector<int> norms;
};

class ObjLoader {
    using basep = std::shared_ptr<ObjBase>;
public:
    explicit ObjLoader(const std::string& file);
    void render();
    void draw() const;

    std::string mtllib;
    std::string objName;
    std::string material;
    std::vector<ObjVertex> vertices;
    std::vector<ObjTextureCoordinate> texCoords;
    std::vector<ObjVertexNorm> normVecs;
    std::vector<ObjFace> faces;

    // render
    GLuint VAO, VBO, EBO;
    int triangleCount;
};