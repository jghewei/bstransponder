#ifndef _C_THREAD_
#define _C_THREAD_

#include <pthread.h>

typedef void* (*CThreadProc)(void *arg);

class CThread
{
public:
    CThread();
    CThread(const char *name, CThreadProc fun, void *arg);
    ~CThread();

    static void* loop(void *arg);
    void create(const char *name, CThreadProc fun, void *arg);
    void join();

    const char* name() { return name_; }
    bool running() { return running_; }
    void* arg() { return arg_; }
    CThreadProc fun() { return fun_; }

private:
    char name_[8];
    pthread_t tid_;
    bool running_;
    void *arg_;
    CThreadProc fun_;
};

#endif
