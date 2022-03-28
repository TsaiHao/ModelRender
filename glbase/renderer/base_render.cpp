#include "base_render.h"
#include "animator.h"
#include "shader.h"

#include "obj_render.h"

void BaseRender::updateViewMatrix(const float *mat) const {
    shader->setViewMatrix(mat);
}

void BaseRender::updateProjectionMatrix(const float *mat) const {
    shader->setProjectionMatrix(mat);
}

void BaseRender::attachShaders(const std::string &vsFile, const std::string &fsFile) {
    shader = std::shared_ptr<Shader>(new Shader(vsFile, fsFile));
    animator = std::make_shared<Animator>(shader);
}

void BaseRender::translate(float x, float y, float z, float speed) const {
    auto translator = AnimatorActor::getActor(AnimationType::Translate);
    translator->setOrigin(glm::vec3(x, y, z));
    translator->setSpeed(speed);

    animator->addDynamicActor(translator);
}

void BaseRender::rotate(float x, float y, float z, float angle, float speed) const {
    auto rotate = AnimatorRotator::getActor(AnimationType::Rotate);
    rotate->setOrigin(glm::vec3(x, y, z));
    rotate->setSpeed(speed);
    rotate->setExtra(ROTATE_STATIC_ANGLE, angle);

    animator->addDynamicActor(rotate);
}

void BaseRender::scale(float x, float y, float z) const {
    auto scale = AnimatorActor::getActor(AnimationType::Scale);
    scale->setOrigin(glm::vec3(x, y, z));

    animator->addDynamicActor(scale);
}


std::shared_ptr<BaseRender> createModel(const std::string &modelFile) {
    if (modelFile.size() > 4 && modelFile.substr(modelFile.size() - 4, 4) == ".obj") {
        return std::make_shared<ObjRender>(modelFile);
    }
    return nullptr;
}
