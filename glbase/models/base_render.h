#ifndef GRAPHICS_BASERENDER_H
#define GRAPHICS_BASERENDER_H

#include <string>
#include <memory>
#include <vector>

enum class GeometryType {
    Triangle,
    Rectangle,
    Circle,
    Custom
};

class AnimatorActor;
class Animator;
class Shader;

class BaseRender {
public:
    ~BaseRender() = default;

    virtual void draw() = 0;

    virtual void initRender() = 0;

    virtual void updateViewMatrix(const float* mat) const = 0;

    virtual void updateProjectionMatrix(const float* mat) const = 0;

    virtual void attachShaders(const std::string& vsFile, const std::string& fsFile) = 0;

    virtual void attachTexture(const std::string& imagePath) = 0;

    virtual void setVisibility(bool isVisible) = 0;

    virtual bool getVisibility() = 0;

    /******
     * translate model
     * @param x translate direction x
     * @param y translate direction y
     * @param z translate direction z
     * @param speed normalized speed per second
     */
    virtual void translate(float x, float y, float z, float speed = 0) const = 0;

    /*******
     * rotate model angle radian around axis
     * @param x pivot axis x
     * @param y pivot axis y
     * @param z pivot axis z
     * @param angle rotation angle in radian
     * @param speed delta rotation in radian per second
     */
    virtual void rotate(float x, float y, float z, float angle, float speed = 0) const = 0;

    /*******
     * scale model in three dimensions
     * @param x x axis scale
     * @param y y axis scale
     * @param z z axis scale
     */
    virtual void scale(float x, float y, float z) const = 0;
};

std::shared_ptr<BaseRender> createModel(const std::string& modelFile);

std::shared_ptr<BaseRender> createModel(GeometryType geo);

#endif //GRAPHICS_BASERENDER_H
