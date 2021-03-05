#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include <string>
#include <vector>

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>

#include "util/ThreadSafeQueue.h"
#include "util/ConditionVariable.h"
#include "util/Log.h"
#include "util/Types.h"

#include "MessageSocket.h"

#define HALT 0
#define WORK 1

#define NOVOTE -1

#define SPAN 10

#define MAX_CMD_SIZE 255

enum
{
    UNKNOWN,     
    ALIVE,      
    ENTHRONE   
}Situation;

enum
{
    LEADER,
    CANDIDATE,
    FOLLOWER
}State;

class StateMachine
{
    public:
    // select function:
    // 0x1, start consensus(Raft)
    // 0x2, start distributed ML(parameter servers)
    // TODO: 0x4, start graph process(pregel)
    uint8_t mSelect;

    int mMachineId;
    int numOfServer;
    // latest term server has seen 
    // initialized to 0 on first boot, increases monotonically
    int currentTerm;

    std::string mClusterCfgPath;
    std::string mSnapshotPath;
    std::ofstream mSnapFile;

    MessageSocket mMessageSocket;
    ThreadSafeQueue<MessageBuffer> msgQueue;

    // receiverThread: receive message from cluster
    pthread_t receiverThread;
    // electionThread: manage the state of state machine
    pthread_t electionThread;
    // dispatchThread: do work according to message
    pthread_t dispatchThread;
    // dominateThread: the leader do client's command
    pthread_t dominateThread;
    // snapshotThread: do snapshot
    pthread_t snapshotThread;

    EpollEvent usrCmdEvent;

    // mutex of state should be a read-write lock
    // the only thread that can change state is election thread
    // election thread add write lock when it change state
    // receiver thread and dispatch thread read state, add read lock
    int state;

    // mainProcState: coordinate all thread, play a role as barrier
    ConditionVariable<int> mainProcState;
    // longLiveTheKing: check if the leader alive
    // if the leader dead, notify all the working thread
    ConditionVariable<int> longLiveTheKing;

    // candidateId that received vote in currentTerm
    int voteFor;
    int numOfReceivedVote;
    int *majorVote;

    // the leader keeps track of the highest index it knows to be commited,
    // and it includes that index in future AppendEntries RPCs (including
    // heartbeats) so that the other servers eventually find out. Once a
    // follower learns that a log entry is committed, it applies the entry
    // to its local state machine (in log order)
    // DESIGN: check commitIndex
    int commitIndex;
    int numOfAppendSuccess;
    int *majorAppend;

    int lastApplied;

    // log entries; each entry contains command for state machine
    // and term when entry was received by leader (first index is 1)
    Log mLog;
    // leader maintains a nextIndex for each follower, which is the
    // index of the next log entry the leader will send to that follower
    // when leader fist comes to power, it initialize all nextIndex
    // values to the index just after the last one in its log
    int *nextIndexList;
    int *matchIndexList;

    // for snapshot
    int lastIncludeIndex;
    int lastIncludeTerm;

    public:
    void init(int id);

    // updateElectionTime: update the next election time
    // parameters: elecTime-the structure of election time
    // outputs   : void
    void updateElectionTime(struct timespec &elecTime);

    // updateCommitIndex: update commitIndex
    // parameters: void
    // outputs   : void
    void updateCommitIndex();

    // majority: check if the candidate has received majority votes
    // parameters: numVote-vote already received
    // outputs   : true if candidate receive votes from majority, otherwise false
    bool majority(int numVote);

    // doRequestVote: handle RequestVote RPCs
    // parameters: mbuf-message buffer of RequestVote message
    // outputs   : void
    void doRequestVote(MessageBuffer mbuf);

    // doVote: handle vote result
    // parameters: mbuf-message buffer of vote message
    // outputs   : void
    void doVote(MessageBuffer mbuf);

    // doAppendEntries: handle AppendEntries RPCs
    // parameters: mbuf-message buffer of AppendEnries message
    // outputs   : void
    void doAppendEntries(MessageBuffer mbuf);

    // doAppendEntriesResult: handle AppendEntries result
    // parameters: mbuf-message buffer of AppendEntriesResult message
    // outputs   : void
    void doAppendEntriesResult(MessageBuffer mbuf);

    // doCommit: handle commit, apply entries to the state machine
    // parameters: mbuf-message buffer of commit message
    // outputs   : void
    void doCommit(MessageBuffer mbuf);

    // doInstallSnapshot: handle install snapshot
    // parameters: mbuf-message buffer of install snapshot
    // outputs   : void
    void doInstallSnapshot(MessageBuffer mbuf);

    // apply: apply entries to the state machine
    // parameters: void
    // outputs   : void
    void apply();

    // destroy: halt the main thread
    // parameters: void
    // outputs   : void
    void destroy();

};

#endif // _STATE_MACHINE_H