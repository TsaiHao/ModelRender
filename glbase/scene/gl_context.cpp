#include <pthread.h>
#include "gl_context.h"
#include "cgutils.h"
#include "scene.h"

using namespace std;

static void* glTask(void* asset) {
    auto scene = reinterpret_cast<Scene*>(asset);

    return nullptr;
}

class GLContext::GLThread {
public:
    GLThread() = default;

    void init() {
        pthread_create(&thread, nullptr, glTask, nullptr);
        pthread_mutex_init(&mutex, nullptr);
        Logger::message("gl thread create %ld", thread);
    }

    pthread_t  thread;
    pthread_mutex_t mutex;
};


class GLContext::GLNativeContext {
public:
    GLNativeContext() = default;

    void initWindow()
    {
#ifdef USE_GLFW
        glfwSetErrorCallback([](int error, const char* desc) {
            Logger::error("glfw error, code %d, desc %s", error, desc);
        });

        if (!glfwInit())
            exit(EXIT_FAILURE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(GL_WINDOW_WIDTH, GL_WINDOW_HEIGHT, "Model Render", NULL, NULL);
        if (!window) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        });
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
        });

        glfwMakeContextCurrent(window);
#endif

#ifndef __ANDROID__
        gladLoadGL();
#endif
        glEnable(GL_DEPTH_TEST);
    }

    void swapBuffer() const {
#ifdef USE_GLFW
        glfwSwapBuffers(window);
        glfwPollEvents();
#else
#endif
    }

    WindowType window = nullptr;
};

GLContext::GLContext(): glThread(make_unique<GLThread>()),
                        glNative(make_unique<GLNativeContext>()),
                        state(GLState::Prepare) {

}

void GLContext::init() {
    glNative->initWindow();
}

GLContext::~GLContext() = default;

