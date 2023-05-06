#pragma once

class InetAddress;
class Socket
{
private:
    int m_fd;
public:
    Socket(/* args */);
    Socket(int _fd);
    Socket& operator=(int _fd);
    
    ~Socket();

    void bind(InetAddress& addr);
    void listen();
    int setNonBlocking();
    int accept(InetAddress& addr);
    void connect(InetAddress& addr);
    int getFd();
};


