#pragma once
#include"Channel.h"
#include"Socket.h"
#include<map>
class EventLoop;
class Socket;
class Channel;

class Server
{
private:
    EventLoop &loop;
    int servfd;
    Socket serv_sock, cli_sock;
    Channel *servChannel;

    //保存连接用户描述符 -> Channel
    std::map<int, Channel*> users;
public:

    Server(EventLoop&, const char*);
    ~Server();

    void handleReadEvent(int);
    void newConnection();
};

