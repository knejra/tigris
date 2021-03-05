#define _BSD_SOURCE

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "util/Util.h"
#include "MessageSocket.h"

#include <iostream>

void MessageSocket::fillAddrTable(const std::string cfgPath)
{
    Address adr1("192.168.219.133", mPort);
    Address adr2("192.168.219.131", mPort);
    Address adr3("192.168.219.132", mPort);
    Address adr4("192.168.219.134", mPort);

    mAddrTable.push_back(adr1);
    mHostMap.insert({"192.168.219.133", 0});
    mAddrTable.push_back(adr2);
    mHostMap.insert({"192.168.219.131", 1});
    mAddrTable.push_back(adr3);
    mHostMap.insert({"192.168.219.132", 2});
    mAddrTable.push_back(adr4);
    mHostMap.insert({"192.168.219.134", 3});
}

void MessageSocket::initServer()
{
    int numInCluster = mAddrTable.size();
    mRegNum = 0;

    mListenEvents.create();

    printf("[server] init server sockets, and listen.\n");
    mPort = mAddrTable[mMachineId].port;
    mServSockFd = initServerFd(mPort.c_str(), numInCluster);

    printf("[server] init connect sockets.\n");
    mConnSockFd = (int *)malloc(numInCluster * sizeof(int));
    if(mConnSockFd == NULL)
    {
        Util::printErrorAndExit("MessageSocket::init malloc");
    }

    printf("[server] accept connect.\n");
    while(mRegNum < numInCluster - 1)acceptConnects();

    printf("[server] init server success.\n");
}

void MessageSocket::initClient()
{
    int numInCluster = mAddrTable.size();

    printf("[client] init client socket, and connect.\n");
    mCliSockFd = (int *)malloc(numInCluster * sizeof(int));
    if(mCliSockFd == NULL)
    {
        Util::printErrorAndExit("MessageSocket::initClient malloc");
    }

    for(int i = 0; i < numInCluster; i++)
    {
        if(i != mMachineId)
        {
            mCliSockFd[i] = -1;
            int retry = 0;
            while(mCliSockFd[i] == -1)
            {
                mCliSockFd[i] = initClientFd(mAddrTable[i].hostname.c_str(), mAddrTable[i].port.c_str());
                retry++;
                sleep(1);
                if(retry > 60)
                {
                    Util::printErrorAndExit("MessageSock::init client socket failed");
                }
            }
        }
    }

    printf("[client] init client success.\n");
}

void MessageSocket::init(const std::string port)
{
    mPort = port;
    fillAddrTable("");
}

int MessageSocket::initClientFd(const char *host, const char *port)
{
    struct addrinfo hints, *result, *p;
    int sockFd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_flags |= AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;

    int ret = getaddrinfo(host, port, &hints, &result);
    if(ret != 0)
    {
        Util::printErrorAndExit("MessageSocket::initClientFd getaddrinfo");
    }

    for(p = result; p; p = p->ai_next)
    {
        if((sockFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }

        if(connect(sockFd, p->ai_addr, p->ai_addrlen) != -1)
        {
            break;
        }

        close(sockFd);
    }

    freeaddrinfo(result);

    if(!p)
    {
        //Util::printErrorAndExit("MessageSocket::Init client socket failed");
        return -1;
    }

    return sockFd;
}

int MessageSocket::initServerFd(const char *port, int lCnt)
{
    struct addrinfo hints, *result, *p;
    int optval = 1;

    struct sockaddr_in cliAddr;
    memset((char *)&cliAddr, 0, sizeof(cliAddr));
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr = INADDR_ANY;
    cliAddr.sin_port = htons(5000);

    int sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockFd < 0)
    {
        Util::printErrorAndExit("MessageSocket::initServerFd socket");
    }
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    int retry = 0;
    while(bind(sockFd, (const sockaddr *)&cliAddr, sizeof(cliAddr)) < 0)
    {
        if(errno != EADDRINUSE)
        {
            Util::printErrorAndExit("MessageSocket::initServerFd bind");
        }
        retry++;
        if(retry % 10 == 0)
        {
            Util::PrintWarning("MessageSocket::initServerFd bind");
        }
        if(retry > 60)
        {
            close(sockFd);
            Util::printErrorAndExit("MessageSocket::initServerFd bind failed");
        }
    }

    if(listen(sockFd, lCnt) < 0)
    {
        close(sockFd);
        Util::printErrorAndExit("MessageSocket::initServerFd listen");
    }

    return sockFd;
}

void MessageSocket::acceptConnects()
{
    struct sockaddr_storage claddr;
    char host[NI_MAXHOST];
    char port[NI_MAXSERV];
    int regMachineId = -1;

    socklen_t addrlen = sizeof(struct sockaddr_storage);
    int connfd = accept(mServSockFd, (struct sockaddr *)&claddr, &addrlen);
    if(connfd == -1)
    {
        Util::printErrorAndExit("MessageSocket::acceptConnects accept");
    }
    if(getnameinfo((const sockaddr *)&claddr, addrlen, host, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
    {
        printf("[server] Register message from (%s, %s)\n", host, port);
        if(mHostMap.find(host) != mHostMap.end())
        {
            regMachineId = mHostMap[std::string(host)];
        }
        else
        {
            Util::PrintWarning("MessageSocket::acceptConnects receive message out of cluster");
            return ;
        }
        
    }
    else
    {
        Util::printErrorAndExit("MessageSocket::acceptConnect receive unexpected connect");
    }

    mConnSockFd[regMachineId] = connfd;
    mListenEvents.addEventFd(mConnSockFd[regMachineId]);
    mRegNum++;
}

int MessageSocket::nextMsgId()
{
    return (mMsgSerialNum++);
}

MessageBuffer MessageSocket::packMsg(google::protobuf::Message &msg, int msgType)
{
    MessageBuffer mbuf;

    // Serialize
    size_t msglen = msg.ByteSize();
    mbuf.message = (char *)malloc(msglen * sizeof(char));
    memset(mbuf.message, 0, msglen);
    msg.SerializeToArray(mbuf.message, msglen);

    // Fill Header
    mbuf.fixed = 0x9a;
    mbuf.msglen = msglen;
    mbuf.msgId = 1;
    mbuf.msgType = msgType;
    mbuf.from = mMachineId;
    
    return mbuf;
}

// MessageBuffer MessageSocket::packRegMsg()
// {
//     RegisterMessage rm;
//     // message type, for dispatcher to diliver to message
//     rm.set_machineid(mMachineId);
//     return mbuf;
// }

MessageBuffer MessageSocket::packRequestVote(int term, int candidateId, int lastLogIndex, int lastLogTerm)
{
    RequestVote rv;
    rv.set_term(term);
    rv.set_candidateid(candidateId);
    rv.set_lastlogindex(lastLogIndex);
    rv.set_lastlogterm(lastLogTerm);
    MessageBuffer mbuf = packMsg(rv, MSG_REQUEST_VOTE);
    return mbuf;
}

MessageBuffer MessageSocket::packVote(int serId, int term, bool voteGranted)
{
    Vote v;
    v.set_serverid(serId);
    v.set_term(term);
    v.set_votegranted(voteGranted);
    MessageBuffer mbuf = packMsg(v, MSG_VOTE);
    return mbuf;
}

MessageBuffer MessageSocket::packAppendEntries(int term, int leaderId, int prevLogIndex, 
                                              int prevLogTerm, std::vector<std::string> entries, int leaderCommit)
{
    AppendEntries ae;
    ae.set_term(term);
    ae.set_leaderid(leaderId);
    ae.set_prevlogindex(prevLogIndex);
    ae.set_prevlogterm(prevLogTerm);
    for(int i = 0; i < entries.size(); i++)
    {
        ae.add_entries(entries[i]);
    }
    ae.set_leadercommit(leaderCommit);
    MessageBuffer mbuf = packMsg(ae, MSG_APPEND_ENTRY);
    return mbuf;
}

MessageBuffer MessageSocket::packAppendEntriesResult(int term, bool success)
{
    AppendEntriesResult r;
    r.set_term(term);
    r.set_success(success);
    MessageBuffer mbuf = packMsg(r, MSG_APPEND_ENTRY_RESULT);
    return mbuf;
}

MessageBuffer MessageSocket::packHeartBeat(int term, int leaderId, int prevLogIndex, 
                                           int prevLogTerm, int leaderCommit)
{
    AppendEntries ae;
    ae.set_term(term);
    ae.set_leaderid(leaderId);
    ae.set_prevlogindex(prevLogIndex);
    ae.set_prevlogterm(prevLogTerm);
    ae.set_leadercommit(leaderCommit);
    ae.add_entries("");
    MessageBuffer mbuf = packMsg(ae, MSG_HEARTBEAT);
    return mbuf;
}

MessageBuffer MessageSocket::packCommit(bool commit)
{
    Commit c;
    c.set_commit(commit);
    MessageBuffer mbuf = packMsg(c, MSG_COMMIT);
    return mbuf;
}

MessageBuffer MessageSocket::packPushVec(std::vector<int> &keys, std::vector<double> &values, int timestamp)
{
    PSPushVec pv;

    for(int i = 0; i < keys.size(); i++)
    {
        pv.add_keys(keys[i]);
    }

    for(int i = 0; i < values.size(); i++)
    {
        pv.add_values(values[i]);
    }

    pv.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(pv, MSG_PUSH_VEC);
    return mbuf;
}

MessageBuffer MessageSocket::packPullVec(std::vector<int> &keys, int timestamp)
{
    PSPullVec pv;

    for(int i = 0; i < keys.size(); i++)
    {
        pv.add_keys(keys[i]);
    }

    pv.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(pv, MSG_PULL_VEC);
    return mbuf;
}

MessageBuffer MessageSocket::packResponse(std::vector<int> keys, std::vector<double> vals, bool isSimple, int timestamp)
{
    PSResponse res;
    if(!isSimple)
    {
        for(int i = 0; i < keys.size(); i++)
        {
            res.add_keys(keys[i]);
        }

        for(int i = 0; i < vals.size(); i++)
        {
            res.add_vals(vals[i]);
        }
    }
    res.set_resp(isSimple);
    res.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(res, MSG_RESPONSE);
    return mbuf;
}

MessageBuffer MessageSocket::packGResponse(bool isSimple, double aggr, int timestamp)
{
    GraphResp res;
    res.set_resp(isSimple);
    res.set_aggrgate(aggr);
    res.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(res, MSG_GRAPH_RESP);
    return mbuf;
}

MessageBuffer MessageSocket::packSimpleRes(bool isSimple, int timestamp)
{
    PSResponse res;
    res.set_resp(isSimple);
    res.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(res, MSG_RESPONSE);
    return mbuf;
}

MessageBuffer MessageSocket::packControl(int type, int timestamp)
{
    PSControl pc;
    pc.set_type(type);
    pc.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(pc, MSG_CONTROL);
    return mbuf;
}

MessageBuffer MessageSocket::packGControl(int type, int timestamp)
{
    GContorl gc;
    gc.set_type(type);
    gc.set_timestamp(timestamp);
    MessageBuffer mbuf = packMsg(gc, MSG_GRAPH_CTL);
    return mbuf;
}

bool MessageSocket::bsend(int fd, void *buf, int nleft, int retry)
{
    ssize_t ret = 0, nsend = 0;
    while(nleft > 0)
    {
        ret = send(fd, (char *)buf + nsend, nleft, MSG_DONTWAIT);
        if(ret < 0)
        {
            if(errno == EINTR || errno == EAGAIN)
            {
                continue;
            }
            else
            {
                Util::PrintWarning("MessageSocket::bsend unexpected error");
            }

            retry--;
            if(retry < 0)
            {
                Util::PrintWarning("MessageSocket::bsend failed");
                return false;
            }
        }
        nsend += ret;
        nleft -= ret;
    }

    return true;
}

void MessageSocket::sendMsg(int fd, MessageBuffer mbuf)
{
    ssize_t nleft = offsetof(MessageBuffer, message);
    if(!bsend(fd, &mbuf, nleft, 40))
    {
        Util::PrintWarning("MassageSocket::sendMsg: send head failed");
        return;
    }
 
    nleft = mbuf.msglen;
    if(!bsend(fd, mbuf.message, nleft, 40))
    {
        Util::PrintWarning("MessageSocket::sendMsg: send data failed");
        return;
    }
}

void MessageSocket::sendAll(MessageBuffer mbuf)
{
    for(int i = 0; i < mAddrTable.size(); i++)
    {
        if(i != mMachineId)
        {
            sendMsg(mCliSockFd[i], mbuf);
        }
    }
}

void MessageSocket::sendGroup(MessageBuffer mbuf, std::vector<int> group)
{
    for(int i = 0; i < group.size(); i++)
    {
        sendMsg(mCliSockFd[group[i]], mbuf);
    }
}

bool MessageSocket::brecv(int fd, void *buf, int nleft, int retry)
{
    ssize_t ret = 0, nrecv = 0;
    while(nleft > 0)
    {
        ret = recv(fd, (char *)buf + nrecv, nleft, MSG_DONTWAIT);
        if(ret < 0)
        {
            if(errno == EINTR || errno == EAGAIN)
            {
                continue;
            }

            retry--;
            if(retry < 0)
            {
                Util::PrintWarning("MessageSocket::brecv failed");
                return false;
            }
        }
        else
        {
            nrecv += ret;
            nleft -= ret;
        }
    }

    return true;
}

MessageBuffer MessageSocket::recvMsg(int fd)
{
    MessageBuffer mbuf;

    size_t nleft = offsetof(MessageBuffer, message);
    if(!brecv(fd, &mbuf, nleft, 40))
    {
        Util::printErrorAndExit("MessageSocket::recvMsg receive head failed");
    }

    mbuf.message = (char *)malloc(mbuf.msglen * sizeof(char));
    nleft = mbuf.msglen;
    if(!brecv(fd, mbuf.message, nleft, 40))
    {
        Util::printErrorAndExit("MessageSocket::recvMsg receive data failed");
    }
 
    return mbuf;
}

void MessageSocket::destroy()
{
    free(mCliSockFd);
    free(mConnSockFd);
}




