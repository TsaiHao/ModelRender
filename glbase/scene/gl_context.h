#ifndef GRAPHICS_GLCONTEXT_H
#define GRAPHICS_GLCONTEXT_H

#include <memory>
#include <deque>
#include <thread>
#include <mutex>

#include "scene.h"

class GLThread;
class GLContext {
public:
    class GLNativeContext;
    class ContextState;
    enum class GLState: int {
        Prepare = 0,
        Drawing,
        Stopped,
    };

    GLContext();
    ~GLContext();

    void init();
    void makeCurrent() const;

    void swapBuffer() const;
    void clearGLBuffer() const;

    void setMaxCommandCount(int n);
    GLTaskCommand pickCommand();
    bool pushCommand(GLTaskCommand cmd);

    GLState getState() const;
    void setState(GLState s);

private:
    std::unique_ptr<GLThread> glThread;
    std::unique_ptr<GLNativeContext> glNative;
    std::unique_ptr<ContextState> state;

    std::deque<GLTaskCommand> queue;
    int maxCommandCount = 20;
    std::mutex queueMutex;
};

#endif //GRAPHICS_GLCONTEXT_H
