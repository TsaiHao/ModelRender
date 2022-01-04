#ifndef GRAPHICS_CAMERA_H
#define GRAPHICS_CAMERA_H

#include <vector>
#include <memory>
#include <array>
#include <string>

class CameraData;
class Camera {
public:
    using Vec3 = std::array<float, 3>;

    //Camera();
    Camera(const Vec3 &pos, const Vec3 &target, const Vec3 &up);
    Camera(const Camera& rhs);
    Camera& operator=(const Camera& rhs);
    ~Camera();
    
    void setCameraVector(const Vec3 &pos, const Vec3 &target, const Vec3 &up);

    void setPosition(const Vec3 &pos);
    void setTarget(const Vec3 &target);
    void setUp(const Vec3 &up);

    const float* getViewMatrix() const;

private:
    std::unique_ptr<CameraData> data;
};


#endif //GRAPHICS_CAMERA_H
