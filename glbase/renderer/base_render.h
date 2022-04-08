#ifndef GRAPHICS_BASERENDER_H
#define GRAPHICS_BASERENDER_H

#include <string>
#include <memory>
#include <vector>

class AnimatorActor;
class Animator;
class Shader;

class BaseRender {
public:
    ~BaseRender() = default;

    virtual void draw() = 0;

    virtual void initRender() = 0;

    virtual void updateViewMatrix(const float* mat) const;

    virtual void updateProjectionMatrix(const float* mat) const;

    void attachShaders(const std::string& vsFile, const std::string& fsFile);

    void attachTexture(const std::string& imagePath);

    /******
     * translate model
     * @param x translate direction x
     * @param y translate direction y
     * @param z translate direction z
     * @param speed normalized speed per second
     */
    void translate(float x, float y, float z, float speed = 0) const;

    /*******
     * rotate model angle radian around axis
     * @param x pivot axis x
     * @param y pivot axis y
     * @param z pivot axis z
     * @param angle rotation angle in radian
     * @param speed delta rotation in radian per second
     */
    void rotate(float x, float y, float z, float angle, float speed = 0) const;

    /*******
     * scale model in three dimensions
     * @param x x axis scale
     * @param y y axis scale
     * @param z z axis scale
     */
    void scale(float x, float y, float z) const;


    std::shared_ptr<Shader> shader = nullptr;
    std::shared_ptr<Animator> animator = nullptr;
};

std::shared_ptr<BaseRender> createModel(const std::string& modelFile);

#endif //GRAPHICS_BASERENDER_H
