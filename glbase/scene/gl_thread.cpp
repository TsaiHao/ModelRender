#include "cgutils.h"
#include "gl_context.h"
#include "gl_thread.h"


static void* glTask(void* asset) {
    auto scene = reinterpret_cast<Scene*>(asset);
    auto& context = scene->getGLContext();
    while (context) {
        auto cmd = context->pickCommand();
        switch (cmd) {
            case GLTaskCommand::None:       // queue empty

                break;
            case GLTaskCommand::Init:
                scene->initGLContext();
                scene->initRenders();
                break;
            case GLTaskCommand::Draw:
                scene->drawFrame();
                break;
            case GLTaskCommand::Pause:
                break;
            case GLTaskCommand::Quit:
                context->setState(GLContext::GLState::Stopped);
                return nullptr;
        }
    }
    return nullptr;
}


GLThread::GLThread() {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&producerCondition, nullptr);
    pthread_cond_init(&consumerCondition, nullptr);
    pthread_mutex_init(&producerMutex, nullptr);
    pthread_mutex_init(&consumerMutex, nullptr);
}

GLThread::~GLThread() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&producerCondition);
    pthread_cond_destroy(&consumerCondition);
    pthread_mutex_destroy(&producerMutex);
    pthread_mutex_destroy(&consumerMutex);
}

void GLThread::init() {
    pthread_create(&thread, nullptr, glTask, nullptr);
    Logger::message("gl thread create %ld", thread);
}

void GLThread::sleep(int64_t uSec) const {
    usleep(uSec);
}

void GLThread::consumerWait() {
    pthread_cond_wait(&consumerCondition, &consumerMutex);
}

void GLThread::consumerAwake() {
    pthread_cond_signal(&consumerCondition);
}

void GLThread::producerWait() {
    pthread_cond_wait(&producerCondition, &producerMutex);
}

void GLThread::producerAwake() {
    pthread_cond_signal(&producerCondition);
}
