#ifndef _MESSAGE_SOCKET_H
#define _MESSAGE_SOCKET_H

#include <pthread.h>

#include <deque>
#include <vector>
#include <map>
#include <string>

#include <google/protobuf/message.h>


#include "util/Address.h"
#include "util/EpollEvent.h"
#include "util/MessageBuffer.h"

// Raft's messages
#include "message/AppendEntries.pb.h"
#include "message/AppendEntriesResult.pb.h"
#include "message/RegisterMessage.pb.h"
#include "message/RequestVote.pb.h"
#include "message/Vote.pb.h"
#include "message/Commit.pb.h"

// PS's messages
#include "message/PSPushVec.pb.h"
#include "message/PSPullVec.pb.h"
#include "message/PSResponse.pb.h"
#include "message/PSControl.pb.h"

// Graph's message
#include "message/GraphMessage.pb.h"
#include "message/GraphResp.pb.h"
#include "message/GraphControl.pb.h"

enum
{
    MSG_REG,                  // register
    MSG_REQUEST_VOTE,         // raft
    MSG_VOTE,
    MSG_APPEND_ENTRY,
    MSG_APPEND_ENTRY_RESULT,
    MSG_HEARTBEAT,
    MSG_COMMIT,
    MSG_INSTALL_SNAPSHOT,
    MSG_PUSH_VEC,            // parameter server
    MSG_PULL_VEC,
    MSG_RESPONSE,
    MSG_CONTROL,
    MSG_GRAPH,              // graph processing
    MSG_GRAPH_RESP,
    MSG_GRAPH_CTL
}MessageType;

enum
{
    CTL_LOAD,
    CTL_EPOCH,
    CTL_BARRIER,
    CTL_TERMINATE
}ControlType;

class MessageSocket
{
    public:
    int mMachineId;
    int mMsgSerialNum;
    int mRegNum;

    std::string mHost;
    std::string mPort;

    std::map<std::string, int> mHostMap;
    std::vector<Address> mAddrTable;

    int mServSockFd;
    int *mCliSockFd;
    int *mConnSockFd;

    EpollEvent mListenEvents;

    public:

    // fillAddrTable: read cluster address table from disk
    // parameters: cfgPath-configuration file path
    // outputs   : void
    void fillAddrTable(const std::string cfgPath);

    // init: initialize connections between this server and others in cluster
    // parameters: port-port to use
    // outputs   : void
    void init(const std::string port);

    // initServer: initialize server to receive connections
    // parameters: void
    // outputs   : void
    void initServer();

    // initClient: initialize client to connect servers
    // parameters: void
    // outputs   : void
    void initClient();

    // initClientFd: initialize client sockets file descriptors
    // parameters: host-the destination hostname
    //             port-the destination port
    // outputs   : socket fd if suceess, otherwise returns -1
    int initClientFd(const char *host, const char *port);

    // initServerFd: initialize client sockets file descriptors
    // parameters: port-the passive port
    //             lCnt-max number of connection，set as the size of cluster
    // outputs   : socket fd if success, otherwise returns -1
    int initServerFd(const char *port, int lCnt);

    // acceptConnects: receive connection from cluster
    // parameters: void
    // outputs   : void
    void acceptConnects();

    // nextMsgId: get next msgId, to prevent duplicate message
    // parameters: void
    // outputs   : void
    int nextMsgId();

    // packMsg: pack protobuf type messge to MessageBuffer
    // parameters: msg-protobuf type messge
    // outputs   : packed message buffer of protobuf message
    MessageBuffer packMsg(google::protobuf::Message &msg, int msgType);

    // packRegMsg: pack register message
    // parameters: void
    // outputs   : packed message buffer of register message
    MessageBuffer packRegMsg();

    // packRequestVote: pack request vote message
    // parameters: term-candidate's term
    //             candidateId-candidate requeting vote
    //             lastLogIndex-index of candidate's last log entry
    //             lastLogTerm-term of candidate's last log entry
    // outputs   : packed message buffer of RequestVote
    MessageBuffer packRequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm);

    // packVote: pack vote result message
    // parameters: term-current term, for candidate to update itself
    //             voteGranted-true means candidate received vote
    // outputs   : packed message buffer of Vote
    MessageBuffer packVote(int serId, int term, bool voteGranted);

    // packAppendEntry: pack append entries
    // parameters: term-leader's term
    //             leaderId-so followers can redirect clients
    //             prevLogIndex-index of log entry immediately preceding new ones
    //             prevLogTerm-term of prevLogIndex entry
    //             entries-log entries to store(empty for heartbeat; may send more than one for effeciency)
    //             leaderCommit-leader's commitIndex
    // outputs  : packed message buffer of AppendEntries
    MessageBuffer packAppendEntries(int term, int leaderId, int prevLogIndex,
                                    int prevLogTerm, std::vector<std::string> entries, int leaderCommit);
    
    // packAppendEntriesResult: pack append entries result
    // parameters: term-currentTerm, for leader to update itself
    //             success-true if follower contained entry matching prevLogIndex and prevLogTerm
    // outputs   : packed message buffer of AppendEntries Result
    MessageBuffer packAppendEntriesResult(int term, bool success);
    
    // packHeartBeat: pack heartbeat
    // parameters: term-leader's term
    //             leaderId-so followers can redirect clients
    //             prevLogIndex-index of log entry immediately preceding new ones
    //             prevLogTerm-term of prevLogIndex entry
    //             #entries-heartbeat, empty
    //             leaderCommit-leader's commitIndex
    // outputs  : packed message buffer of HeartBeat  
    MessageBuffer packHeartBeat(int term, int leaderId, int prevLogIndex, 
                                int prevLogTerm, int leaderCommit);

    // packCommit: pack commit message
    // parameters: commit-commit true or false
    // outputs   : packed message buffer of Commit
    MessageBuffer packCommit(bool commit);

    // attention: for ps, message buffer should allow to extend to adapt the vector

    // packPushVec: pack push vector
    // parameters: keys-keys of the vector
    //             values-values of the vector
    //             timestamp-timestamp of this range
    // outputs   : packed message buffer of PushVec
    MessageBuffer packPushVec(std::vector<int> &keys, std::vector<double> &values, int timestamp);

    // packPullVec: pack pull vector
    // parameters: keys-keys of the vector
    //             timestamp-timestamp of this range
    // outputs   : packed message buffer of PullVec
    MessageBuffer packPullVec(std::vector<int> &keys, int timestamp);

    // packResponse: pack response message
    // parameters: keys-keys
    //             vals-values
    //             isSimple-carry data or not
    //             timestamp-timestamp
    // outputs   : packed message buffer
    MessageBuffer packResponse(std::vector<int> keys, std::vector<double> vals, bool isSimple, int timestamp);

    // packSimpleRes: pack simple response message
    // parameters: isSimple-carry data or not (not in this case)
    //             timestamp-timestamp
    // outputs   : packed message buffer
    MessageBuffer packSimpleRes(bool isSimple, int timestamp);

    // packGResponse: pack graph response message
    // parameters: isSimle-carry data or not
    //             aggr-aggregator
    //             timestamp-timestamp
    // outputs   : packed message buffer
    // attention: merge with packResponse later!
    MessageBuffer packGResponse(bool isSimple, double aggr, int timestamp);

    // packControl: pack control message: LOAD, EPOCH, BARRIER, TERMINATE
    // parameters: type-control type
    //             timestamp-timestamp
    // outputs   : packed message buffer
    MessageBuffer packControl(int type, int timestamp);

    // packGControl: pack graph control message: LOAD, EPOCH, VOTETOHALT
    // parameters: type-control type
    //             timestamp-timestamp
    // outputs   : packed message buffer
    MessageBuffer packGControl(int type, int timestamp);

    // bsend: send a single buffer
    // parameters: fd-sender socket file descriptor
    //             buf-buffer to be send
    //             nleft-left bytes to send
    //             retry-times of retry
    // outputs   : send success or fail
    bool bsend(int fd, void *buf, int nleft, int retry);

    // sendMsg: send a message
    // parameters: fd-sender socket file descriptor
    //             msg-message to send
    // outputs   : void
    void sendMsg(int fd, MessageBuffer msg);

    // sendAll: send message to all servers in cluster
    // parameters: mbuf-message buffer to send
    // outputs   : void
    void sendAll(MessageBuffer mbuf);

    // sendGroup: send message to machines in group
    // parameters: mbuf-message buffer to send
    //             group-destination group
    // outputs   : void
    void sendGroup(MessageBuffer mbuf, std::vector<int> group);

    // brecv: receive a single buffer
    // parameters: fd-receiver socket file descriptor
    //             buf-buffer to receive message
    //             nleft-left bytes to receive
    //             retry-times of retry
    // outputs   : receive success or fail
    bool brecv(int fd, void *buf, int nleft, int retry);

    // recvMsg: receive a message
    // parameters: fd-receiver socket file descriptor
    //             msg-to store the received message
    // outputs   : message buffer of message received
    MessageBuffer recvMsg(int fd);

    // destroy: destroy message socket
    // parameters: void
    // outputs   : void
    void destroy();
};

#endif /* _MESSAGE_SOCKET_H*/
