#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <CThread.h>

CThread::CThread() : tid_(-1), running_(false)
{
    memset(name_, 0, sizeof(name_));
}

CThread::CThread(const char *name, CThreadProc fun, void *arg)
{
    create(name, fun, arg);
}

CThread::~CThread()
{
    join();
}

void* CThread::loop(void *arg)
{
    CThread *thread = (CThread*)arg;
    prctl(PR_SET_NAME, thread->name());
    printf("CThread::loop begin %s\n", thread->name());
    while (thread->running())
    {
        (thread->fun())(thread->arg());
    }
    printf("CThread::loop end %s\n", thread->name());
    return NULL;
}

void CThread::create(const char *name, CThreadProc fun, void *arg)
{
    strncpy(name_, name, 7);
    fun_ = fun;
    arg_ = arg;
    running_ = true;
    if (pthread_create(&tid_, NULL, loop, this) == -1)
    {
        running_ = false;
    }
}

void CThread::join()
{
    void *status;
    if (running_)
    {
        running_ = false;
        printf("CThread::join begin %s\n", name_);
        pthread_join(tid_, &status);
        printf("CThread::join end %s\n", name_);
        tid_ = -1;
    }
}

