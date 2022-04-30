#ifndef GRAPHICS_UI_BASE_H
#define GRAPHICS_UI_BASE_H

#include <memory>

class UIBase {
protected:
    enum class State: int {
        Idle,
        CameraTranslating,
        CameraRotating,
    };

public:
    virtual ~UIBase() = default;

    virtual void processEvents() = 0;

    State state;
};

#endif //GRAPHICS_UI_BASE_H
