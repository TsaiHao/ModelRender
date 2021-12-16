#pragma once

#include <string>
#include <vector>
#include "cgutils.h"

class Texture {
public:
    explicit Texture(const std::string& imagePath = "");
    Texture(const Texture& tex);
    ~Texture() = default;

    void setParam(GLenum type, GLint value) const;

    GLuint getTexture() const;

    void bind(const int unit = 0) const;

private:
    GLuint texture;
};

class Shader {
public:
    Shader() = default;
    Shader(const std::string &vertFile, const std::string &fragFile);
    Shader(const Shader& shd);
    ~Shader();

    void use(bool bindTextures = true) const;

    void setInt(const std::string &name, int value) const;
    void setFloatVec4(const std::string &name, const float* value) const;
    void setMat4(const std::string &name, const float* value) const;

    void attachTexture(const std::string& texName, const Texture& tex);

    GLuint getProgram() const;

private:
    void checkCompileErrors(unsigned int shader, std::string type) const;
    GLint getUniformLocation(const std::string &name) const;

    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

    std::vector<Texture> textures;
};