#include "camera.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "cgmacros.h"

class CameraData {
public:
    glm::mat4 view;
    Camera::Vec3 position;
    Camera::Vec3 target;
    Camera::Vec3 up;
};

inline static void calculateLookAt(std::unique_ptr<CameraData>& data) {
    data->view = glm::lookAt(
            glm::vec3(data->position[0], data->position[1], data->position[2]),
            glm::vec3(data->target[0], data->target[1], data->target[2]),
            glm::vec3(data->up[0], data->up[1], data->up[2])
    );
}

Camera::Camera(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up): data(std::make_unique<CameraData>()) {
    setCameraVector(pos, target, up);
}

void Camera::setCameraVector(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    data->position = pos;
    data->target = target;
    data->up = up;
    calculateLookAt(data);
}

void Camera::setPosition(const Camera::Vec3 &pos) {
    data->position = pos;
    calculateLookAt(data);
}

void Camera::setTarget(const Camera::Vec3 &target) {
    data->target = target;
    calculateLookAt(data);
}

void Camera::setUp(const Camera::Vec3 &up) {
    data->up = up;
    calculateLookAt(data);
}

Camera::Camera(const Camera &rhs) {
    *this = rhs;
}

Camera &Camera::operator=(const Camera &rhs) {
    if (this == &rhs) {
        return *this;
    }

    *data = *(rhs.data);
    return *this;
}

const float *Camera::getViewMatrix() const {
    return VPTR(data->view);
}

Camera::~Camera() = default;
