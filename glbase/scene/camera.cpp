#include "camera.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"

//Camera::Camera() = default;

Camera::Camera(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    setCameraVector(pos, target, up);
}


Camera::Camera(const Camera& cam) {
    cameraPosition = cam.cameraPosition;
    cameraTarget = cam.cameraTarget;
    cameraUp = cam.cameraUp;
    updateViewMatrix();
}

void Camera::setCameraVector(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    setPosition(pos);
    setTarget(target);
    setUp(up);
}

void Camera::setPosition(const Camera::Vec3 &pos) {
    cameraPosition = pos;
    updateViewMatrix();
}

void Camera::setTarget(const Camera::Vec3 &target) {
    cameraTarget = target;
    updateViewMatrix();
}

void Camera::setUp(const Camera::Vec3 &up) {
    cameraUp = up;
    updateViewMatrix();
}

void Camera::updateViewMatrix() {
     view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
     if (isnan(view[0][0])) {
         Logger::error("nan detected");
     }
}

const glm::mat4& Camera::getCameraViewMatrix() const {
    return view;
}

glm::vec4 Camera::getCameraPosition() const {
    return glm::vec4(cameraPosition, 1.0f);
}

Camera::~Camera() = default;
