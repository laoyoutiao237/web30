/***************************************
 *  author: laoyotiao
 * 
 * 
 * 
 * 
 *************************************/
#include"Channel.h"
#include"Connection.h"
#include"Socket.h"
#include<unistd.h>
#include<string.h>
#include"Buffer.h"
#include"util.h"

#define READ_BUFFER 5

Connection::Connection(EventLoop &_loop, const Socket& sock):loop(_loop), cli_sock(sock), 
channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr){
    channel = new Channel(loop, cli_sock.getFd());
    channel->enableRead();
    channel->useET();
  
    std::function<void()> cb = std::bind(&Connection::echo, this);
    channel->setReadCallback(cb);
    channel->setUseThreadPool(true);
    
    readBuffer = new Buffer();
}

Connection::~Connection(){
    delete channel;
    delete readBuffer;
    delete inBuffer;
}

void Connection::echo(){
    int clifd = cli_sock.getFd();
    char buf[READ_BUFFER];              
    while(true){
        bzero(&buf, sizeof buf);
        ssize_t read_bytes = read(clifd, buf, sizeof(buf));
        if(read_bytes > 0){
            readBuffer->append(buf, read_bytes);
        }else if(read_bytes == -1 && errno == EINTR){
            printf("continue read!\n");
            continue;
               
        }else if(read_bytes == -1 && (errno == EAGAIN) || (errno == EWOULDBLOCK)){//没有可读数据 或 描述符为 阻塞
            //printf("finish reading once, errno: %d\n", errno);
            printf("receive message from client fd %d: %s\n", clifd, readBuffer->c_str());
            //errif(write(clifd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write err");
            send(clifd);
            readBuffer->clear(); //此处清空貌似不合适 send 不一定发完
            break;
        }else if(read_bytes == 0){
            printf("client fd %d close\n", clifd);
            deleteConnectionCallback(cli_sock); //多线程会有bug
            break;
        }else{
            perror("Connection reset by peer\n");
            deleteConnectionCallback(cli_sock); //多线程会有bug
            break;
        }
    }
    
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket &sock)> _cb){
    deleteConnectionCallback = _cb;
}

void Connection::send(int sockfd){
    char buf[readBuffer->size()];
    strcpy(buf, readBuffer->c_str());
    ssize_t data_size = readBuffer->size();
    ssize_t data_left = data_size;
    while(data_left > 0){
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if(bytes_write == -1 && errno == EAGAIN){
            break; 
        }
        data_left -= bytes_write;
    }
}