#ifndef GRAPHICS_BASE_RENDER_IMPL_H
#define GRAPHICS_BASE_RENDER_IMPL_H

#include <numeric>
#include <vector>
#include "base_render.h"

#include "glad/glad.h"

// vertex buffer order and size, P for position, T for texture, N for normal
enum class VertexBufferFormat {
    PTN433,     // wave front obj format
    PT32,       // 2d polygon format
};

struct VertexBufferSize {
    std::vector<int> sizes;
    GLint stride() { return std::accumulate(sizes.begin(), sizes.end(), 0); }
};

VertexBufferSize convertVertexBufferFormat(VertexBufferFormat format);

class BaseRenderImpl: public BaseRender {
public:
    void updateViewMatrix(const float* mat) const override;

    void updateProjectionMatrix(const float* mat) const override;

    void attachShaders(const std::string& vsFile, const std::string& fsFile) override;

    void attachTexture(const std::string& imagePath) override;

    void setVisibility(bool visible) override;

    bool getVisibility() override;

    void translate(float x, float y, float z, float speed) const override;

    void rotate(float x, float y, float z, float angle, float speed) const override;

    void scale(float x, float y, float z) const override;

    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Animator> animator = nullptr;

protected:
    virtual void initGL();

    GLuint VAO;
    GLuint VBO;
    VertexBufferFormat bufferFormat = VertexBufferFormat::PTN433;
    std::vector<float> vertices;        // store vertices' position/texture/normal data

    bool isVisible = true;
};


#endif //GRAPHICS_BASE_RENDER_IMPL_H
