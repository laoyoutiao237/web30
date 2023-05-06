#pragma once
#include<sys/epoll.h>
#include<vector>
class Channel;
class Epoll
{
private:
    int m_epollfd;
    struct epoll_event *events;
public:
    
    Epoll(/* args */);
    ~Epoll();

    void addFd(int fd, uint32_t op);
    void removefd(int fd);

    //根据Channel 更新 epoll
    void updateChannel(Channel*);
    //int poll(int timeout = -1);
    std::vector<Channel*> poll(int timeout = -1);
};
