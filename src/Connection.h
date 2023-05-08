#pragma once
#include<functional>
#include"Socket.h"
class EventLoop;
class Socket;
class Channel;
class Buffer;

//抽象连接类
class Connection
{
private:
    EventLoop &loop;
    Socket cli_sock; //应该由此处关闭 sock
    Channel *channel;
    std::function<void(Socket&)> deleteConnectionCallback;

    std::string *inBuffer;
    Buffer * readBuffer;//读缓冲器
public:
    Connection(EventLoop &_loop, const Socket& sock);
    ~Connection();

    //事件回调函数 现为回响函数
    void echo();

    //设置连接断开后的处理函数，由创建者传入
    void setDeleteConnectionCallback(std::function<void(Socket&)>);

    void send(int sockfd);
};

