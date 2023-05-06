#include"EventLoop.h"
#include"Epoll.h"
#include"Channel.h"

EventLoop::EventLoop(/* args */):quit(false), ep(new Epoll()){
}

EventLoop::~EventLoop()
{
    delete ep;
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