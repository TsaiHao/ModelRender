#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include <vector>
#include <memory>
#include <array>

class Shader;
class Camera {
public:
    using Vec3 = std::array<float, 3>;

    //Camera();
    Camera(const Vec3 &pos, const Vec3 &target, const Vec3 &up);
    ~Camera();

    void setCameraVector(const Vec3 &pos, const Vec3 &target, const Vec3 &up);

    void setPosition(const Vec3 &pos);
    void setTarget(const Vec3 &target);
    void setUp(const Vec3 &up);
    float* getCameraViewMatrix();

    class MathData;
private:
    void updateViewMatrix() const;
    std::unique_ptr<MathData> data;
};


#endif //GRAPHICS_CAMERA_H
