#include <stdio.h>

#include "ParameterServer.h"

#include "util/ConsistentHashing.h"

#include "message/PSPushVec.pb.h"
#include "message/PSPullVec.pb.h"
#include "message/PSResponse.pb.h"
#include "message/PSControl.pb.h"

extern ParameterServer paramServer;

void *receiverThreadMain_p(void *args)
{
    paramServer.mMailbox.initServer();

    pthread_mutex_lock(&(paramServer.mState.mutex));
    while(paramServer.mState.value != PS_WORK)
    {
        pthread_cond_wait(&(paramServer.mState.cv), &(paramServer.mState.mutex));
    }
    pthread_mutex_unlock(&(paramServer.mState.mutex));

    while(paramServer.mState.value == PS_WORK)
    {
        int ready = paramServer.mMailbox.mListenEvents.waitEvent();
        for(int i = 0; i < ready; i++)
        {
            if(paramServer.mMailbox.mListenEvents.mEvents[i].events & EPOLLIN)
            {
                MessageBuffer mbuf = paramServer.mMailbox.recvMsg(paramServer.mMailbox.mListenEvents.mEvents[i].data.fd);
                paramServer.msgQueue.push(mbuf);
            }
        }
    }
}

void *dispatchThreadMain_p(void *args)
{
    paramServer.mMailbox.initClient();

    pthread_mutex_lock(&(paramServer.mState.mutex));
    paramServer.mState.value = PS_WORK;
    pthread_cond_broadcast(&(paramServer.mState.cv));
    pthread_mutex_unlock(&(paramServer.mState.mutex));

    while(paramServer.mState.value == PS_WORK)
    {
        MessageBuffer mbuf;
        paramServer.msgQueue.pop(mbuf);
        switch(mbuf.msgType)
        {
            case MSG_PUSH_VEC:
            {
                printf("[dispatch] receive PUSH message\n");
                MessageBuffer res = paramServer.defaultPushCallback(mbuf);
                paramServer.response(res, mbuf.from);
                break;
            }
            case MSG_PULL_VEC:
            {
                printf("[dispatch] receive PULL message\n");
                MessageBuffer res = paramServer.defaultPullCallback(mbuf);
                paramServer.response(res, mbuf.from);
                break;
            }
            case MSG_RESPONSE:
            {
                printf("[dispatch] receive RESPONSE message\n");
                paramServer.doResponse(mbuf);
                break;
            }
            case MSG_CONTROL:
            {
                printf("[dispatch] receive CONTROL message\n");
                paramServer.doControl(mbuf);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void *scheduleThreadMain_p(void *args)
{
    pthread_mutex_lock(&(paramServer.mState.mutex));
    while(paramServer.mState.value != PS_WORK)
    {
        pthread_cond_wait(&(paramServer.mState.cv), &(paramServer.mState.mutex));
    }
    pthread_mutex_unlock(&(paramServer.mState.mutex));

    if(paramServer.mRole == W_SCHEDULER)
    {
        int ts = paramServer.newRequest(paramServer.mGroupMap[WORKER]);
        printf("[scheduler] send LOAD message.\n");
        MessageBuffer mbuf = paramServer.mMailbox.packControl(CTL_LOAD, ts);
        paramServer.mMailbox.sendGroup(mbuf, paramServer.mGroupMap[WORKER]);
        paramServer.wait(ts);

        while(paramServer.mState.value == PS_WORK)
        {
            paramServer.mEpoch++;
            ts = paramServer.newRequest(paramServer.mGroupMap[WORKER]);
            printf("[scheduler] send EPOCH message: %d\n", paramServer.mEpoch);
            mbuf = paramServer.mMailbox.packControl(CTL_EPOCH, ts);
            paramServer.mMailbox.sendGroup(mbuf, paramServer.mGroupMap[WORKER]);
            paramServer.wait(ts);
            // test
            sleep(5);
        }
    }
    else
    {
        // send heartbeat
    }
}

void *workerThreadMain_p(void *args)
{
    pthread_mutex_lock(&(paramServer.mState.mutex));
    while(paramServer.mState.value != PS_WORK)
    {
        pthread_cond_wait(&(paramServer.mState.cv), &(paramServer.mState.mutex));
    }
    pthread_mutex_unlock(&(paramServer.mState.mutex));

    while(paramServer.mState.value == PS_WORK)
    {
        pthread_mutex_lock(&(paramServer.cvEpoch.mutex));
        while(!paramServer.cvEpoch.value)
        {
            pthread_cond_wait(&(paramServer.cvEpoch.cv), &(paramServer.cvEpoch.mutex));
        }
        pthread_mutex_unlock(&(paramServer.cvEpoch.mutex));
        paramServer.pull(paramServer.mTstKeys, paramServer.mGroupMap[SERVER], NULL);
        paramServer.mlModel(paramServer.mTstKeys, paramServer.mTstVals);
        paramServer.push(paramServer.mTstKeys, paramServer.mTstVals, paramServer.mGroupMap[SERVER], NULL);
        MessageBuffer res = paramServer.mMailbox.packSimpleRes(false, paramServer.mRecvTs);
        paramServer.response(res, paramServer.mGroupMap[W_SCHEDULER][0]);
        paramServer.cvEpoch.value = false;
    }
}

void ParameterServer::init(int id, int role)
{
    // basic runtime parameters
    mMachineId = id;
    mEpoch = -1;

    // connections
    mMailbox.mMachineId = mMachineId;
    mMailbox.init("5000");
    mNumServer = mMailbox.mAddrTable.size();

    // assign role
    mRole = role;
    // test: 133-server, 131-scheduler, 132-worker
    mGroupMap[SERVER].push_back(mMailbox.mHostMap["192.168.219.133"]);
    mGroupMap[WORKER].push_back(mMailbox.mHostMap["192.168.219.132"]);
    mGroupMap[WORKER].push_back(mMailbox.mHostMap["192.168.219.134"]);
    mGroupMap[W_SCHEDULER].push_back(mMailbox.mHostMap["192.168.219.131"]);
    if(mRole == SERVER)
    {
        int num = 100000;
        mTstKeys.reserve(num);
        mTstVals.reserve(num);
        srand(time(NULL));
        for(int i = 0; i < num; i++)
        {
            mTstKeys[i] = i;
            mTstVals[i] = (rand() % 100);
            mKvEng.kvStore.insert({mTstKeys[i], mTstVals[i]});
        }
    }
    // test end

    mState.value = PS_HALT;
    pthread_mutex_init(&(mState.mutex), NULL);
    pthread_cond_init(&(mState.cv), NULL);

    pthread_mutex_init(&(cvTrack.mutex), NULL);
    pthread_cond_init(&(cvTrack.cv), NULL);

    cvEpoch.value = false;
    pthread_mutex_init(&(cvEpoch.mutex), NULL);
    pthread_cond_init(&(cvEpoch.cv), NULL);

    // start working threads
    printf("[Parameter Server] start receiver thread.\n");
    if(pthread_create(&receiverThread, NULL, receiverThreadMain_p, NULL))
    {
        Util::printErrorAndExit("ParameterServer::pthread_create, receiver\n");
    }

    printf("[Parameter Server] start dispatch thread.\n");
    if(pthread_create(&dispatchThread, NULL, dispatchThreadMain_p, NULL))
    {
        Util::printErrorAndExit("ParameterServer::pthread_create, dispatch\n");
    }

    if(mRole == W_SCHEDULER)
    {
        printf("[Parameter Server] start schedule thread.\n");
        if(pthread_create(&scheduleThread, NULL, scheduleThreadMain_p, NULL))
        {
            Util::printErrorAndExit("ParameterServer::pthread_create, scheduler\n");
        }
    }

    if(mRole == WORKER)
    {
        printf("[Parameter Server] start worker thread.\n");
        if(pthread_create(&workerThread, NULL, workerThreadMain_p, NULL))
        {
            Util::printErrorAndExit("ParameterServer::pthread_create, worker\n");
        }
    }
}

void ParameterServer::destroy()
{
    pthread_join(receiverThread, NULL);
    pthread_join(dispatchThread, NULL);

    if(mRole == W_SCHEDULER)
    {
        pthread_join(scheduleThread, NULL);
    }

    if(mRole == WORKER)
    {
        pthread_join(workerThread, NULL);
    }

    pthread_mutex_destroy(&(mState.mutex));
    pthread_cond_destroy(&(mState.cv));

    pthread_mutex_destroy(&(cvTrack.mutex));
    pthread_cond_destroy(&(cvTrack.cv));

    pthread_mutex_destroy(&(cvEpoch.mutex));
    pthread_cond_destroy(&(cvEpoch.cv));

    mMailbox.destroy();
}

void ParameterServer::partition(std::vector<int> group, std::vector<int> keys)
{
    for(int i = 0; i < group.size(); i++)
    {
        mConsHash.insertMachine(mMailbox.mAddrTable[group[i]], group[i]);
    }

    for(int i = 0; i < keys.size(); i++)
    {
        int mId = mConsHash.kvHash(keys[i]);
        mkeyCache.insert({keys[i], mId});
    }
}

void ParameterServer::wait(int timestamp)
{
    pthread_mutex_lock(&(cvTrack.mutex));
    while(mTracker[timestamp].first != mTracker[timestamp].second)
    {
        pthread_cond_wait(&(cvTrack.cv), &(cvTrack.mutex));
    }
    pthread_mutex_unlock(&(cvTrack.mutex));
}

int ParameterServer::newRequest(std::vector<int> group)
{
    pthread_mutex_lock(&(cvTrack.mutex));
    mTracker.push_back(std::make_pair(group.size(), 0));
    pthread_mutex_unlock(&(cvTrack.mutex));
    return mTracker.size() - 1;
}

void ParameterServer::push(std::vector<int> keys, std::vector<double> values, std::vector<int> group, 
                           void (*callback)(void *args))
{
    int ts = newRequest(group);
    MessageBuffer mbuf = mMailbox.packPushVec(keys, values, ts);
    mMailbox.sendGroup(mbuf, group);
    mKvEng.setCallback(callback);
    wait(ts);
}

MessageBuffer ParameterServer::defaultPushCallback(MessageBuffer mbuf)
{
    PSPushVec psv;
    psv.ParseFromArray(mbuf.message, mbuf.msglen);

    printf("[server] kv push: %d\n", psv.keys_size());
    for(int i = 0; i < psv.keys_size(); i++)
    {
        mKvEng.kvStore[psv.keys(i)] += psv.values(i);
    }

    MessageBuffer ret = mMailbox.packSimpleRes(false, psv.timestamp());
    return ret;
}

void ParameterServer::pull(std::vector<int> keys, std::vector<int> group, void (*callback)(void *args))
{
    int ts = newRequest(group);
    MessageBuffer mbuf = mMailbox.packPullVec(keys, ts);
    mMailbox.sendGroup(mbuf, group);
    mKvEng.setCallback(callback);
    wait(ts);
}

MessageBuffer ParameterServer::defaultPullCallback(MessageBuffer mbuf)
{
    PSPullVec plv;
    plv.ParseFromArray(mbuf.message, mbuf.msglen);

    PSResponse res;
    printf("[server] kv pull: %d\n", plv.keys_size());
    for(int i = 0; i < plv.keys_size(); i++)
    {
        res.add_keys(plv.keys(i));
        res.add_vals(mKvEng.kvStore[plv.keys(i)]);
    }
    res.set_resp(true);
    res.set_timestamp(plv.timestamp());
    MessageBuffer ret = mMailbox.packMsg(res, MSG_RESPONSE);
    return ret;
}

void ParameterServer::response(MessageBuffer res, int from)
{
    mMailbox.sendMsg(mMailbox.mCliSockFd[from], res);
}

void ParameterServer::doResponse(MessageBuffer mbuf)
{
    PSResponse res;
    res.ParseFromArray(mbuf.message, mbuf.msglen);
    // not simple response, carry with data
    // response to pull
    if(!res.resp())
    {
        for(int i = 0; i < res.keys_size(); i++)
        {
            mKvEng.kvStore[res.keys(i)] = res.vals(i);
        }
    }

    // update tracker
    pthread_mutex_lock(&(cvTrack.mutex));
    mTracker[res.timestamp()].second++;
    if(mTracker[res.timestamp()].first == mTracker[res.timestamp()].second)
    {
        pthread_cond_broadcast(&(cvTrack.cv));
    }
    pthread_mutex_unlock(&(cvTrack.mutex));
}

void ParameterServer::doControl(MessageBuffer mbuf)
{
    PSControl pc;
    pc.ParseFromArray(mbuf.message, mbuf.msglen);
    if(pc.type() == CTL_LOAD)
    {
        // mIo.load();
        // test: simulate load
        printf("[worker] receive LOAD message.\n");
        int num = 10000;
        srand(time(NULL));
        for(int i = 0; i < num; i++)
        {
            mTstKeys.push_back(mMachineId + i);
            mTstVals.push_back(rand() % 1000);
        }
        // test end
        MessageBuffer res = mMailbox.packSimpleRes(false, pc.timestamp());
        response(res, mGroupMap[W_SCHEDULER][0]);
        printf("[worker] load parameters end.\n");
    }
    else if(pc.type() == CTL_EPOCH)
    {
        pthread_mutex_lock(&(cvEpoch.mutex));
        mRecvTs = pc.timestamp();
        cvEpoch.value = true;
        // notify worker thread
        pthread_cond_signal(&(cvEpoch.cv));
        pthread_mutex_unlock(&(cvEpoch.mutex));
    }
    else
    {
        // MSG_BARRIER
    }
    
}

void ParameterServer::mlModel(std::vector<int> keys, std::vector<double> vals)
{
    printf("[ParameterServer] run machine learning model\n");
}