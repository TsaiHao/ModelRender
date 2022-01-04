#include "scene.h"
#include "camera.h"
#include <gl_context.h>

#include "obj_render.h"
#include "cgutils.h"

Scene::~Scene() = default;

Scene::Scene(std::unique_ptr<GLContext>& ctx, std::unique_ptr<Camera>& cam) {
    if (cam == nullptr) {
        camera = std::move(cam);
    } else {
        camera = std::make_unique<Camera>(
                Camera::Vec3 {0, 0, 3},
                Camera::Vec3 {0, 0, 0},
                Camera::Vec3 {0, 1, 0}
                );
    }

    if (ctx == nullptr) {
        context = std::move(ctx);
    } else {
        context = std::make_unique<GLContext>();
    }

    Logger::message("camera initialize with %p", camera.get());
}

void Scene::drawFrame() {
    updateViewMatrix();
    updateModelMatrix();
    doDrawFrame();
}

void Scene::updateViewMatrix() {
    auto view = camera->getViewMatrix();
    for (auto&& obj : models) {
        obj->setViewMatrix(view);
    }
}

void Scene::updateModelMatrix() {
    // do in obj render draw
}

void Scene::doDrawFrame() {
    for (auto&& obj : models) {
        obj->draw();
    }
}

void Scene::initGLContext() {
    context->init();
    Logger::message("GL context inited");
}
