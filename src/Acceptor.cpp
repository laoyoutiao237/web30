#include"Acceptor.h"
#include"EventLoop.h"

#include"Epoll.h"
#include"Channel.h"

Acceptor::Acceptor(EventLoop &_loop, const char * ch):loop(_loop), serv_sock(), cli_sock(-1), 
serv_addr(ch, 9999){
    serv_sock.bind(serv_addr);
    serv_sock.listen();
    //serv_sock.setNonBlocking();

    acceptChannel = new Channel(loop, serv_sock.getFd());   
    //绑定事件的回调函数
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setReadCallback(cb);
    acceptChannel->enableRead();
    acceptChannel->setUseThreadPool(false);
}

Acceptor::~Acceptor(){
    cli_sock = -1;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress cli_addr;
    cli_sock = serv_sock.accept(cli_addr);
    printf("new client fd is %d!  IP: %s, Port: %d\n", cli_sock.getFd(), inet_ntoa(cli_addr.m_addr.sin_addr), ntohs(cli_addr.m_addr.sin_port));
    cli_sock.setNonBlocking();
    newConnectionCallback(cli_sock);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket &)> _cb){
    newConnectionCallback = _cb;
}