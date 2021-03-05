#ifndef _PARAMETER_SERVERS_H
#define _PARAMETER_SERVERS_H

#include <unordered_map>
#include <map>

#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>

#include "util/Io.h"
#include "util/KeyValueEngine.h"
#include "util/ThreadSafeQueue.h"
#include "util/ConditionVariable.h"
#include "util/ConsistentHashing.h"

#include "MessageSocket.h"

#define PS_WORK 1
#define PS_HALT 0

enum
{
    SERVER,
    WORKER,
    W_SCHEDULER,
    S_MANAGER
}Role;

class ParameterServer
{
    public:
    // attention: differ from StateMachine
    int mMachineId;
    int mNumServer;

    // server, worker, server manager, task scheduler
    int mRole;
    // current epoch
    int mEpoch;

    std::map<int, std::vector<int>> mGroupMap;

    // test
    std::vector<int> mTstKeys;
    std::vector<double> mTstVals;

    // data
    IO mIo;

    // kvStore
    KVEngine<double> mKvEng;
    ConsistentHashing mConsHash;
    // key: keys of kv pairs, value: machine which this key belong to 
    std::unordered_map<uint64_t, int> mkeyCache;

    // distinguish from sockets in StateMachine
    MessageSocket mMailbox;
    ThreadSafeQueue<MessageBuffer> msgQueue;

    // <#request, #response>, index: timestamp
    std::vector<std::pair<int, int>> mTracker;
    ConditionVariable<int> cvTrack;
    ConditionVariable<int> mState;
    ConditionVariable<bool> cvEpoch;
    int mRecvTs;

    pthread_t receiverThread;
    pthread_t dispatchThread;
    pthread_t scheduleThread;
    pthread_t workerThread;

    public:
    // init: initialize parameter servers
    // parameters: id-id of this machine
    //             role-role of this machine
    // outputs   : void
    void init(int id, int role);

    // partition: partition data to different machines
    // Plan: should provide a HDFS i/o interface as io.h 
    void partition(std::vector<int> group, std::vector<int> keys);

    // wait: wait request(at timestamp)
    // parameters: timestamp-timestamp of request
    // outputs   : void
    void wait(int timestamp);

    // newRequest: add new request which send to group
    // parameters: group-dest group
    // outputs   : return timestamp
    int newRequest(std::vector<int> group);

    // push: push parameters to the group, in parameter server, it means servers
    // parameters: keys-key of key-value pairs
    //             value-value of key-value pairs
    //             group-dest group to send
    //             timestamp-timestamp of this bunch of kv pairs
    //             callback-callback function after push success
    // outputs   : void
    void push(std::vector<int> keys, std::vector<double> values, 
              std::vector<int> group, void (*callback)(void *args));
    
    // pull: pull parameters from group, the value will be stored in param values
    // parameters: keys-keys of kv pairs
    //             timestamp-timestamp of this bunch of kv pairs
    //             callback-callback function after pull success
    // outpus: void
    void pull(std::vector<int> keys, std::vector<int> group, void (*callback)(void *args));

    // response: send respone after finish push/pull request
    // parameters: res-response message buffer 
    //             from-the source of this request
    // outpus    : void
    void response(MessageBuffer res, int from);

    // doResponse: handle response message
    // parameters: mbuf-message buffer of response
    // outputs   : void
    void doResponse(MessageBuffer mbuf);

    // doControl: handle control message
    // parameters: mbuf-message buffer of control
    // outputs   : void
    void doControl(MessageBuffer mbuf);

    // defaultPushCallback: default callback function of PUSH
    // parameters: mbuf-message buffer of push message
    // outputs   : message buffer of response
    MessageBuffer defaultPushCallback(MessageBuffer mbuf);

    // defaultPullCallback: default callback function of PULL
    // parameters: mbuf-message buffer of pull message
    // outputs   : message buffer of response
    MessageBuffer defaultPullCallback(MessageBuffer mbuf);

    // mlModel: naive machine learning model, for test
    // parameters: keys-keys
    //              vals-values
    // outputs   : void 
    void mlModel(std::vector<int> keys, std::vector<double> vals);

    // destroy: destroy parameter server instance
    // parameters: void
    // outputs   : void
    void destroy();
};

#endif // _PARAMETER_SERVERS_H