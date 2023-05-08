#include"Server.h"
#include"EventLoop.h"
#include"InetAddress.h"
#include"Channel.h"
#include<map>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include"Acceptor.h"
#include"Connection.h"

Server::Server(EventLoop& _loop, const char* ch):loop(_loop), connections(), acceptor(nullptr){
    acceptor = new Acceptor(loop, ch); 
    //绑定对新连接cli_sock的处理，回调函数
    std::function<void(Socket&)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server()
{
    delete acceptor;
}

void Server::newConnection(Socket &sock){
    //创建连接
    if(sock.getFd() != -1){
        Connection *conn = new Connection(loop, sock);
        std::function<void(Socket &sock)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);//绑定删除的回调函数
        {
            std::unique_lock<std::mutex> lock(mtx);
            connections[sock.getFd()] = conn; //记录连接，方便后续删除  
        }     
    }
    
}

//删除连接
void Server::deleteConnection(Socket &sock){
    if(sock.getFd() != -1){
        std::unique_lock<std::mutex> lock(mtx);
        auto it = connections.find(sock.getFd());
        if(it != connections.end()){
            Connection *conn = connections[sock.getFd()];
            connections.erase(sock.getFd());
            delete conn;
        }
    }  
}