#include"EventLoop.h"
#include"Epoll.h"
#include"Channel.h"
#include"ThreadPool.h"

EventLoop::EventLoop(/* args */):quit(false), ep(new Epoll()){
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
    delete ep;
    delete threadPool;
    printf("~EventLoop()\n");
}

void EventLoop::loop(){
    while(!quit){
        std::vector<Channel*> activechs = ep->poll();
        for(auto it = activechs.begin(); it != activechs.end(); ++it){
            (*it)->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch){
    ep->updateChannel(ch);
}

void EventLoop::addThread(std::function<void()> func){
    threadPool->add(func);
}