#pragma once
#include<arpa/inet.h>
#include<cstring>
class InetAddress
{
public:
    struct sockaddr_in m_addr;
    socklen_t m_addr_len;

    InetAddress(/* args */);
    InetAddress(const char* ip, int port);
    ~InetAddress();
};


