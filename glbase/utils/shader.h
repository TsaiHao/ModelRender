#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "cgutils.h"

class Texture {
public:
    explicit Texture(const std::string& imagePath = "");
    Texture(const Texture& tex);
    ~Texture() = default;

    void init();

    void setParam(GLenum type, GLint value) const;

    GLuint getTexture() const;

    void bind(int unit = 0) const;

private:
    std::string texFile;
    GLuint texture;
};

class Shader {
public:
    Shader() = default;
    Shader(const std::string &vertFile, const std::string &fragFile);
    Shader(const Shader& shd);
    ~Shader();

    void init();
    void use(bool bindTextures = true) const;

    void setInt(const std::string &name, int value) const;
    void setFloatVec4(const std::string &name, const float* value) const;
    void setMat4(const std::string &name, const float* value) const;
    void setModelMatrix(const float* mat);
    void setViewMatrix(const float* mat);

    void attachTexture(const std::string& texName, const Texture& tex);

    GLuint getProgram() const;

#pragma mark for debug use
    std::vector<std::string> getAllUniformList() const;

private:
    void compileAndLinkShader(const std::string &vertSource, const std::string &fragSource);
    static void checkCompileErrors(unsigned int shader, std::string type) ;
    GLint getUniformLocation(const std::string &name) const;
    static void modifyShaderVersion(std::string &shader) ;

    std::string vertSource, fragSource;

    GLuint vertShader;
    GLuint fragShader;
    GLuint program;

    std::vector<Texture> textures;

    std::unordered_map<std::string, GLint> uniformMap;
};
