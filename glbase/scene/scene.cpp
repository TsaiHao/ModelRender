#include "scene.h"
#include "camera.h"

Scene::~Scene() = default;

Scene::Scene(std::unique_ptr<Camera> cam) {
    camera = std::move(cam);
}

void Scene::drawFrame() {
    updateViewMatrix();
    updateModelMatrix();
    doDrawFrame();
}

void Scene::updateViewMatrix() {

}

void Scene::updateModelMatrix() {

}

void Scene::doDrawFrame() {

}
