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

    int v = stof(nums[0]) - 1;

    int vt = 0;
    if (nums.size() > 1 && !nums[1].empty()) {
        vt = stof(nums[1]) - 1;
    }

    int vn = 0;
    if (nums.size() > 2 && !nums[2].empty()) {
        vn = stof(nums[2]) - 1;
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
            auto vert = ObjVertex();
            vert.parseObjLine(line);
            vertices.push_back(vert);
        } else if (token == "vt") {
            auto tex = ObjTextureCoordinate();
            tex.parseObjLine(line);
            texCoords.push_back(tex);
        } else if (token == "vn") {
            auto norm = ObjVertexNorm();
            norm.parseObjLine(line);
            normVecs.push_back(norm);
        } else if (token == "f") {
            auto face = ObjFace();
            face.parseObjLine(line);
            faces.push_back(face);
        } else if (token == "#") {
            continue;
        } else if (token == "s") {

        } else if (token == "o") {
            auto words = splitString(line, " ");
            objName = words[1];
        } else if (token == "usemtl") {
            auto words = splitString(line, " ");
            materialName = words[1];
        } else if (token == "mtllib") {
            auto words = splitString(line, " ");
            materialFile = words[1];
        } else {
            Logger::error("obj load, token not known: "s + token);
        }
    }
    
    cout << "parse obj file: " << file << " completed" << endl
        << "get vertex " << vertices.size() << endl
        << "get face " << faces.size() << endl;
}