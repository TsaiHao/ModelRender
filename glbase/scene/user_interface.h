#ifndef GRAPHICS_USER_INTERFACE_H
#define GRAPHICS_USER_INTERFACE_H

#include <memory>
#include "camera.h"

class Camera;
class GLFWwindow;

class GLFWUserInterface {
    enum class State: int {
        Idle,
        CameraTranslating,
        CameraRotating,
    };

public:
    GLFWUserInterface(std::shared_ptr<Camera> cam, GLFWwindow* window);

private:
    void scrollEvent(GLFWwindow* win, double xOffset, double yOffset);

    void keyEvent(GLFWwindow* win, int key, int scancode, int action, int mods);

    void mouseEvent(GLFWwindow* win, int button, int action, int mods);

    void mouseCursorPosEvent(GLFWwindow* win, double x, double y);

    void cameraTranslatePreview(GLFWwindow* win, double xPos, double yPos);

    void cameraRotatePreview(GLFWwindow* win, double xPos, double yPos);

    State state;

    GLFWwindow* window;

    std::shared_ptr<Camera> camera;

    struct {
        std::unique_ptr<Camera> cameraCopy;
        double x0, y0;
    } mouseOrigin;
};



#endif //GRAPHICS_USER_INTERFACE_H
