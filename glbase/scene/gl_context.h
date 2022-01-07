#ifndef GRAPHICS_GLCONTEXT_H
#define GRAPHICS_GLCONTEXT_H

#include <memory>
#include <deque>
#include <thread>
#include <mutex>

enum class GLTaskCommand {
    Init,
    Draw,
    Pause,
    Quit,
    None,
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
    void makeCurrent() const;

    void setMaxCommandCount(int n);
    GLTaskCommand pickCommand();
    bool pushCommand(GLTaskCommand cmd);

private:
    std::unique_ptr<GLThread> glThread;
    std::unique_ptr<GLNativeContext> glNative;
    std::deque<GLTaskCommand> queue;
    int maxCommandCount = 20;
    std::mutex queueMutex;
    GLState state;
};

#endif //GRAPHICS_GLCONTEXT_H
