#ifndef _KEYVALUE_ENGINE_H
#define _KEYVALUE_ENGINE_H

#include <unordered_map>
#include <vector>

#include "Types.h"
#include "Util.h"

template<typename value>
class KVEngine
{
    public:
    std::unordered_map<int, value> kvStore;
    void (*callback)(void *arg);

    public:

    // setCallback: set callback function
    // parameters: func-callback function
    // outputs   : void
    void setCallback(void (*func)(void *arg))
    {
        callback = func;
    }

    // defualtPushCallback: default callback of push
    // parameters: key-key
    //             val-value
    // outputs   : void 
    void defaultPushCallback(int key, value val);

    // defaultPullCallback: default callback of pull
    // parameters: key-key
    //             val-value
    // outputs   : void
    void defaultPullCallback(int key, value &val);
};

#endif // _KEYVALUE_ENGINE_H