#ifndef _GRAPH_H
#define _GRAPH_H

#include <vector>
#include <map>

#include <pthread.h>

#include "MessageSocket.h"

#include "util/Types.h"
#include "util/ConditionVariable.h"
#include "util/ThreadSafeQueue.h"
#include "util/ConsistentHashing.h"

#define G_WORK 1
#define G_HALT 0

enum
{
    MASTER,
    SLAVE
}Role;

template<typename Value>
class Edge
{
    public:
    uint64_t to;
    Value value;
};

template<typename Value, typename M>
class Vertex
{
    public:
    int vertexId;                 // vertex ID
    Value value;             
    std::vector<int> refToEdge;   // reference to outedges
    std::queue<M> inMsgQue;       // in-message queue, attention: not threadsage queue!
    void (*callback)(void *args); // user can set their own callback

    public:
    // getValue: get value of this vertex
    // parameters: void
    // outputs   : value of vertex
    Value getValue();

    // mutableValue: mutate value
    // parameters: v-value
    // outputs   : void
    void mutableValue(Value &v);

    // sendMsgTo: send message to dest vertex
    // parameters: dstV-dest vertext
    //             msg-message
    // outputs   : void
    void sendMsgTo(uint64_t dstV, M msg);

    // sendMsgToAllNeighbors: send message to all neighbor vertex
    // parameters: msg-message
    // outputs   : void
    void sendMsgToAllNeighbors(M msg);

    // voteToHalt: when finish work, vote to halt
    // parameters: void
    // outputs   : void
    void voteToHalt();

    // compute: compute on vertex, virtual
    virtual void compute();
};

template<typename V, typename E, typename M>
class Graph
{
    public:
    int mMachineId;
    int mNumServer;
    int mMasterId;

    int mEpoch;
    int mRole;
    int mRecvTs;

    double mAggrgator;

    std::vector<Edge<E>> mOutEdgeList;
    std::map<uint64_t, Vertex<V, M>> mVertexMap;
    ConsistentHashing mConsHash;


    MessageSocket mMailbox;
    ThreadSafeQueue<MessageBuffer> mMsgQue;

    std::vector<std::pair<int, int>> mTracker;
    ConditionVariable<int> mCvTrack;
    ConditionVariable<int> mState;
    ConditionVariable<bool> mCvEpoch;

    pthread_t receiverThread;
    pthread_t dispatchThread;
    pthread_t scheduleThread;
    pthread_t workerThread;

    public:

    // init: initialize graph processing
    // parameters: id-id of this machine
    // outputs   : void
    void init(int id, int role);

    // destroy: destroy graph
    // parameters: void
    // outputs   : void
    void destroy();

    // defaultGraphLoad: default graph load, for test
    // parameters: gpath-graph file path
    // outputs   : void
    void defaultGraphLoad(std::string gpath);

    // wait: wait on timestamp message
    // parameters: timestamp-timestamp
    // outputs   : void
    void wait(int timestamp);

    // newRequest: new request
    // parameters: gsize-size of group, graph only contains master and worker
    //                   do not need a group map like PS
    // outputs   : timestamp
    int newRequest(int gsize);

    // response: send response
    // parameters: res-response
    //             from-response to which sender
    // outputs   : void
    void response(MessageBuffer res, int from);

    // doResponse: handle response message
    // parameters: mbuf-message buffer
    // outputs   : void
    void doResponse(MessageBuffer mbuf);

    // doControl: handle control message
    // parameters: mbuf-message buffer
    // outputs   : void
    void doContorl(MessageBuffer mbuf);

    // test: pagerank

};

#endif // _GRAPH_H