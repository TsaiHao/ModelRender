#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <array>
#include <any>
#include <memory>

#include "../glm/glm/glm.hpp"

class Shader;

using Vec3 = glm::vec3;
using Mat4 = glm::mat4;

enum class AnimationType {
    Translate, Rotate, Scale
};

class AnimatorActor {
public:
    static std::shared_ptr<AnimatorActor> getActor(AnimationType t, const std::string &i);

    AnimatorActor(AnimationType t, std::string i);

    virtual void onNotifyTime(double renderTime);

    // origin is direction for translate, pivot for rotate
    virtual void setOrigin(const Vec3 &);

    virtual void setSpeed(const float s) {
        speed = s;
    }

    virtual void setPeriod(const float p) {
        periodTime = p;
    }
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
};

class ObjAnimator {
public:
    using ActorType = std::shared_ptr<AnimatorActor>;

    ObjAnimator() = default;

    explicit ObjAnimator(std::shared_ptr<Shader> s, const std::string &mvpMatrixName = "mvp");

    void addDynamicActor(const ActorType &actor);

    void doProcess();

private:
    std::shared_ptr<Shader> shader;
    std::string mvpUniformName;
    std::vector<ActorType> dynamicActors;       // set every loop
    glm::mat4 mvp;
};