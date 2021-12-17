//
// Created by bytedance on 11/30/21.
//

#ifndef NATIVEAPPLACATION_GLRENDER_H
#define NATIVEAPPLACATION_GLRENDER_H

#include <memory>

#include <pthread.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "glbase/glbase.h"

class GLRender {
public:
    static std::shared_ptr<GLRender> Instance();

    enum class Status {
        Prepare,
        Drawing,
        Pausing,
        Stopping,
        Ready
    };

    GLRender();
    ~GLRender();

    void setWindow(ANativeWindow* w);

    void setStatus(Status s);
    Status getStatus() const {
        return status;
    }

    void renderFrame();

    void initGL();
    int initEGL();

    std::shared_ptr<ObjRender> render;
    pthread_mutex_t mutex;
    pthread_t threadId;

    Status status;

    EGLint width;
    EGLint height;
    ANativeWindow *window;
    EGLSurface surface;
    EGLContext context;
    EGLDisplay display;

    bool glInited = false;
    struct {
        GLuint VAO;
        GLuint VBO;
        GLuint program;
    } model;
};


#endif //NATIVEAPPLACATION_GLRENDER_H
