#ifndef GLFW_USER_INTERFACE_H
#define GLFW_USER_INTERFACE_H

#include <memory>
#include "ui_base.h"
#include "camera.h"

class Camera;
class GLFWwindow;

class GLFWUserInterface: public UIBase {
public:
    GLFWUserInterface(std::shared_ptr<Camera> cam, GLFWwindow* window);

    void processEvents();

private:
    void scrollEvent(GLFWwindow* win, double xOffset, double yOffset);

    void keyEvent(GLFWwindow* win, int key, int scancode, int action, int mods);

    void mouseEvent(GLFWwindow* win, int button, int action, int mods);

    void mouseCursorPosEvent(GLFWwindow* win, double x, double y);

    void cameraTranslatePreview(GLFWwindow* win, double xPos, double yPos);

    void cameraRotatePreview(GLFWwindow* win, double xPos, double yPos);

    GLFWwindow* window;

    std::shared_ptr<Camera> camera;

    struct {
        std::unique_ptr<Camera> cameraCopy;
        double x0, y0;
    } mouseOrigin;
};

#endif //GRAPHICS_UI_BASE_H
