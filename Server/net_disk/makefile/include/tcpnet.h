/*************************************************************************
	> File Name: TcpNet.h
	> Author: 
	> Mail: 
	> Created Time: Sat 15 Dec 2018 10:31:41 PM CST
 ************************************************************************/

#ifndef _TCPNET_H
#define _TCPNET_H
#include<iostream>
#include<string>
#include<cstring>
using namespace std;
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<pthread.h>
#include"packdef.h"

struct READ_BUFFER
{
    char        m_szbuf[MAX_BUFFER_SIZE];
    unsigned    m_len;
    READ_BUFFER(){
        this->reset();
    }
    void    reset(){
        bzero(m_szbuf,MAX_BUFFER_SIZE);
        m_len = 0;
    }
};

class TcpMediator;
class TcpNet
{
private:
    int             m_listener;
    int             m_epollfd;
    volatile int    m_epollListenNum;
    TcpMediator*    m_tcpMediator;
    bool            m_IsInitRecv;
public:
    TcpNet();
    ~TcpNet();
private:
    void    set_nonblock(int sockfd);
    bool    init_net(unsigned short port);
    static void*   ThreadProc(void* pvoid);
public:
    bool    open(unsigned short port);
    bool    send(int sockfd,char* szbuf,unsigned len);
    bool    recv(int sockfd,char* szbuf,unsigned len);
};

#endif
