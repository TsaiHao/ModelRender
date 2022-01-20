#ifndef GRAPHICS_GL_THREAD_H
#define GRAPHICS_GL_THREAD_H

class GLThread {
public:
    GLThread();

    ~GLThread();

    void init();

    void sleep(int64_t uSec) const;

    void consumerWait(/*int timeout*/);

    void consumerAwake();

    void producerWait();

    void producerAwake();

private:
    pthread_t  thread;
    pthread_mutex_t mutex;
    pthread_cond_t producerCondition, consumerCondition;
    pthread_mutex_t producerMutex, consumerMutex;
};



#endif //GRAPHICS_GL_THREAD_H
