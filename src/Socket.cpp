#include"Socket.h"
#include<arpa/inet.h>
#include"util.h"
#include<sys/socket.h>
#include<fcntl.h>
#include<unistd.h>
#include"InetAddress.h"

Socket::Socket(/* args */):m_fd(-1){
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(m_fd == -1, "socket create err\n");
}

Socket::Socket(int _fd):m_fd(_fd){
}

Socket& Socket::operator=(int _fd){
    m_fd = _fd;
}

Socket::~Socket(){
    printf("~Socket() fd : %d\n", m_fd);
    if(m_fd >= 0){
        close(m_fd);
        m_fd = -1;
    }    
}

void Socket::bind(InetAddress& addr){
    errif(::bind(m_fd, (sockaddr*)&addr.m_addr, addr.m_addr_len) == -1, "bind err\n");
}

void Socket::listen(){
    errif(::listen(m_fd, SOMAXCONN) == -1, "listen err\n");
}

int Socket::setNonBlocking(){
    int old_option = fcntl(m_fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(m_fd, F_SETFL, new_option);
    return old_option;
}

int Socket::accept(InetAddress& addr){
    int clifd = ::accept(m_fd, (sockaddr *)&addr.m_addr, &addr.m_addr_len);
    errif(clifd == -1, "accept sock err\n");
    return clifd;
}

void Socket::connect(InetAddress& addr){
    errif(::connect(m_fd, (sockaddr*)&addr.m_addr, addr.m_addr_len) == -1, "connect server err\n");
    printf("connect success\n");
}

int Socket::getFd(){
    return m_fd;
}
