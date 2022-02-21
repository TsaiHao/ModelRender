#include "scene.h"
#include "camera.h"
#include "cgutils.h"
#include "obj_render.h"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

class Scene::GLNativeWindow {
public:
    WindowType window = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;
};

void Scene::setCameraMatrix(const std::array<float, 3> &position, const std::array<float, 3> &target,
                            const std::array<float, 3> &up) const {
    cam->setCameraVector(position, target, up);
}

Scene::Scene(): cam(std::make_unique<Camera>(
        Camera::Vec3({3.f, 0.f, 3.f}),
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
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);

    auto const *view = cam->getCameraViewMatrix();
    auto const* projection = glm::value_ptr(glm::mat4(1.0f));
    for (auto&& model : models) {
        model->updateViewMatrix(view);
        model->updateProjectionMatrix(projection);
        model->draw();
    }

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
    auto* buffer = new unsigned char[GL_WINDOW_HEIGHT * GL_WINDOW_WIDTH * 3];
    glReadPixels(0, 0, GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    fstream fs("./screen.bmp", ios::binary | ios::out);
    fs.write(reinterpret_cast<char*>(buffer), GL_WINDOW_WIDTH * GL_WINDOW_HEIGHT * 3);
    fs.close();
}

Scene::~Scene() {
    glfwTerminate();
}
