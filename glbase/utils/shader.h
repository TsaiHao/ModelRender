#pragma once

#include <string>
#include <vector>
#include <memory>
#include "cgutils.h"
#include "texture.h"

class Shader {
public:
    Shader() = default;
    Shader(std::string vertFile, std::string fragFile);
    Shader(const Shader& shd);
    ~Shader();

    void updateShaders(const std::string& vf, const std::string& ff);

    void use(bool bindTextures = true) const;

    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatVec4(const std::string &name, const float* value) const;
    void setMat4(const std::string &name, const float* value) const;

    void setModelMatrix(const float* mat) const;
    void setViewMatrix(const float* mat) const;
    void setProjectionMatrix(const float* mat) const;

    void attachTexture(const std::string &tex);
    std::shared_ptr<YUVTexture> attachTexture();

#pragma mark for debug use
    [[nodiscard]] std::vector<std::string> getAllUniformList() const;

private:
    GLint getUniformLocation(const std::string &name) const;
    static void modifyShaderVersion(std::string &shader) ;

    void init();

    GLuint vertShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;

    std::string vertSourceFile;
    std::string fragSourceFile;

    std::vector<std::shared_ptr<Texture>> textures;
};
