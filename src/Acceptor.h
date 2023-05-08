#pragma once
#include<functional>
#include"InetAddress.h"
#include"Socket.h"

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor
{
private:
    EventLoop &loop;
    Socket serv_sock, cli_sock;
    InetAddress serv_addr;
    Channel *acceptChannel;
    std::function<void(Socket &)> newConnectionCallback;
public:
    Acceptor(EventLoop &_loop, const char *);
    ~Acceptor();

    void acceptConnection(); 

    void setNewConnectionCallback(std::function<void(Socket &)>);
};


