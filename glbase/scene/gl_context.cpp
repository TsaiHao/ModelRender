#include <pthread.h>
#include "gl_context.h"
#include "cgutils.h"
#include "scene.h"
#include "gl_thread.h"

using namespace std;

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

    void clearGLBuffer() const {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.4f, 0.3f, 0.2f, 1.0f);
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

class GLContext::ContextState {
    pthread_rwlock_t lock;
    GLState state;
public:
    ContextState() {
        pthread_rwlock_init(&lock, nullptr);
    }

    GLState getState() {
        pthread_rwlock_rdlock(&lock);
        auto s = state;
        pthread_rwlock_unlock(&lock);
        return s;
    }

    void setState(GLState s) {
        pthread_rwlock_wrlock(&lock);
        state = s;
        pthread_rwlock_unlock(&lock);
    }
};

GLContext::GLContext(): glThread(make_unique<GLThread>()),
                        glNative(make_unique<GLNativeContext>()),
                        state(make_unique<ContextState>()) {

}

void GLContext::init() {
    glNative->initWindow();
    setState(GLState::Drawing);
}

void GLContext::makeCurrent() const {
#ifdef USE_GLFW
    glfwMakeContextCurrent(glNative->window);
#else
#endif
}

void GLContext::setMaxCommandCount(int n) {
    std::lock_guard lock(queueMutex);
    maxCommandCount = n;
}

GLTaskCommand GLContext::pickCommand() {
    std::lock_guard lock(queueMutex);
    if (queue.empty()) {
        glThread->producerAwake();
        glThread->consumerWait();
        return GLTaskCommand::None;
    }
    GLTaskCommand cmd = queue.front();
    queue.pop_front();
    return cmd;
}

bool GLContext::pushCommand(GLTaskCommand cmd) {
    std::lock_guard lock(queueMutex);
    if (queue.size() > maxCommandCount) {
        glThread->sleep(10000);
    }
    queue.push_back(cmd);
    glThread->consumerAwake();
    return true;
}

void GLContext::swapBuffer() const {
    glNative->swapBuffer();
}

void GLContext::clearGLBuffer() const {
    glNative->clearGLBuffer();
}

GLContext::GLState GLContext::getState() const {
    return state->getState();
}

void GLContext::setState(GLContext::GLState s) {
    state->setState(s);
}

GLContext::~GLContext() = default;

