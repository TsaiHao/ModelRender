#include "obj_animator.h"

#include <utility>
#include "cgutils.h"
#include "shader.h"

#include "glm/gtc/type_ptr.hpp"
#include "cgmacros.h"

using namespace std;

ObjAnimator::ObjAnimator(std::shared_ptr<Shader> s, const std::string &mvpMatrixName) : mvpUniformName(mvpMatrixName),
                                                                                        mvp(glm::mat4(1.0f)),
                                                                                        shader(std::move(s)) {
    if (s->getProgram() < 0 || mvpMatrixName.empty()) {
        Logger::error("init animator failed, shader: %d, name: %s", s->getProgram(), mvpMatrixName.c_str());
    }
}

void ObjAnimator::addDynamicActor(const ActorType &actor) {
    dynamicActors.push_back(actor);
}

void ObjAnimator::doProcess() {
    mvp = glm::mat4(1.0f);
    auto time = getTime();
    for (auto &&actor: dynamicActors) {
        actor->onNotifyTime(time);
        actor->onProcess(mvp);
    }

    shader->setMat4(mvpUniformName, VPTR(mvp));
    _glCheckError();
}

shared_ptr<AnimatorActor> AnimatorActor::getActor(const AnimationType t, const std::string &i) {
    shared_ptr<AnimatorActor> actor = nullptr;
    switch (t) {
        case AnimationType::Rotate:
            actor = make_shared<AnimatorRotator>(t, i);
            Logger::message("get a rotate actor, id " + i);
            break;
        case AnimationType::Scale:
            Logger::message("get a scale actor, id " + i);
            actor = make_shared<AnimatorActor>(t, i);
            break;
        case AnimationType::Translate:
            Logger::message("get a translate actor, id " + i);
            actor = make_shared<AnimatorActor>(t, i);
            break;
        default:
            Logger::error("unknown actor type: %d", t);
            break;
    }
    return actor;
}

AnimatorActor::AnimatorActor(const AnimationType t, std::string i) : type(t), id(std::move(i)) {
}

inline void AnimatorActor::onNotifyTime(double renderTime) {
    prevFrameTime = thisFrameTime;
    thisFrameTime = renderTime;
    deltaTime = renderTime - prevFrameTime;
    ++frameCount;
}

void AnimatorActor::setOrigin(const Vec3 &og) {
    origin = og;
    curState = og;
    direction = glm::normalize(og);
}

void AnimatorActor::onProcess(Mat4 &mvp) {
    curState = curState + (speed * direction * deltaTime);
    if (type == AnimationType::Translate) {
        mvp = glm::translate(mvp, curState);
    } else if (type == AnimationType::Scale) {
        mvp = glm::scale(mvp, curState);
    }
}

void AnimatorActor::checkPeriod() {
    if (periodTime < 0) {
        return;
    }
    if (elapsedTime > periodTime) {
        curState = origin;
        elapsedTime = 0;
    } else {
        elapsedTime += deltaTime;
    }
}

void AnimatorActor::setPeriod(const float p) {
    periodTime = p;
}

void AnimatorActor::setSpeed(const float s) {
    speed = s;
}

void AnimatorActor::setExtra(const string &configKey, const any &value) {

}

void AnimatorRotator::onProcess(Mat4 &mvp) {
    if (frameCount > 1 && FLOAT_EQUAL(speed, 0)) {
        curState += Vec3(1.0f, 0, 0) * speed * deltaTime;
    }
    mvp = glm::rotate(mvp, curState[0], origin);
}

void AnimatorRotator::setOrigin(const Vec3 &v) {
    origin = glm::normalize(v);
    curState = {0, 0, 0};
}

AnimatorRotator::AnimatorRotator(AnimationType t, const string &i) : AnimatorActor(t, i) {
    curState = Vec3(0, 0, 0);
}

void AnimatorRotator::setExtra(const string &configKey, const any &value) {
    AnimatorActor::setExtra(configKey, value);
    if (configKey == ROTATE_STATIC_ANGLE) {
        staticAngle = any_cast<float>(value);
        curState = Vec3(staticAngle, 0, 0);
    }
}
