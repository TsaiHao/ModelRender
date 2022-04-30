#include "cgutils.h"
#include "glfw_user_interface.h"

#include <utility>
#include <functional>

#include "camera.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;
using namespace std::placeholders;

static inline GLFWUserInterface* getThis(GLFWwindow* win) {
    return static_cast<GLFWUserInterface*>(glfwGetWindowUserPointer(win));
}

GLFWUserInterface::GLFWUserInterface(std::shared_ptr<Camera> cam, GLFWwindow* win):
    camera(cam), window(win), state(State::Idle)
{
    camera = std::move(cam);

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        getThis(w)->keyEvent(w, key, scancode, action, mods);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y) {
        getThis(w)->scrollEvent(w, x, y);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        getThis(win)->mouseCursorPosEvent(win, x, y);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow *win, int button, int action, int mods) {
        getThis(win)->mouseEvent(win, button, action, mods);
    });

    Logger::message("glfw user interface created");
}

void GLFWUserInterface::processEvents() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void GLFWUserInterface::scrollEvent(GLFWwindow *win, double xOffset, double yOffset) {
    float speed = 0.1f;
    auto& curPos = camera->cameraPosition;
    auto delta = glm::normalize(camera->cameraTarget - curPos) * speed * float(yOffset);
    //GLFWUserInterface::camera->cameraTarget += delta;
    camera->cameraPosition += delta;
    camera->updateViewMatrix();
}

void GLFWUserInterface::keyEvent(GLFWwindow* win, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }
}

void GLFWUserInterface::mouseEvent(GLFWwindow *win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            Logger::message("left mouse button pressed");

            mouseOrigin = { make_unique<Camera>(*camera), 0, 0 };
            glfwGetCursorPos(window, &mouseOrigin.x0, &mouseOrigin.y0);

            glfwSetCursorPosCallback(win, [](GLFWwindow* win, double x, double y) {
                getThis(win)->cameraTranslatePreview(win, x, y);
            });

            state = State::CameraTranslating;
        } else if (action == GLFW_RELEASE) {
            Logger::message("left mouse button released");

            glfwSetCursorPosCallback(win, [](GLFWwindow* win, double x, double y) { } );

            mouseOrigin = { nullptr, 0, 0 };
            state = State::Idle;
        }
    }
}

void GLFWUserInterface::cameraTranslatePreview(GLFWwindow* win, double xPos, double yPos) {
    if (state != State::CameraTranslating) {
        return;
    }

    int w, h;
    glfwGetWindowSize(win, &w, &h);
    double dx = -(xPos - mouseOrigin.x0) / (w / 2);
    double dy = (yPos - mouseOrigin.y0) / (h / 2);

    glm::vec4 delta = glm::inverse(camera->view)  * glm::vec4(dx, dy, 0, 0);
    delta = glm::normalize(delta) * glm::length(glm::vec3(dx, dy, 0));

    if (isnan(delta[0])) {
        // when mouse is too close to origin, delta could be nan, skip it
        return;
    }
    camera->cameraPosition = mouseOrigin.cameraCopy->cameraPosition + glm::vec3(delta);
    camera->cameraTarget = mouseOrigin.cameraCopy->cameraTarget + glm::vec3(delta);
    camera->updateViewMatrix();
}

void GLFWUserInterface::cameraRotatePreview(GLFWwindow *win, double xPos, double yPos) {

}

void GLFWUserInterface::mouseCursorPosEvent(GLFWwindow* win, double x, double y) {
    // null event
}
