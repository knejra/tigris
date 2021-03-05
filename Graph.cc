#include "Graph.h"

#include "util/Util.h"

extern Graph<double, double, GraphMessage> graph;

void *receiverThreadMain_g(void *args)
{
    graph.mMailbox.initServer();

    pthread_mutex_lock(&(graph.mState.mutex));
    while(graph.mState.value != G_WORK)
    {
        pthread_cond_wait(&(graph.mState.cv), &(graph.mState.mutex));
    }
    pthread_mutex_unlock(&(graph.mState.mutex));

    while(graph.mState.value == G_WORK)
    {
        int ready = graph.mMailbox.mListenEvents.waitEvent();
        for(int i = 0; i < ready; i++)
        {
            if(graph.mMailbox.mListenEvents.mEvents[i].events & EPOLLIN)
            {
                MessageBuffer mbuf = graph.mMailbox.recvMsg(graph.mMailbox.mListenEvents.mEvents[i].data.fd);
                graph.mMsgQue.push(mbuf);
            }
        }
    }
}

void *dispatchThreadMain_g(void *args)
{
    graph.mMailbox.initClient();

    pthread_mutex_lock(&(graph.mState.mutex));
    graph.mState.value = G_WORK;
    pthread_cond_broadcast(&(graph.mState.cv));
    pthread_mutex_unlock(&(graph.mState.mutex));

    while(graph.mState.value == G_WORK)
    {
        MessageBuffer mbuf;
        graph.mMsgQue.pop(mbuf);
        switch(mbuf.msgType)
        {
            case MSG_GRAPH:
            {
                printf("[dispatch] receive graph UPDATE message.\n");
                GraphMessage gm;
                gm.ParseFromArray(mbuf.message, mbuf.msglen);
                graph.mVertexMap[gm.targetid()].inMsgQue.push(gm);
                break;
            }
            case MSG_GRAPH_CTL:
            {
                printf("[dispatch] receive CONTROL message.\n");
                graph.doContorl(mbuf);
                break;
            }
            case MSG_GRAPH_RESP:
            {
                printf("[dispatch] receive RESPONSE message.\n");
                graph.doResponse(mbuf);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void *scheduleThreadMain_g(void *args)
{
    pthread_mutex_lock(&(graph.mState.mutex));
    while(graph.mState.value != G_WORK)
    {
        pthread_cond_wait(&(graph.mState.cv), &(graph.mState.mutex));
    }
    pthread_mutex_unlock(&(graph.mState.mutex));

    if(graph.mRole == MASTER)
    {
        // a little different from PS
        int ts = graph.newRequest(graph.mNumServer - 1);
        printf("[master] send LOAD message.\n");
        MessageBuffer mbuf = graph.mMailbox.packGContorl(CTL_LOAD, ts);
        graph.mMailbox.sendAll(mbuf);
        graph.wait(ts);

        while(graph.mState.value == G_WORK)
        {
            graph.mEpoch++;
            ts = graph.newRequest(graph.mNumServer - 1);
            printf("[master] send EPOCH message: %d\n", graph.mEpoch);
            mbuf = graph.mMailbox.packGControl(CTL_EPOCH, ts);
            graph.mMailbox.sendAll(mbuf);
            graph.wait(ts);
        }
    }
}

void *workerThreadMain_g(void *args)
{
    pthread_mutex_lock(&(graph.mState.mutex));
    while(graph.mState.value != G_WORK)
    {
        pthread_cond_wait(&(graph.mState.cv), &(graph.mState.mutex));
    }
    pthread_mutex_unlock(&(graph.mState.mutex));

    while(graph.mState.value == G_WORK)
    {
        pthread_mutex_lock(&(graph.mCvEpoch.mutex));
        while(!graph.mCvEpoch.value)
        {
            pthread_cond_wait(&(graph.mCvEpoch.cv), &(graph.mCvEpoch.mutex));
        }
        pthread_mutex_unlock(&(graph.mCvEpoch.mutex));

        // vertex iteration, only calculate messages match mRecvTs
        std::map<uint64_t, Vertex<double, GraphMessage>>::iterator vIter;
        for(vIter = graph.mVertexMap.begin(); vIter != graph.mVertexMap.end(); vIter++)
        {
            vIter->second.compute();
        }

        MessageBuffer res = graph.mMailbox.packGResponse(true, graph.mAggrgator, graph.mRecvTs);
        graph.response(res, graph.mMasterId);
        graph.mCvEpoch.value = false;
    }
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::init(int id, int role)
{
    mMachineId = id;
    mEpoch = -1;
    mRole = role;

    mMailbox.mMachineId = mMachineId;
    mMailbox.init("5000");
    mNumServer = mMailbox.mAddrTable.size();

    mState.value = G_HALT;
    pthread_mutex_init(&(mCvTrack.mutex), NULL);
    pthread_cond_init(&(mCvTrack.cv), NULL);

    mCvEpoch.value = false;
    pthread_mutex_init(&(mCvEpoch.mutex), NULL);
    pthread_cond_init(&(mCvEpoch.cv), NULL);

    printf("[Graph] start receiver thread.\n");
    if(pthread_create(&receiverThread, NULL, receiverThreadMain_g, NULL))
    {
        Util::printErrorAndExit("Graph::pthread_create, receiver thread");
    }

    printf("[Graph] start dispatch thread.\n");
    if(pthread_create(&dispatchThread, NULL, dispatchThreadMain_g, NULL))
    {
        Util::printErrorAndExit("Graph::pthread_create, dispatch thread");
    }

    if(mRole == MASTER)
    {
        printf("[Graph] start schedule thread.\n");
        if(pthread_create(&scheduleThread, NULL, scheduleThreadMain_g, NULL))
        {
            Util::printErrorAndExit("Graph::pthread_create, schedule thread");
        }
    }

    if(mRole == SLAVE)
    {
        printf("[Graph] start worker thread.\n");
        if(pthread_create(&workerThread, NULL, workerThreadMain_g, NULL))
        {
            Util::printErrorAndExit("Graph::pthread_create, worker thread");
        }
    }
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::destroy()
{
    pthread_join(receiverThread, NULL);
    pthread_join(dispatchThread, NULL);

    if(mRole == MASTER)
    {
        pthread_join(scheduleThread, NULL);
    }

    if(mRole == SLAVE)
    {
        pthread_join(workerThread, NULL);
    }

    pthread_mutex_destroy(&(mState.mutex));
    pthread_cond_destroy(&(mState.cv));

    pthread_mutex_destroy(&(mCvTrack.mutex));
    pthread_cond_destroy(&(mCvTrack.cv));

    pthread_mutex_destroy(&(mCvEpoch.mutex));
    pthread_cond_destroy(&(mCvEpoch.cv));

    mMailbox.destroy();
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::defaultGraphLoad(std::string path)
{
    // 从数据文件中只读取hash到自己的部分
    // 保存一个vertexId到机器的映射, mConsHash.kmHash
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::wait(int timestamp)
{
    pthread_mutex_lock(&(mCvTrack.mutex));
    while(mTracker[timestamp].first != mTracker[timestamp].second)
    {
        pthread_cond_wait(&(mCvTrack.cv), &(mCvTrack.mutex));
    }
    pthread_mutex_unlock(&(mCvTrack.mutex));
}

template<typename V, typename E, typename M>
int Graph<V, E, M>::newRequest(int gsize)
{
    pthread_mutex_lock(&(mCvTrack.mutex));
    mTracker.push_back(std::make_pair(gsize, 0));
    pthread_mutex_unlock(&(mCvTrack.mutex));
    return mTracker.size() - 1;
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::doContorl(MessageBuffer mbuf)
{
    GContorl gc;
    gc.ParseFromArray(mbuf.message, mbuf.msglen);
    if(gc.type() == CTL_LOAD)
    {
        // graph load
    }
    else if(gc.type() == CTL_EPOCH)
    {
        pthread_mutex_lock(&(mCvEpoch.mutex));
        mRecvTs = gc.timestamp();
        mCvEpoch.value = true;
        pthread_cond_signal(&(mCvEpoch.cv));
        pthread_mutex_unlock(&(mCvEpoch.mutex));
    }
    else
    {
        printf("[dispatch] unexpected control type.\n");
    } 
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::response(MessageBuffer res, int from)
{
    mMailbox.sendMsg(mMailbox.mCliSockFd[from], res);
}

template<typename V, typename E, typename M>
void Graph<V, E, M>::doResponse(MessageBuffer mbuf)
{
    GraphResp res;
    res.ParseFromArray(mbuf.message, mbuf.msglen);

    if(!res.resp())
    {
        // contains aggregator, update
    }

    pthread_mutex_lock(&(mCvTrack.mutex));
    mTracker[res.timestamp()].second++;
    if(mTracker[res.timestamp()].first == mTracker[res.timestamp()].second)
    {
        pthread_cond_broadcast(&(mCvTrack.cv));
    }
    pthread_mutex_unlock(&(mCvTrack.mutex));
}