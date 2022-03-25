#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include <vector>
#include <memory>
#include <array>

#include "glm/glm.hpp"

class Shader;
class Camera {
public:
    using Vec3 = glm::vec3;

    //Camera();
    Camera(const Vec3 &pos, const Vec3 &target, const Vec3 &up);
    ~Camera();

    void setCameraVector(const Vec3 &pos, const Vec3 &target, const Vec3 &up);

    void setPosition(const Vec3 &pos);
    void setTarget(const Vec3 &target);
    void setUp(const Vec3 &up);
    const glm::mat4& getCameraViewMatrix() const;
    glm::vec4 getCameraPosition() const;

private:
    void updateViewMatrix();
    Vec3 cameraPosition;
    Vec3 cameraTarget;
    Vec3 cameraUp;
    glm::mat4 view;
};


#endif //GRAPHICS_CAMERA_H
