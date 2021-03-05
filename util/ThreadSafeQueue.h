#ifndef _THREAD_SAFE_QUEUE_H
#define _THREAD_SAFE_QUEUE_H

#include <pthread.h>

#include <memory>
#include <queue>

#include "Util.h"

template<typename T>
class ThreadSafeQueue
{
    public:
    pthread_mutex_t mtx;
    pthread_cond_t cv;
    std::queue<T> threadSafeQueue;

    public:
    ThreadSafeQueue()
    {
        pthread_mutex_init(&mtx, NULL);
        pthread_cond_init(&cv, NULL);
    }
    ~ThreadSafeQueue()
    {
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&cv);
    }

    void push(T value)
    {
        int r = pthread_mutex_lock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::push pthread_mutex_lock");
        }

        threadSafeQueue.push(value);

        r = pthread_mutex_unlock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::push pthread_mutex_unlock");
        }

        r = pthread_cond_signal(&cv);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::push pthread_cond_signal");
        }
    }

    void pop(T &value)
    {
        int r = pthread_mutex_lock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::pop pthread_mutex_lock");
        }

        while(threadSafeQueue.empty())
        {
            pthread_cond_wait(&cv, &mtx);
        }

        value = threadSafeQueue.front();
        threadSafeQueue.pop();

        r = pthread_mutex_unlock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::pop pthread_mutex_unlock");
        }
    }

    std::shared_ptr<T> pop()
    {
        int r = pthread_mutex_lock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::pop thread_mutex_lock");
        }

        while(threadSafeQueue.empty())
        {
            pthread_cond_wait(&cv, &mtx);
        }

        std::shared_ptr<T> ret(std::make_shared<T>(threadSafeQueue.front()));
        threadSafeQueue.pop();

        r = pthread_mutex_unlock(&mtx);
        if(r != 0)
        {
            Util::printErrorAndExit("[Error] ThreadSafeQueue::pop pthread_mutex_unlock");
        }

        return ret;
    }
};

#endif // _THREAD_SAFE_QUEUE_H