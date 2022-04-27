#include "base_render_impl.h"
#include "obj_render.h"
#include "geometry_render.h"
#include "base_render.h"

#include <memory>
#include "animator.h"

#include "shader.h"

VertexBufferSize convertVertexBufferFormat(VertexBufferFormat format) {
    switch (format) {
        case VertexBufferFormat::PTN433:
            return { {4, 3, 3} };
        case VertexBufferFormat::PT32:
            return { {3, 2} };
    }
}

void BaseRenderImpl::updateViewMatrix(const float *mat) const {
    shader->setViewMatrix(mat);
}

void BaseRenderImpl::updateProjectionMatrix(const float *mat) const {
    shader->setProjectionMatrix(mat);
}

void BaseRenderImpl::attachShaders(const std::string &vsFile, const std::string &fsFile) {
    shader = std::make_shared<Shader>(vsFile, fsFile);
    animator = std::make_shared<Animator>(shader);
}

void BaseRenderImpl::translate(float x, float y, float z, float speed) const {
    auto translator = AnimatorActor::getActor(AnimationType::Translate);
    translator->setOrigin(glm::vec3(x, y, z));
    translator->setSpeed(speed);

    animator->addDynamicActor(translator);
}

void BaseRenderImpl::rotate(float x, float y, float z, float angle, float speed) const {
    auto rotate = AnimatorRotator::getActor(AnimationType::Rotate);
    rotate->setOrigin(glm::vec3(x, y, z));
    rotate->setSpeed(speed);
    rotate->setExtra(ROTATE_STATIC_ANGLE, angle);

    animator->addDynamicActor(rotate);
}

void BaseRenderImpl::scale(float x, float y, float z) const {
    auto scale = AnimatorActor::getActor(AnimationType::Scale);
    scale->setOrigin(glm::vec3(x, y, z));

    animator->addDynamicActor(scale);
}

void BaseRenderImpl::attachTexture(const std::string &imagePath) {
    shader->attachTexture(imagePath);
}

void BaseRenderImpl::setVisibility(bool visible) {
    isVisible = visible;
}

bool BaseRenderImpl::getVisibility() {
    return isVisible;
}

void BaseRenderImpl::initGL() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    auto format = convertVertexBufferFormat(bufferFormat);
    auto stride = format.stride() * sizeof(decltype(vertices)::value_type);

    for (unsigned long accSize = 0, i = 0; i < format.sizes.size(); ++i) {
        glVertexAttribPointer(i, format.sizes[i], GL_FLOAT, GL_FALSE, stride, (void*)accSize);
        accSize += format.sizes[i] * sizeof(decltype(vertices)::value_type);
        glEnableVertexAttribArray(i);
    }
    _glCheckError();

    glBindVertexArray(0);
}

std::shared_ptr<BaseRender> createModel(const std::string &modelFile) {
    if (modelFile.size() > 4 && modelFile.substr(modelFile.size() - 4, 4) == ".obj") {
        return std::make_shared<ObjRender>(modelFile);
    }
    return nullptr;
}

std::shared_ptr<BaseRender> createModel(GeometryType geo) {
    return std::make_shared<PolygonRender>(geo);
}