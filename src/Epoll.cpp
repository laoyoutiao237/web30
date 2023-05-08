#include"Epoll.h"
#include"util.h"
#include<unistd.h>
#include<string.h>
#include"Channel.h"

#define MAX_EVENTS 1024

Epoll::Epoll(/* args */):m_epollfd(-1), events(nullptr){
    m_epollfd = epoll_create1(0);
    errif(m_epollfd == -1, "epoll create err\n");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll()
{
    if(m_epollfd != -1){
        close(m_epollfd);
        m_epollfd = -1;    
    }  
    delete [] events; 
    printf("~Epoll()\n");
}

void Epoll::addFd(int fd, uint32_t op){
    epoll_event ev;
    bzero(&ev, sizeof(ev));

    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

void Epoll::removefd(int sockfd){
    //epoll_ctl(m_epollfd, EPOLL_CTL_DEL, sockfd, nullptr);
    close(sockfd);
}

// int Epoll::poll(int timeout){
    
//     int nfds = ::epoll_wait(m_epollfd, events, MAX_EVENTS, timeout);
//     errif(nfds == -1, "epoll wait err\n");
//     //std::vector<epoll_event> activeEvents(nfds);
//     // std::vector<epoll_event> activeEvents;
//     // for(int i = 0; i < nfds; ++i){
//     //     //activeEvents[i] = events[i];
//     //     activeEvents.push_back(events[i]);
//     // }
//     // return activeEvents;
//     return nfds;
// }

//监听事件 并返回就绪事件队列
std::vector<Channel*> Epoll::poll(int timeout){
    
    int nfds = ::epoll_wait(m_epollfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait err\n");
    std::vector<Channel*> activeEvents(nfds);
    // std::vector<epoll_event> activeEvents;
    for(int i = 0; i < nfds; ++i){
        //activeEvents[i] = events[i];
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setReady(events[i].events);
        //获取事件
        activeEvents[i] = ch;
    }
    return activeEvents;
}

//更新事件
void Epoll::updateChannel(Channel* channel){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();

    if(!channel->getInEpoll()){
        errif(epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add err");
        channel->setInEpoll(true);
    }else{
        errif(epoll_ctl(m_epollfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify err");
    }
}