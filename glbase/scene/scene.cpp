#include "scene.h"
#include "camera.h"
#include "cgutils.h"
#include "obj_render.h"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"

using namespace std;

class Scene::GLNativeWindow {
public:
    WindowType window = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;
};

void Scene::setCameraMatrix(const std::array<float, 3> &position, const std::array<float, 3> &target,
                            const std::array<float, 3> &up) const {
    cam->setCameraVector(Camera::Vec3(position[0], position[1], position[2]),
                         Camera::Vec3(target[0], target[1], target[2]),
                         Camera::Vec3(up[0], up[1], up[2]));
}

Scene::Scene(): cam(std::make_unique<Camera>(
        Camera::Vec3({3.f, 3.f, 3.f}),
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

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    glfwSetKeyCallback(nativeWindow->window, glfwKeyCallback);
    glfwSetFramebufferSizeCallback(nativeWindow->window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glfwMakeContextCurrent(nativeWindow->window);
#endif

#ifndef __ANDROID__
    gladLoadGL();
#endif
    glEnable(GL_DEPTH_TEST);

    Logger::message("scene %p inited successfully", (void*)this);
}

void Scene::renderAFrame() {
    static glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                   float(GL_WINDOW_WIDTH) / float(GL_WINDOW_HEIGHT),
                                                   0.1f, 100.0f);

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    auto const &view = cam->getCameraViewMatrix();

    for (auto&& model : models) {
        model->updateViewMatrix(VPTR(view));
        model->updateProjectionMatrix(VPTR(projection));
        auto lightPos =  lightSource->getCurrentPosition();
        model->shader->setFloatVec4("lightPos", VPTR(lightPos));
        model->draw();
    }

    lightSource->updateViewMatrix(VPTR(view));
    lightSource->updateProjectionMatrix(VPTR(projection));
    lightSource->draw();

    glfwSwapBuffers(nativeWindow->window);
    glfwPollEvents();
}

void Scene::addModel(std::shared_ptr<ObjRender> model) {
    models.emplace_back(std::move(model));
}

void Scene::addModel(const ObjRender& model) {
    auto modelCopy = std::make_shared<ObjRender>(model);
    models.emplace_back(modelCopy);
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

void Scene::addLightSource(const shared_ptr<ObjRender> &lightModel) {
    lightSource = lightModel;
}
