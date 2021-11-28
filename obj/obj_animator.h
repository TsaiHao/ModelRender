#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <any>

#include "cgutils.h"

struct AnimatorScript {
public:
    typedef std::string ScriptParamKeyType;
    typedef std::unordered_map<ScriptParamKeyType, std::any> ScriptParamType;
    enum Type {
        Rotate, Translate, Scale
    };

    Type type;
    ScriptParamType params;

    template <typename T>
    bool getParam(const ScriptParamKeyType& key, T& value) const {
        auto iter = params.find(key);
        if (iter == params.end()) {
            Logger::error("get params value %s no this key", key.c_str());
            return false;
        }

        try {
            value = std::any_cast<T>(iter->second);
        } catch (std::bad_any_cast &e) {
            Logger::error("get param value %s cast type error", key.c_str());
            return false;
        }
        return true;
    }

    template <typename T>
    void setParam(const ScriptParamKeyType& key, const T& value) {
        params.emplace(key, std::make_any<T>(value));
    }

    auto begin() {
        return params.begin();
    }

    auto end() {
        return params.end();
    }
};

class ObjAnimator {

};