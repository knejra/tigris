#ifndef _CONDITION_VARIABLE_H
#define _CONDITION_VARIABLE_H

#include <pthread.h>

template<typename T>
class ConditionVariable
{
    public:
    T value;
    pthread_mutex_t mutex;
    pthread_cond_t cv;
};

#endif // _CONDITION_VARIABLE_H