#ifndef GRAPHICS_GLCONTEXT_H
#define GRAPHICS_GLCONTEXT_H

#include <memory>
#include <deque>

enum class GLTaskCommand {
    Init,
    Draw,
    Pause,
    Quit,
};

class GLContext {
public:
    class GLThread;
    class GLNativeContext;
    enum class GLState {
        Prepare,
        Drawing,
        Stopped,
    };

    GLContext();
    ~GLContext();

    void init();

private:
    std::unique_ptr<GLThread> glThread;
    std::unique_ptr<GLNativeContext> glNative;
    std::deque<GLTaskCommand> queue;
    GLState state;
};

#endif //GRAPHICS_GLCONTEXT_H
