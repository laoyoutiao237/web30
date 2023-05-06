#include"Server.h"
#include"EventLoop.h"
#include"InetAddress.h"
#include"Channel.h"
#include<map>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#define READ_BUFFER 1024


Server::Server(EventLoop& _loop, const char* ch):loop(_loop), serv_sock(), cli_sock(-1),users(){
    InetAddress serv_addr(ch, 9999);
    serv_sock.bind(serv_addr);
    serv_sock.listen();
    serv_sock.setNonBlocking();
    servfd = serv_sock.getFd();

    servChannel = new Channel(loop, servfd);
    servChannel->enableReading();
    //绑定事件的回调函数
    std::function<void()> cb = std::bind(&Server::newConnection, this);
    servChannel->setCallback(cb);
}

Server::~Server()
{
    delete servChannel;
}

void Server::handleReadEvent(int handle_fd){
    char buf[READ_BUFFER];              
    while(true){
        bzero(&buf, sizeof buf);
        ssize_t read_bytes = read(handle_fd, buf, sizeof(buf));
        if(read_bytes > 0){
            printf("class receive message from client fd %d: %s\n", handle_fd, buf);
            write(handle_fd, buf, strlen(buf) + 1);
        }else if(read_bytes == 0){
            printf("client fd %d close\n", handle_fd);
            close(handle_fd);
            //维护users
            
            delete this->users[handle_fd];
            this->users.erase(handle_fd);
            break;
        }else if(read_bytes == -1){
            if(errno == EINTR){//信号打断
                printf("continue read!\n");
                continue;
            }else if((errno == EAGAIN) || (errno == EWOULDBLOCK)){//没有可读数据 或 描述符为 阻塞
                printf("finish reading once, errno: %d\n", errno);
                break;
            }
            perror("read err\n");
            break;
        }
    }
}

void Server::newConnection(){
    InetAddress cli_addr;
    cli_sock = serv_sock.accept(cli_addr);
    printf("new client fd is %d!  IP: %s, Port: %d\n", cli_sock.getFd(), inet_ntoa(cli_addr.m_addr.sin_addr), ntohs(cli_addr.m_addr.sin_port));
    cli_sock.setNonBlocking();
    
    Channel*  cliChannel = new Channel(loop, cli_sock.getFd());
    this->users[cli_sock.getFd()] = cliChannel;
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, cli_sock.getFd());
    cliChannel->setCallback(cb);
    cliChannel->enableReading();
}