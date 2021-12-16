#include "obj_animator.h"
#include "cgutils.h"
#include "shader.h"

#include "glm/gtc/type_ptr.hpp"
#include "cgmacros.h"

using namespace std;

ObjAnimator::ObjAnimator(std::shared_ptr<Shader> s, const std::string& mvpMatrixName): mvpUniformName(mvpMatrixName), mvp(glm::mat4(1.0f)) {
    shader = s;

    if (s->getProgram() < 0 || mvpMatrixName.empty()) {
        Logger::error("init animator failed, shader: %d, name: %s", s->getProgram(), mvpMatrixName.c_str());
    }
}

void ObjAnimator::addDynamicActor(const ActorType& actor) {
    dynamicActors.push_back(std::move(actor));
}

void ObjAnimator::doProcess() {
    mvp = glm::mat4(1.0f);
    for (auto&& actor : dynamicActors) {
        actor->onNotifyTime(getTime());
        actor->onProcess(mvp);
    }

    shader->setMat4(mvpUniformName, VPTR(mvp));
    _glCheckError();
}

shared_ptr<AnimatorActor> AnimatorActor::getActor(const AnimationType t, const std::string& i)
{
    shared_ptr<AnimatorActor> actor = nullptr;
    switch (t) {
    case AnimationType::Rotate:
        actor = make_unique<AnimatorRotator>(t, i);
        Logger::message("get a rotate actor, id " + i);
        break;
    case AnimationType::Scale:
        Logger::message("get a scale actor, id " + i);
        actor = make_unique<AnimatorActor>(t, i);
        break;
    case AnimationType::Translate:
        Logger::message("get a translate actor, id " + i);
        actor = make_unique<AnimatorActor>(t, i);
        break;
    default:
        Logger::error("unknown actor type: %d", t);
        break;
    }
    return actor;
}

AnimatorActor::AnimatorActor(const AnimationType t, const std::string& i): type(t), id(i)
{
}

inline void AnimatorActor::onNotifyTime(double renderTime) {
    prevFrameTime = thisFrameTime;
    thisFrameTime = renderTime;
    deltaTime = renderTime - prevFrameTime;
    ++frameCount;
}

void AnimatorActor::setOrigin(const Vec3& og)
{
    origin = og;
    curState = og;
    direction = glm::normalize(og);
}

void AnimatorActor::onProcess(Mat4& mvp)
{
    curState = curState + (speed * direction * deltaTime);
    if (type == AnimationType::Translate) {
        mvp = glm::translate(mvp, curState);
    }
    else if (type == AnimationType::Scale) {
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

void AnimatorRotator::onProcess(Mat4 &mvp) {
    curState += Vec3(1.0f, 0, 0) * speed * deltaTime;
    mvp = glm::rotate(mvp, curState[0], origin);
}

void AnimatorRotator::setOrigin(const Vec3 &v) {
    origin = glm::normalize(v);
    curState = {0, 0, 0};
}
