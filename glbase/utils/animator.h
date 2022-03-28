#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <any>
#include <memory>

#include "../thirdparty/glm/glm/glm.hpp"
#include "base_render.h"


class Shader;

using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;

enum class AnimationType {
    Translate, Rotate, Scale
};

class AnimatorActor {
public:
    static std::shared_ptr<AnimatorActor> getActor(AnimationType t, const std::string &i = "");

    AnimatorActor(AnimationType t, std::string i);

    virtual void onNotifyTime(double renderTime);

    // origin is direction for translate, pivot for rotate
    virtual void setOrigin(const Vec3 &);

    virtual void setSpeed(float s);

    virtual void setExtra(const std::string &configKey, const std::any &value);

    virtual void setPeriod(float p);
    //virtual void setExternal(const std::string& key, const std::any& value) {}

    virtual void onProcess(Mat4 &mvp);

    virtual ~AnimatorActor() = default;

    std::string id;
    AnimationType type;

protected:
    double thisFrameTime = 0;
    double prevFrameTime = 0;
    float deltaTime = 0;
    float periodTime = -1;
    float elapsedTime = 0;
    unsigned int frameCount = 0;

    Vec3 direction{};
    Vec3 origin{};
    Vec3 curState{};
    float speed = 0;

    virtual void checkPeriod();
};

class AnimatorRotator : public AnimatorActor {
public:
    AnimatorRotator(AnimationType t, const std::string &i);

    void setOrigin(const Vec3 &v) override;

    void onProcess(Mat4 &mvp) override;

    void setExtra(const std::string &configKey, const std::any &value) override;

private:
    float staticAngle = 0;
};

class Animator {
public:
    Animator() = default;

    Animator& operator=(const Animator& rhs);

    Animator(const Animator& rhs);

    explicit Animator(std::shared_ptr<Shader> s);

    void addDynamicActor(std::shared_ptr<AnimatorActor> &actor);

    void doProcess();

    Vec4 processSinglePoint(const Vec4& point);

    Mat4 getModelMatrix();

private:
    std::shared_ptr<Shader> shader;
    std::vector<std::shared_ptr<AnimatorActor>> dynamicActors;
};