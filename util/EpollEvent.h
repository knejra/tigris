#ifndef _EPOLLEVENT_H
#define _EPOLLEVENT_H

#include <sys/epoll.h>

#include "Util.h"

#define MAXEVENT 128

class EpollEvent
{
    public:
    int mEpollFd;

    int mEventNum;

    struct epoll_event mEvents[MAXEVENT];

    public:
    
    void create()
    {
        mEpollFd = epoll_create1(EPOLL_CLOEXEC);
        if(mEpollFd == -1)
        {
            Util::printErrorAndExit("[Error] Epoll::create epoll_create1");
        }
    }

    void addEventFd(int fd)
    {
        struct epoll_event ev;
        
        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        if(epoll_ctl(mEpollFd, EPOLL_CTL_ADD, fd, &ev) == -1)
        {
            Util::printErrorAndExit("[Error] Epoll::addEventFd epoll_ctl");
        }
        mEventNum++;
    }

    void removeEventFd(int fd)
    {
        struct epoll_event ev;

        ev.data.fd = fd;
        ev.events = EPOLLIN | EPOLLET;
        if(epoll_ctl(mEpollFd, EPOLL_CTL_DEL, fd, &ev) == -1)
        {
            Util::printErrorAndExit("[Error] Epoll::removeEventFd epoll_ctl");
        }
        mEventNum--;
    }

    int waitEvent()
    {
        int ready = epoll_wait(mEpollFd, mEvents, mEventNum, -1);
        return ready;
    }

};

#endif // _EPOLL_EVENT_H