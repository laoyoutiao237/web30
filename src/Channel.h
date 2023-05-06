#pragma once
#include<sys/epoll.h>
#include<functional>
class EventLoop;

class Channel
{
private:
    EventLoop &eloop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;

    std::function<void()> callback;
public:
    Channel(EventLoop &_ep, int _fd);
    ~Channel();

    //处理事件
    void handleEvent();
    //设置 events 可读 边沿触发
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();

    bool getInEpoll();
    void setInEpoll(bool);

    void setRevents(uint32_t);

    void setCallback(std::function<void()>);
};
