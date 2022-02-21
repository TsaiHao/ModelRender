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

    void bind(int unit = 0) const;

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

    void setModelMatrix(const float* mat) const;
    void setViewMatrix(const float* mat) const;
    void setProjectionMatrix(const float* mat) const;

    void attachTexture(const std::string& texName, const Texture& tex);

#pragma mark for debug use
    std::vector<std::string> getAllUniformList() const;

private:
    GLint getUniformLocation(const std::string &name) const;
    static void modifyShaderVersion(std::string &shader) ;

    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

    std::vector<Texture> textures;
};
