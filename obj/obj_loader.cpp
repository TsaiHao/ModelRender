#include <fstream>
#include <cassert>
#include <memory>
#include <cmath>
#include <iterator>

#include "glad/glad.h"
#include "obj_loader.h"
#include "cgutils.h"
using namespace std;

ObjVertex::ObjVertex(float x, float y, float z, float w): point({x, y, z, w}) { }

void ObjVertex::parseObjLine(const std::string& line) {
    auto words = splitString(line, " ");
    if (words.size() > 5 || words.size() < 4 || words.front() != "v") {
        Logger::error("parse obj vertex line: "s + line);
        return;
    }

    float x = stof(words[1]);
    float y = stof(words[2]);
    float z = stof(words[3]);
    float w = words.size() == 5 ? stof(words.back()) : 1.0f;
    point = {x, y, z, w};
}

ObjTextureCoordinate::ObjTextureCoordinate(float x, float y, float z): coord({x, y, z}) { 
    if (x < 0 || x > 1.0f) {
        cerr << "Waning: get wrong tex coord x " << x << endl;
    }
    if (y < 0 || y > 1.0f) {
        cerr << "Waning: get wrong tex coord y " << y << endl;
    }
    if (z < 0 || z > 1.0f) {
        cerr << "Waning: get wrong tex coord z " << z << endl; 
    }
}

void ObjTextureCoordinate::parseObjLine(const std::string& line) {
    auto words = splitString(line, " ");
    if (words.size() > 4 || words.size() < 2 || words.front() != "vt") {
        Logger::error("parse obj vertex texture line: "s + line);
        return;
    }

    float x = stof(words[1]);
    float y = words.size() > 2 ? stof(words[2]) : 0;
    float z = words.size() > 3 ? stof(words[3]) : 0;
    coord = {x, y, z};
}

ObjVertexNorm::ObjVertexNorm(float x, float y, float z, bool needNormalize): norm({x, y, z}) {
    if (needNormalize) {
        normalize();
    }
}

void ObjVertexNorm::parseObjLine(const std::string& line) {
    auto words = splitString(line, " ");
    if (words.size() != 4 || words.front() != "vn") {
        Logger::error("parse obj vertex normal line: "s + line);
        return;
    }

    float x = stof(words[1]);
    float y = stof(words[2]);
    float z = stof(words[3]);
    norm = {x, y, z}; 
    normalize();
}

void ObjVertexNorm::normalize() {
    float len = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
    if (len == 0) {
        Logger::warn("try to normalize a vector length is 0");
        return;
    }
    norm[0] /= len;
    norm[1] /= len;
    norm[2] /= len; 
}

void ObjFace::parseObjLine(const std::string& line) {
    auto words = splitString(line, " ");
    if (words.size() < 2 || words.front() != "f") {
        Logger::error("parse face line: "s + line);
        return;
    }

    for (int i = 1; i < words.size(); ++i) {
        parseObjWord(words[i]);
    }
}

void ObjFace::parseObjWord(const std::string& word) {
    auto nums = splitString(word, "/");

    int v = stof(nums[0]);

    int vt = 0;
    if (nums.size() > 1 && !nums[1].empty()) {
        vt = stof(nums[1]);
    }

    int vn = 0;
    if (nums.size() > 2 && !nums[2].empty()) {
        vn = stof(nums[2]);
    }

    verts.push_back(v);
    texs.push_back(vt);
    norms.push_back(vn);
}

ObjLoader::ObjLoader(const std::string& file) {
    ifstream ifs(file);
    if (!ifs) {
        cerr << "Error: load obj file: " << file << " failed";
        return;
    }
    string line;

    while (getline(ifs, line)) {
        string token = line.substr(0, line.find(" "));
        if (token == "v") {
            shared_ptr<ObjBase> vert = make_shared<ObjVertex>();
            vert->parseObjLine(line);
            vertices.push_back(vert);
        } else if (token == "vt") {
            shared_ptr<ObjBase> tex = make_shared<ObjTextureCoordinate>();
            tex->parseObjLine(line);
            texCoords.push_back(tex);
        } else if (token == "vn") {
            shared_ptr<ObjBase> norm = make_shared<ObjVertexNorm>();
            norm->parseObjLine(line);
            texCoords.push_back(norm);
        } else if (token == "f") {
            shared_ptr<ObjBase> face = make_shared<ObjFace>();
            face->parseObjLine(line);
            faces.push_back(face);
        } else if (token == "#") {
            continue;
        } else {
            Logger::error("obj load, token not known: "s + token);
        }
    }
    
    cout << "parse obj file: " << file << " completed" << endl
        << "get vertex " << vertices.size() << endl
        << "get face " << faces.size() << endl;
}

void ObjLoader::render() {
    const int stride = 4 + 3 + 3;
    vector<float> vertData(vertices.size() * stride);
    for (int i = 0; i < vertices.size(); ++i) {
        auto vert = static_pointer_cast<ObjVertex>(vertices[i]);
        copy(vert->point.begin(), vert->point.end(), vertData.begin() + stride * i);
    }

    vector<unsigned int> indexData;    // unsigned int?
    for (int i = 0; i < faces.size(); ++i) {
        auto face = static_pointer_cast<ObjFace>(faces[i]);
        for (auto const& ind : face->verts) {
            indexData.push_back(ind - 1);
        }
    }
    triangleCount = indexData.size() / 3;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    int bufferSize = vertData.size() * sizeof(float);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, vertData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)4);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(4 + 3));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);
}

void ObjLoader::draw() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, triangleCount * 3, GL_UNSIGNED_INT, 0);
}