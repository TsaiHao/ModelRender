#include "obj_render.h"
#include "scene.h"
#include "camera.h"
#include "cgutils.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "render_helper.h"
#include "base_render.h"
#include "user_interface.h"

using namespace std;

class Scene::GLNativeWindow {
public:
    WindowType window = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            float(GL_WINDOW_WIDTH) / float(GL_WINDOW_HEIGHT),
                                        0.1f, 100.0f);;
};

void Scene::setCameraMatrix(const std::array<float, 3> &position, const std::array<float, 3> &target,
                            const std::array<float, 3> &up) const {
    cam->setCameraVector(Camera::Vec3(position[0], position[1], position[2]),
                         Camera::Vec3(target[0], target[1], target[2]),
                         Camera::Vec3(up[0], up[1], up[2]));
}

Scene::Scene(): cam(std::make_shared<Camera>(
        Camera::Vec3({3.f, 0.f, -3.f}),
        Camera::Vec3({0.f, 0.f, 0.f}),
        Camera::Vec3({0.f, 1.f, 0.f}))),
                nativeWindow(nullptr) {
    init();
}

#ifdef USE_GLFW
static void glfwErrorCallback(int error, const char* description)
{
    Logger::error(description);
}
#endif

void Scene::init() {
    nativeWindow = make_unique<GLNativeWindow>();

#ifdef USE_GLFW
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    nativeWindow->window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, "Model Render", nullptr, nullptr);
    nativeWindow->windowHeight = GL_WINDOW_HEIGHT;
    nativeWindow->windowWidth = GL_WINDOW_WIDTH;
    if (!nativeWindow->window) {
        Logger::error("create window failed");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetFramebufferSizeCallback(nativeWindow->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    ui = make_unique<GLFWUserInterface>(cam, nativeWindow->window);
    glfwSetWindowUserPointer(nativeWindow->window, (void*)ui.get());

    glfwMakeContextCurrent(nativeWindow->window);
#endif

#ifndef __ANDROID__
    gladLoadGL();
#endif
    glEnable(GL_DEPTH_TEST);

    lightSource = static_pointer_cast<BaseRenderImpl>(createModel(GeometryType::Circle));
    lightSource->setVisibility(false);

    Logger::message("scene %p inited successfully", (void*)this);
}

void Scene::renderAModel(const std::shared_ptr<BaseRenderImpl>& model, const float* lightPos) {
    if (model == nullptr || !model->getVisibility()) {
        return;
    }

    model->updateViewMatrix(VPTR(cam->getCameraViewMatrix()));
    model->updateProjectionMatrix(VPTR(nativeWindow->projection));

    model->shader->setFloatVec4(LIGHT_SOURCE_POSITION, lightPos);
    model->shader->setFloatVec4(CAMERA_POSITION, VPTR(cam->getCameraPosition()));

    model->draw();
}

void Scene::renderAFrame() {


    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    auto const &view = cam->getCameraViewMatrix();
    glm::vec4 lightPos;
    if (lightSource) {
        lightPos = RenderHelper::getModelSpacePosition(*lightSource);
    }

    for (auto&& model : models) {
        renderAModel(model, VPTR(lightPos));
    }

    renderAModel(lightSource, VPTR(lightPos));
    glfwSwapBuffers(nativeWindow->window);
    glfwPollEvents();
}

void Scene::addModel(std::shared_ptr<BaseRender> model) {
    model->initRender();
    models.emplace_back(std::static_pointer_cast<BaseRenderImpl>(model));
}

void Scene::draw() {
    while (!glfwWindowShouldClose(nativeWindow->window)) {
        renderAFrame();
    }

    // for debug use
    auto* buffer = new unsigned char[GL_WINDOW_HEIGHT * GL_WINDOW_WIDTH * 4];
    glReadPixels(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    fstream fs("./screen.bmp", ios::binary | ios::out);
    fs.write(reinterpret_cast<char*>(buffer), GL_WINDOW_WIDTH * GL_WINDOW_HEIGHT * 4);
    fs.close();
    delete[] buffer;
}

Scene::~Scene() {
    glfwTerminate();
}

void Scene::addLightSource(const shared_ptr<BaseRender> &lightModel) {
    lightSource = static_pointer_cast<BaseRenderImpl>(lightModel);
    lightSource->initRender();
}
