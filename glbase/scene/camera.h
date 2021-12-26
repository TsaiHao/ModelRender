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
    
    void attachShader(const std::shared_ptr<Shader> &sd, const std::string uf = "camera");
    
    void setCameraVector(const Vec3 &pos, const Vec3 &target, const Vec3 &up);

    void setPosition(const Vec3 &pos);
    void setTarget(const Vec3 &target);
    void setUp(const Vec3 &up);

    void updateUniform() const;
    
private:
    std::shared_ptr<Shader> shader;

    Vec3 cameraPosition;
    Vec3 cameraTarget;
    Vec3 cameraUp;

    std::string uniform;
};


#endif //GRAPHICS_CAMERA_H
