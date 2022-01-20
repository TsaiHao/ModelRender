#include "scene.h"
#include "camera.h"
#include <gl_context.h>

#include "obj_render.h"
#include "cgutils.h"

Scene::~Scene() = default;

Scene::Scene(std::unique_ptr<Camera>& cam) {
    if (cam == nullptr) {
        camera = std::make_unique<Camera>(
                Camera::Vec3 {0, 0, 3},
                Camera::Vec3 {0, 0, 0},
                Camera::Vec3 {0, 1, 0}
        );
    } else {
        camera = std::move(cam);
    }

    Logger::message("camera initialize with %p", camera.get());
}

void Scene::drawFrame() {
    if (context->getState() != GLContext::GLState::Drawing) {
        return;
    }
    context->clearGLBuffer();

    updateViewMatrix();
    updateModelMatrix();
    doDrawFrame();

    context->swapBuffer();
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

void Scene::initRenders() {
    for (auto&& obj : models) {
        obj->bufferData();
    }
}

const std::unique_ptr<GLContext>& Scene::getGLContext() const {
    return context;
}

void Scene::addModels(std::shared_ptr<ObjRender> &model) {
    models.emplace_back(std::move(model));
}

void Scene::init() {
    context->pushCommand(GLTaskCommand::Init);
    Logger::message("scene init %p", (void*)this);
}

void Scene::draw() {
    context->pushCommand(GLTaskCommand::Draw);
}
