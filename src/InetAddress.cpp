#include"InetAddress.h"
#include<stdio.h>
InetAddress::InetAddress(/* args */):m_addr_len(sizeof (m_addr)){
    bzero(&m_addr, sizeof(m_addr));
    m_addr_len = sizeof(m_addr);
    //printf("InetAddress default!");
}

InetAddress::InetAddress(const char* ip, int port):m_addr_len(sizeof (m_addr)){
    bzero(&m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = inet_addr(ip);
    m_addr.sin_port = htons(port);
}

InetAddress::~InetAddress()
{
}