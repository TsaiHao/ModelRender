#include "camera.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "cgmacros.h"

class Camera::MathData {
public:
    Vec3 cameraPosition;
    Vec3 cameraTarget;
    Vec3 cameraUp;
    glm::mat4 view;
};
//Camera::Camera() = default;

Camera::Camera(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up):
    data(std::make_unique<MathData>()) {
    setCameraVector(pos, target, up);
}

void Camera::setCameraVector(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    data->cameraPosition = pos;
    data->cameraTarget = target;
    data->cameraUp = up;
}

void Camera::setPosition(const Camera::Vec3 &pos) {
    if (isVectorEqual(pos, data->cameraPosition)) {
        return;
    }
    data->cameraPosition = pos;
    updateViewMatrix();
}

void Camera::setTarget(const Camera::Vec3 &target) {
    if (isVectorEqual(target, data->cameraTarget)) {
        return;
    }
    data->cameraTarget = target;
    updateViewMatrix();
}

void Camera::setUp(const Camera::Vec3 &up) {
    if (isVectorEqual(up, data->cameraUp)) {
        return;
    }
    data->cameraUp = up;
    updateViewMatrix();
}

void Camera::updateViewMatrix() const {
     data->view = glm::lookAt(glm::vec3(data->cameraPosition[0], data->cameraPosition[1], data->cameraPosition[2]),
                              glm::vec3(data->cameraTarget[0], data->cameraTarget[1], data->cameraTarget[2]),
                              glm::vec3(data->cameraUp[0], data->cameraUp[1], data->cameraUp[2]));
}

float *Camera::getCameraViewMatrix() {
    return VPTR(data->view);
}

Camera::~Camera() = default;
