#ifndef GRAPHICS_OBJ_RENDER_H
#define GRAPHICS_OBJ_RENDER_H

#include <vector>
#include <memory>
#include "glad/glad.h"

#include "obj_loader.h"
#include "shader.h"

class ObjRender {
public:
    explicit ObjRender(std::shared_ptr<ObjLoader> loader);
    explicit ObjRender(std::string const& objFile, const std::string &vsFile = "", const std::string& fsFile = "");

    void bufferData();
    void draw();

    void attachShader(const std::string& vsFile, const std::string& fsFile);
    Shader& getShader();

private:
    GLuint VAO, VBO;
    std::shared_ptr<ObjLoader> obj;
    Shader shader;
    std::vector<float> vertices;        // store vertices' position/texture/normal data
};


#endif //GRAPHICS_OBJ_RENDER_H
