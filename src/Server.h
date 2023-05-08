#pragma once
#include"Socket.h"
#include<map>
#include<mutex>

class EventLoop;
class Socket;
class Channel;
class Acceptor;
class Connection;

class Server
{
private:
    EventLoop &loop;
    Acceptor *acceptor;
    std::mutex mtx;

    //保存连接用户描述符 -> Connection
    std::map<int, Connection*> connections;
public:

    Server(EventLoop&, const char*);
    ~Server();

    void newConnection(Socket &serv_sock);

    void deleteConnection(Socket &sock);
};

