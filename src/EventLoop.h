#pragma once
#include<functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;
public:
    EventLoop(/* args */);
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);

    void addThread(std::function<void()> func);
};


