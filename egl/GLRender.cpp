//
// Created by bytedance on 11/30/21.
//

#include "GLRender.h"
#include "android/native_window.h"
//#include "GLES3/gl3.h"
#include <strings.h>
#include <android/log.h>
#include <string>
#include <thread>

#include "glbase/glbase.h"
using namespace std;

static void* task(void* args) {
    auto* render = reinterpret_cast<GLRender*>(args);
    while (true) {
        switch (render->status) {
            case GLRender::Status::Prepare:
                render->initGL();
                break;
            case GLRender::Status::Drawing:
                render->renderFrame();
                break;
            case GLRender::Status::Stopping:
                Logger::error("Render stopped!");
                pthread_exit(nullptr);
            default:
                break;
        }
    }
}

GLRender::GLRender() :render(nullptr) {
    pthread_mutex_init(&mutex, nullptr);

    pthread_create(&threadId, nullptr, task, (void*)this);
}

GLRender::~GLRender() {
    pthread_mutex_destroy(&mutex);
    pthread_join(threadId, nullptr);
}

std::shared_ptr<GLRender> GLRender::Instance() {
    static auto render = shared_ptr<GLRender>(new GLRender());
    return render;
}

void GLRender::setStatus(GLRender::Status s) {
    status = s;
}

void GLRender::renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    render->draw();
    eglSwapBuffers(display, surface);
}

void GLRender::initGL() {
    pthread_mutex_lock(&mutex);
    if (status != Status::Prepare) {
        pthread_mutex_unlock(&mutex);
        Logger::warn("try to init gl while status is %d", (int)status);
        return;
    }

    if (initEGL() != 0) {
        pthread_mutex_unlock(&mutex);
        return;
    }
    glWindowInit();
    render = make_shared<ObjRender>("/sdcard/resource/model/cube.obj.mp4", "/sdcard/resource/shader/plain.vs.mp4", "/sdcard/resource/shader/plain.fs.mp4");
    render->shader->attachTexture("texture1", Texture("/sdcard/resource/texture/wall.jpg.mp4"));

    auto trans = AnimatorActor::getActor(AnimationType::Translate, "trans");
    trans->setOrigin({0, 0.2f, 0});
    trans->setSpeed(0);

    auto scale = AnimatorActor::getActor(AnimationType::Scale, "scale");
    scale->setOrigin({ 0.25f, 0.25f, 0.25f });
    scale->setSpeed(0);

    auto rotate = AnimatorActor::getActor(AnimationType::Rotate, "rotate");
    rotate->setOrigin({0.5f, 0.7f, 0.9f});
    rotate->setSpeed(1.0f);

    render->animator.addDynamicActor(trans);
    render->animator.addDynamicActor(rotate);
    render->animator.addDynamicActor(scale);

    status = Status::Drawing;
    Logger::message("gl init done, thread %p", (void*)(&threadId));
    pthread_mutex_unlock(&mutex);
}

void GLRender::setWindow(ANativeWindow *w) {
    window = w;
    width = ANativeWindow_getWidth(window);
    height = ANativeWindow_getHeight(window);
    Logger::message("set window: %p, width %d, height %d", (void*)w, width, height);

    status = Status::Prepare;
}

int GLRender::initEGL() {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        Logger::error("No display");
        return -1;
    }
    EGLint majorVersion, minorVersion;
    if (!eglInitialize(display, &majorVersion, &minorVersion)) {
        Logger::error("egl init failed");
        return -1;
    } else {
        Logger::message("egl init success, version: %d.%d", majorVersion, minorVersion);
    }

    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };
    EGLint maxRet = 10, nConfig = 0;
    EGLConfig config;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &nConfig)) {
        Logger::error("failed to get config");
        return -1;
    } else {
        Logger::message("egl get %d configs", nConfig);
    }

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        int error = eglGetError();
        Logger::error("egl create context failed %d", error);
        return -1;
    } else {
        Logger::message("egl create context: %p", (void *) context);
    }

    surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        int error = eglGetError();
        Logger::error("create surface failed, ret %x", error);
        return -1;
    } else {
        Logger::message("create surface: %p", (void *) surface);
    }

    if (!eglMakeCurrent(display, surface, surface, context)) {
        int error = eglGetError();
        Logger::error("egl make current failed, %x", error);
        return -1;
    }
    EGL_BAD_ACCESS;

    return 0;
}
