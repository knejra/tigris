#include <unistd.h>

#include <random>
#include <algorithm>
#include <fstream>

#include "StateMachine.h"

extern StateMachine stateMachine;

void *receiverThreadMain(void *args)
{
    stateMachine.mMessageSocket.initServer();

    // wait until the connections have been built, start work
    pthread_mutex_lock(&(stateMachine.mainProcState.mutex));
    while(stateMachine.mainProcState.value != WORK)
    {
        pthread_cond_wait(&(stateMachine.mainProcState.cv), &(stateMachine.mainProcState.mutex));
    }
    pthread_mutex_unlock(&(stateMachine.mainProcState.mutex));

    while(stateMachine.mainProcState.value == WORK)
    {
        // read epoll, receive messages
        int ready = stateMachine.mMessageSocket.mListenEvents.waitEvent();
        for(int i = 0; i < ready; i++)
        {
            if(stateMachine.mMessageSocket.mListenEvents.mEvents[i].events & EPOLLIN)
            {
                MessageBuffer mbuf = stateMachine.mMessageSocket.recvMsg(stateMachine.mMessageSocket.mListenEvents.mEvents[i].data.fd);
                // if message type is HEARTBEAT, process immediately, then notify election thread
                if(mbuf.msgType == MSG_HEARTBEAT || mbuf.msgType == MSG_APPEND_ENTRY)
                {
                    AppendEntries ae;
                    ae.ParseFromArray(mbuf.message, mbuf.msglen);
                    //printf("[INFO] machine: %d receive leader's hearbeat\n", stateMachine.mMachineId);
                    if(ae.term() >= stateMachine.currentTerm)
                    {
                        pthread_mutex_lock(&(stateMachine.longLiveTheKing.mutex));
                        stateMachine.longLiveTheKing.value = ALIVE;
                        stateMachine.voteFor = NOVOTE;
                        pthread_mutex_unlock(&(stateMachine.longLiveTheKing.mutex));
                        pthread_cond_signal(&(stateMachine.longLiveTheKing.cv));
                        
                    }
                }
                // push messages of other types into msgQueue，wait for dispatch thread to handle them
                if(mbuf.msgType != MSG_HEARTBEAT)
                {
                    stateMachine.msgQueue.push(mbuf);
                }
            }
            else if(stateMachine.mMessageSocket.mListenEvents.mEvents[i].events & EPOLLHUP)
            {
                printf("[receiver] receiver thread: EPOLLHUP\n");
            }
            else if(stateMachine.mMessageSocket.mListenEvents.mEvents[i].events & EPOLLERR)
            {
                printf("[receiver] receiver thread: EPOLLERR\n");
            }
            else
            {
                printf("[receiver] receiver thread: UNEXPECTED ERROR\n");
            }            
        }
    }
}

void *electionThreadMain(void *args)
{
    struct timespec electionTime;
    stateMachine.updateElectionTime(electionTime);

    // wait until the connections have been built, start work
    pthread_mutex_lock(&(stateMachine.mainProcState.mutex));
    while(stateMachine.mainProcState.value != WORK)
    {
        pthread_cond_wait(&(stateMachine.mainProcState.cv), &(stateMachine.mainProcState.mutex));
    }
    pthread_mutex_unlock(&(stateMachine.mainProcState.mutex));

    printf("[election] start work\n");
    while(stateMachine.mainProcState.value == WORK)
    {
        switch(stateMachine.state)
        {
            case FOLLOWER:
            {
                //printf("[election] start election wait\n");
                pthread_mutex_lock(&(stateMachine.longLiveTheKing.mutex));
                pthread_cond_timedwait(&(stateMachine.longLiveTheKing.cv),
                                       &(stateMachine.longLiveTheKing.mutex), &electionTime);

                // If election timeout elapses without receiving AppendEntries RPC
                // from current leader or granting vote to candidate: convert to candidate
                if(stateMachine.longLiveTheKing.value == ALIVE)
                {
                    stateMachine.longLiveTheKing.value = UNKNOWN;
                    stateMachine.updateElectionTime(electionTime);
                    //printf("[election] follwer: leader alive, continue wait\n");
                }
                else
                {
                    if(stateMachine.voteFor == NOVOTE)
                    {
                        stateMachine.state = CANDIDATE;
                        printf("[election] follower: state transform to CANDIDATE\n");
                    }
                }
                pthread_mutex_unlock(&(stateMachine.longLiveTheKing.mutex));
                break;
            }
            case CANDIDATE:
            {
                // 1.Increment currentTerm
                printf("[Election] update term\n");
                stateMachine.currentTerm = stateMachine.currentTerm + 1;
                // 2.Vote for self
                printf("[Election] vote for myself\n");
                stateMachine.voteFor = stateMachine.mMachineId;
                stateMachine.numOfReceivedVote = 1;
                //memset(stateMachine.majorVote, 0, stateMachine.numOfServer * sizeof(int));
                // 3.Reset electionTimer
                stateMachine.updateElectionTime(electionTime);
                // 4.Send RequestVote RPCs to all other servers
                MessageBuffer mbuf = stateMachine.mMessageSocket.packRequestVote(stateMachine.currentTerm, stateMachine.mMachineId, 
                                                                                 stateMachine.mLog.getLastLogIndex(), stateMachine.mLog.getLastLogTerm());
                memset(stateMachine.majorVote, 0, stateMachine.numOfServer * sizeof(int));
                stateMachine.mMessageSocket.sendAll(mbuf);
                printf("[election] send request vote\n");
            
                pthread_mutex_lock(&(stateMachine.longLiveTheKing.mutex));
                // while(stateMachine.longLiveTheKing.value == UNKNOWN)
                // {
                //     pthread_cond_timedwait(&(stateMachine.longLiveTheKing.cv), 
                //                            &(stateMachine.longLiveTheKing.mutex), &electionTime);
                // }
                pthread_cond_timedwait(&(stateMachine.longLiveTheKing.cv), 
                                       &(stateMachine.longLiveTheKing.mutex), &electionTime);

                // wake up has 3 cases:
                // 1. If votes received from majority of servers: become leader
                // 2. If AppendEntries RPC received from new leader: convert to follower
                // 3. If election timeout elapses: start new election
                if(stateMachine.longLiveTheKing.value == ENTHRONE)
                {
                    stateMachine.state = LEADER;
                    printf("[election] state transform into LEADER\n");
                }
                else if(stateMachine.longLiveTheKing.value == ALIVE)
                {
                    stateMachine.state = FOLLOWER;
                    printf("[election] state transform back to FOLLOWER\n");
                }
                else if(stateMachine.longLiveTheKing.value == UNKNOWN)
                {
                    printf("[election] no leader win this term, start another election\n");
                }
                
                pthread_mutex_unlock(&(stateMachine.longLiveTheKing.mutex));
                break;
            }
            case LEADER:
            {
                // if the leader find higher-term leader exists, then return to follower 
                if(stateMachine.longLiveTheKing.value == ALIVE)
                {
                    stateMachine.state = FOLLOWER;
                    break;
                }

                // send heartbeat periodcally
                AppendEntries hb;
                MessageBuffer mbuf = stateMachine.mMessageSocket.packHeartBeat(stateMachine.currentTerm, stateMachine.mMachineId, stateMachine.mLog.getLastLogIndex(), 
                                                                               stateMachine.mLog.getLastLogTerm(), stateMachine.commitIndex);
                stateMachine.mMessageSocket.sendAll(mbuf);
                //printf("[election] leader send heartbeat\n");
                sleep(SPAN / 5);
                break;
            }
            default:
            {
                Util::printErrorAndExit("StateMachine::electionThreadMain invalid state");
            }
        }
    }
}

void *dispatchThreadMain(void *args)
{
    stateMachine.mMessageSocket.initClient();

    // build connection, notify receiver thread and election thread
    pthread_mutex_lock(&(stateMachine.mainProcState.mutex));
    stateMachine.mainProcState.value = WORK;
    pthread_cond_broadcast(&(stateMachine.mainProcState.cv));
    pthread_mutex_unlock(&(stateMachine.mainProcState.mutex));

    printf("[dispatch] start work\n");
    while(stateMachine.mainProcState.value == WORK)
    {
        MessageBuffer mbuf;
        stateMachine.msgQueue.pop(mbuf);
        switch(mbuf.msgType)
        {
            case MSG_APPEND_ENTRY:
            {
                printf("[dispatch] receive AppendEntries\n");
                stateMachine.doAppendEntries(mbuf);
                break;
            }
            case MSG_APPEND_ENTRY_RESULT:
            {
                printf("[dispatch] receive AppendEntriesResult\n");
                stateMachine.doAppendEntriesResult(mbuf);
                break;
            }
            case MSG_COMMIT:
            {
                printf("[dispatch] receive Commit\n");
                stateMachine.doCommit(mbuf);
                break;
            }
            case MSG_REQUEST_VOTE:
            {
                printf("[dispatch] receive RequestVote\n");
                stateMachine.doRequestVote(mbuf);
                break;
            }
            case MSG_VOTE:
            {
                printf("[dispatch] receive Vote\n");
                stateMachine.doVote(mbuf);
                break;
            }
            case MSG_INSTALL_SNAPSHOT:
            {
                printf("[dispatch] receive install snapshot\n");
                stateMachine.doInstallSnapshot(mbuf);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void *dominateThreadMain(void *args)
{
    char cmd[MAX_CMD_SIZE];
    memset(cmd, '\0', MAX_CMD_SIZE);

    pthread_mutex_lock(&(stateMachine.longLiveTheKing.mutex));
    while(stateMachine.longLiveTheKing.value != ENTHRONE)
    {
        pthread_cond_wait(&(stateMachine.longLiveTheKing.cv), &(stateMachine.longLiveTheKing.mutex));
    }
    pthread_mutex_unlock(&(stateMachine.longLiveTheKing.mutex));

    while(stateMachine.mainProcState.value == WORK)
    {
        if(stateMachine.state == LEADER)
        {
            int ready = stateMachine.usrCmdEvent.waitEvent();
            for(int i = 0; i < ready; i++)
            {
                if(stateMachine.usrCmdEvent.mEvents[i].events & EPOLLIN)
                {
                    size_t sz = read(stateMachine.usrCmdEvent.mEvents[i].data.fd, 
                                     cmd, MAX_CMD_SIZE);
                    cmd[sz - 1] = '\0';
                    std::string entry(cmd);
                    stateMachine.mLog.entries.push_back(LogEntry(stateMachine.currentTerm, entry));
                    printf("[command] %s, log size: %d\n", cmd, stateMachine.mLog.lenth());
                }
            }

            for(int i = 0; i < stateMachine.numOfServer; i++)
            {
                if(i != stateMachine.mMachineId && stateMachine.nextIndexList[i] < stateMachine.mLog.lenth())
                {
                    int prevLogIndex = stateMachine.mLog.getPrevLogIndex(stateMachine.nextIndexList[i]);
                    int prevLogTerm = stateMachine.mLog.getPrevLogTerm(stateMachine.nextIndexList[i]);
                    std::vector<std::string> vEntries;
                    for(int j = stateMachine.nextIndexList[i]; j < stateMachine.mLog.lenth(); j++)
                    {
                        vEntries.push_back(stateMachine.mLog.entries[j].value);
                    }
                    MessageBuffer mbuf = stateMachine.mMessageSocket.packAppendEntries(stateMachine.currentTerm, stateMachine.mMachineId, prevLogIndex,
                                                                                       prevLogTerm, vEntries, stateMachine.commitIndex);
                    stateMachine.mMessageSocket.sendMsg(stateMachine.mMessageSocket.mCliSockFd[i], mbuf);
                    printf("[dominate] send AppendEntries\n");
                }
            }

            sleep(SPAN);
        }
    }
}

void *snapshotThreadMain(void *args)
{
    // write log to persistent storage periodically
    pthread_mutex_lock(&(stateMachine.mainProcState.mutex));
    while(stateMachine.mainProcState.value != WORK)
    {
        pthread_cond_wait(&(stateMachine.mainProcState.cv), &(stateMachine.mainProcState.mutex));
    }
    pthread_mutex_unlock(&(stateMachine.mainProcState.mutex));

    while(stateMachine.mainProcState.value == WORK)
    {
        stateMachine.mSnapFile.write((char *)&stateMachine, sizeof(StateMachine));
        stateMachine.mSnapFile.flush();
        printf("[snapshot] write current state to disk\n");
        sleep(100);
    }
}

void StateMachine::init(int id)
{
    // start as follower
    state = FOLLOWER;
    // vote for none
    voteFor = NOVOTE;
    numOfReceivedVote = 0;
    numOfAppendSuccess = 0;
    // term init as 0
    currentTerm = 0;
    commitIndex = -1;

    // initialize connections
    mMachineId = id;
    mMessageSocket.mMachineId = mMachineId;
    mMessageSocket.init("5000");

    numOfServer = mMessageSocket.mAddrTable.size();

    nextIndexList = (int *)malloc((numOfServer) * sizeof(int));
    if(nextIndexList == NULL)
    {
        Util::printErrorAndExit("StateMachine::init malloc nextIndexList");
    }
    memset(nextIndexList, 0, numOfServer * sizeof(int));

    matchIndexList = (int *)malloc((numOfServer) * sizeof(int));
    if(matchIndexList == NULL)
    {
        Util::printErrorAndExit("StateMachine::init malloc matchIndexList");
    }
    memset(matchIndexList, 0, numOfServer * sizeof(int));

    majorVote = (int *)malloc(numOfServer * sizeof(int));
    if(majorVote == NULL)
    {
        Util::printErrorAndExit("StateMachine::init malloc majorVote");
    }
    memset(majorVote, 0, numOfServer * sizeof(int));

    majorAppend = (int *)malloc(numOfServer * sizeof(int));
    if(majorAppend == NULL)
    {
        Util::printErrorAndExit("StateMachine::init malloc majorAppend");
    }
    memset(majorAppend, 0, numOfServer * sizeof(int));

    // initialize user client
    usrCmdEvent.create();
    usrCmdEvent.addEventFd(STDIN_FILENO);

    // initialize mutex and conditionVariable
    // state machine's working state cv
    mainProcState.value = HALT;
    pthread_mutex_init(&(mainProcState.mutex), NULL);
    pthread_cond_init(&(mainProcState.cv), NULL);

    // cluster leader state cv
    longLiveTheKing.value = UNKNOWN;
    pthread_condattr_t pat;
    pthread_condattr_init(&pat);
    pthread_condattr_setclock(&pat, CLOCK_MONOTONIC);
    pthread_mutex_init(&(longLiveTheKing.mutex), NULL);
    pthread_cond_init(&(longLiveTheKing.cv), &pat);

    mSnapshotPath = "snapshot.bin";
    mSnapFile.open(mSnapshotPath, std::ios::out | std::ios::binary);

    // start all work threads
    printf("[Raft] start receiver thread.\n");
    if(pthread_create(&receiverThread, NULL, receiverThreadMain, NULL))
    {
        Util::printErrorAndExit("StateMachine::init create receiver thread failed");
    }

    printf("[Raft] start dispatch thread.\n");
    if(pthread_create(&dispatchThread, NULL, dispatchThreadMain, NULL))
    {
        Util::printErrorAndExit("StateMachine::init create dispatch thread failed");
    }

    printf("[Raft] start election thread.\n");
    if(pthread_create(&electionThread, NULL, electionThreadMain, NULL))
    {
        Util::printErrorAndExit("StateMachine::init create election thread failed");
    }

    printf("[Raft] start dominate thread.\n");
    if(pthread_create(&dominateThread, NULL, dominateThreadMain, NULL))
    {
        Util::printErrorAndExit("StateMachine::init create dominate thread failed");
    }

    printf("[Raft] start snapshot thread\n");
    if(pthread_create(&snapshotThread, NULL, snapshotThreadMain, NULL))
    {
        Util::printErrorAndExit("StateMachine::init create snapshot thread failed");
    }

}

void StateMachine::updateElectionTime(struct timespec &elecTime)
{
    clock_gettime(CLOCK_MONOTONIC, &elecTime);

    //std::default_random_engine re;
    //std::uniform_int_distribution<unsigned> ud(SPAN / 4, SPAN);

    srand(time(NULL));
    // keep the election timeout > heartbeas span
    int up = rand() % SPAN + SPAN / 2;
    elecTime.tv_sec = elecTime.tv_sec + up;
    printf("[update electione time] %ld\n", up);
}

void StateMachine::updateCommitIndex()
{
    // sort matchIndex, set N as matchIndex[n / 2]
    // Boyer-Moore Vote Algorithm ?
    int *tmpMatchIndex = (int *)malloc((numOfServer) * sizeof(int));
    std::sort(tmpMatchIndex, tmpMatchIndex + (numOfServer));
    int N = tmpMatchIndex[numOfServer / 2];
    if(N > commitIndex && mLog.entries[N].term == currentTerm)
    {
        commitIndex = N;
        printf("[update commit index] %ld\n", commitIndex);
    }
    free(tmpMatchIndex);
}

bool StateMachine::majority(int numVote)
{
    if(numVote > numOfServer / 2)
    {
        return true;
    }
    return false;
}

void StateMachine::doRequestVote(MessageBuffer mbuf)
{
    RequestVote rv;
    rv.ParseFromArray(mbuf.message, mbuf.msglen);
    bool voteGrant;

    if(rv.term() < currentTerm)
    {
        // replay false if term < currentTerm
        printf("[RequestVote] candidate's term %d < my term %d\n", rv.term(), currentTerm);
        voteGrant = false;
    }
    else
    {
        currentTerm = rv.term();
        // if voteFor is null or candidateId, and candidate's log is at least 
        // as up-to-date as receiver's log, grant vote
        if(voteFor == NOVOTE || voteFor == rv.candidateid())
        {
            voteFor = rv.candidateid();
            if(mLog.lenth() == 0 || (rv.lastlogterm() > mLog.getLastLogTerm()) || 
              (rv.lastlogterm() == mLog.getLastLogTerm() && rv.lastlogindex() >= mLog.lenth()))
            {
                voteGrant = true;
            }
            else
            {
                voteGrant = false;
            }
        }
        else
        {
            voteGrant = false;
        }   
    }

    printf("[RequestVote] voto for %d: %d\n", rv.candidateid(), voteGrant);
    MessageBuffer voteBuf = mMessageSocket.packVote(mMachineId, currentTerm, voteGrant);
    mMessageSocket.sendMsg(mMessageSocket.mCliSockFd[rv.candidateid()], voteBuf);
}

void StateMachine::doVote(MessageBuffer mbuf)
{
    Vote v;
    v.ParseFromArray(mbuf.message, mbuf.msglen);
    if(v.votegranted() && !majorVote[v.serverid()])
    {
        numOfReceivedVote++;
    }
    majorVote[v.serverid()] = 1;

    if(majority(numOfReceivedVote))
    {
        pthread_mutex_lock(&(longLiveTheKing.mutex));
        longLiveTheKing.value = ENTHRONE;
        // notify election thread and dominate thread
        pthread_cond_broadcast(&(longLiveTheKing.cv));
        pthread_mutex_unlock(&(longLiveTheKing.mutex));
    }
}

void StateMachine::doAppendEntries(MessageBuffer mbuf)
{
    if(state == LEADER)
    {
        printf("leader: unexpected AppendEnries RPC\n");
        return ;
    }

    AppendEntries ae;
    ae.ParseFromArray(mbuf.message, mbuf.msglen);

    AppendEntriesResult ar;
    ar.set_serverid(mMachineId);

    if(ae.term() < currentTerm || !mLog.check(ae.prevlogindex(), ae.prevlogterm()))
    {
        printf("ae term: %d, currentTerm: %d\n", ae.term(), currentTerm);
        currentTerm = ae.term();
        ar.set_success(false);
    }
    else
    {
        if(mLog.lenth() > ae.prevlogindex() + 1 && mLog.entries[ae.prevlogindex() + 1].term != ae.term())
        {
            mLog.del(ae.prevlogindex() + 1);
        }

        for(int i = 0; i < ae.entries_size(); i++)
        {
            LogEntry le = LogEntry(ae.term(), ae.prevlogindex() + i, ae.entries(i));
            printf("[AppendEntries] add log: %s\n", ae.entries(i).c_str());
            mLog.entries.push_back(le);
        }

        if(ae.leadercommit() > commitIndex)
        {
            commitIndex = std::min(ae.leadercommit(), mLog.lenth() - 1);
        }

        ar.set_success(true);
    }

    MessageBuffer mb = mMessageSocket.packMsg(ar, MSG_APPEND_ENTRY_RESULT);
    mMessageSocket.sendMsg(mMessageSocket.mCliSockFd[ae.leaderid()], mb);
}

void StateMachine::doAppendEntriesResult(MessageBuffer mbuf)
{
    if(state == CANDIDATE || state == FOLLOWER)
    {
        return ;
    }

    AppendEntriesResult ar;
    ar.ParseFromArray(mbuf.message, mbuf.msglen);
    if(ar.success() && !majorAppend[ar.serverid()])
    {
        numOfAppendSuccess++;
        nextIndexList[ar.serverid()] = mLog.lenth();
        matchIndexList[ar.serverid()] = nextIndexList[ar.serverid()] - 1;
        majorAppend[ar.serverid()] = 1;
    }

    if(!ar.success())
    {
        nextIndexList[ar.serverid()]--;
    }

    if(majority(numOfAppendSuccess))
    {
        memset(majorAppend, 0, numOfServer * sizeof(int));
        MessageBuffer mbuf = mMessageSocket.packCommit(true);
        mMessageSocket.sendAll(mbuf);
    }
}

void StateMachine::doCommit(MessageBuffer mbuf)
{
    if(state == LEADER)
    {
        return ;
    }

    Commit cm;
    cm.ParseFromArray(mbuf.message, mbuf.msglen);
    if(cm.commit())
    {
        apply();
    }
}

void StateMachine::doInstallSnapshot(MessageBuffer mbuf)
{

}

void StateMachine::apply()
{
    printf("[State Machine] apply, log size: %d\n", mLog.lenth());
}

void StateMachine::destroy()
{
    // terminate all working threads
    pthread_join(receiverThread, NULL);
    pthread_join(dispatchThread, NULL);
    pthread_join(electionThread, NULL);
    pthread_join(dominateThread, NULL);
    pthread_join(snapshotThread, NULL);

    pthread_mutex_destroy(&(mainProcState.mutex));
    pthread_cond_destroy(&(mainProcState.cv));

    pthread_mutex_destroy(&(longLiveTheKing.mutex));
    pthread_cond_destroy(&(longLiveTheKing.cv));

    free(nextIndexList);
    free(matchIndexList);
    free(majorVote);
    free(majorAppend);

    mSnapFile.close();
    
    // terminate connections
    mMessageSocket.destroy();
}