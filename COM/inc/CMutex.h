#ifndef _CMUTEX_H_
#define _CMUTEX_H_

#include <pthread.h>

class CMutex
{
public:
    CMutex()
    {
        pthread_mutex_init(&mtx, NULL);
    }

    ~CMutex()
    {
        pthread_mutex_destroy(&mtx);
    }

    inline void Lock()
    {
        pthread_mutex_lock(&mtx);
    }

    inline void UnLock()
    {
        pthread_mutex_unlock(&mtx);
    }

private:
    pthread_mutex_t mtx;
};

class CMutexLock
{
public:
    CMutexLock(CMutex &mutex) : mtx(mutex), isLocked(false)
    {
        mtx.Lock();
        isLocked = true;
    }

    ~CMutexLock()
    {
        if (isLocked)
            mtx.UnLock();
        isLocked = false;
    }

    inline void Lock()
    {
        if (!isLocked)
            mtx.Lock();
        isLocked = true;
    }

    inline void UnLock()
    {
        if (isLocked)
            mtx.UnLock();
        isLocked = false;
    }

private:
    CMutex &mtx;
    bool isLocked;
};

#endif // _CMUTEX_H_
