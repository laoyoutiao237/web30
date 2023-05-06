#include"Channel.h"
#include "Epoll.h"
#include"EventLoop.h"

Channel::Channel(EventLoop &_ep, int _fd):eloop(_ep), fd(_fd), events(0), revents(0), inEpoll(false){

}

Channel::~Channel()
{
}

//处理事件
void Channel::handleEvent(){
    callback();
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    eloop.updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _inEpoll){
    inEpoll = _inEpoll;
}

void Channel::setRevents(uint32_t _revents){
    revents = _revents;
}

void Channel::setCallback(std::function<void()> _cb){
    callback = _cb;
}