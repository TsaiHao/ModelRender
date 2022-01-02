#include "camera.h"
#include "shader.h"
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "cgmacros.h"

//Camera::Camera() = default;

Camera::Camera(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    setCameraVector(pos, target, up);
}

void Camera::attachShader(const std::shared_ptr<Shader> &sd, const std::string& uf) {
    this->shader = sd;
    this->uniform = uf;
    updateUniform();
}

void Camera::setCameraVector(const Camera::Vec3 &pos, const Camera::Vec3 &target, const Camera::Vec3 &up) {
    cameraPosition = pos;
    cameraTarget = target;
    cameraUp = up;
}

void Camera::setPosition(const Camera::Vec3 &pos) {
    cameraPosition = pos;
}

void Camera::setTarget(const Camera::Vec3 &target) {
    cameraTarget = target;
}

void Camera::setUp(const Camera::Vec3 &up) {
    cameraUp = up;
}

void Camera::updateUniform() const {
    auto camMat = glm::lookAt(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]),
                              glm::vec3(cameraTarget[0], cameraTarget[1], cameraTarget[2]),
                              glm::vec3(cameraUp[0], cameraUp[1], cameraUp[2]));
    shader->setMat4(uniform, VPTR(camMat));
}

Camera::Camera(const Camera &rhs) {
    *this = rhs;
}

Camera &Camera::operator=(const Camera &rhs) {
    if (this == &rhs) {
        return *this;
    }

    shader = rhs.shader;
    cameraPosition = rhs.cameraPosition;
    cameraTarget = rhs.cameraTarget;
    cameraUp = rhs.cameraUp;
    uniform = rhs.uniform;
    return *this;
}

Camera::~Camera() = default;
