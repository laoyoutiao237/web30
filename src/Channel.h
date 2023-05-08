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
    uint32_t ready;
    bool inEpoll;
    bool useThreadPool;

    std::function<void()> readCallback;
    std::function<void()> writeCallback;

public:
    Channel(EventLoop &_ep, int _fd);
    ~Channel();

    //处理事件
    void handleEvent();
    //设置 events 可读 边沿触发
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();

    bool getInEpoll();
    void setInEpoll(bool);
    void useET();

    void setReady(uint32_t);

    void setReadCallback(std::function<void()>);
    void setWriteCallback(std::function<void()>);
    
    void setUseThreadPool(bool use = true);
};
