#include"Channel.h"
#include "Epoll.h"
#include"EventLoop.h"
#include<unistd.h>

Channel::Channel(EventLoop &_ep, int _fd)
:eloop(_ep), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true){

}

Channel::~Channel()
{
    // if(fd != -1){
    //     close(fd);
    //     fd = -1;
    // }
}

//处理事件
void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){
        if(useThreadPool)
            eloop.addThread(readCallback); //线程池
        else
            readCallback();
    }
    if(ready & (EPOLLOUT)){
        if(useThreadPool)
            eloop.addThread(writeCallback); //线程池
        else
            writeCallback();
    }
}

void Channel::enableRead(){
    events |= EPOLLIN | EPOLLPRI;
    eloop.updateChannel(this);
}

void Channel::useET(){
    events |= EPOLLET;
    eloop.updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}
uint32_t Channel::getRevents(){
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _inEpoll){
    inEpoll = _inEpoll;
}

void Channel::setReady(uint32_t _revents){
    ready = _revents;
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}

void Channel::setWriteCallback(std::function<void()> _cb){
    writeCallback = _cb;
}

void Channel::setUseThreadPool(bool use){
    useThreadPool = use;
}