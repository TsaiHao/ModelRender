#include "animator.h"

#include <utility>
#include "cgutils.h"
#include "shader.h"

#include "glm/gtc/type_ptr.hpp"
#include "cgdefs.h"

using namespace std;

Animator::Animator(std::shared_ptr<Shader> s): shader(std::move(s)) {
}

void Animator::addDynamicActor(std::shared_ptr<AnimatorActor> &actor) {
    dynamicActors.push_back(std::move(actor));
}

void Animator::doProcess() {
    auto mvp = glm::mat4(1.0f);
    auto time = getTime();
    for (auto &&actor: dynamicActors) {
        actor->onNotifyTime(time);
        actor->onProcess(mvp);
    }

    shader->setModelMatrix(VPTR(mvp));
}

Animator &Animator::operator=(const Animator &rhs) {
    if (&rhs == this) {
        return *this;
    }
    shader = rhs.shader;
    dynamicActors = rhs.dynamicActors;
    return *this;
}

Animator::Animator(const Animator &rhs) {
    *this = rhs;
}

Vec4 Animator::processSinglePoint(const Vec4 &point) {
    auto mvp = Mat4(1.0f);
    for (auto&& actor : dynamicActors) {
        actor->onProcess(mvp);
    }

    return mvp * point;
}

Mat4 Animator::getModelMatrix() {
   Mat4 model = glm::mat4(1.0f);
   for (auto&& actor: dynamicActors) {
       actor->onProcess(model);
   }
   return model;
}

shared_ptr<AnimatorActor> AnimatorActor::getActor(const AnimationType t, const std::string &i) {
    shared_ptr<AnimatorActor> actor = nullptr;
    switch (t) {
        case AnimationType::Rotate:
            actor = make_shared<AnimatorRotator>(t, i);
            Logger::info("get a rotate actor, id " + i);
            break;
        case AnimationType::Scale:
            Logger::info("get a scale actor, id " + i);
            actor = make_shared<AnimatorActor>(t, i);
            break;
        case AnimationType::Translate:
            Logger::info("get a translate actor, id " + i);
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
