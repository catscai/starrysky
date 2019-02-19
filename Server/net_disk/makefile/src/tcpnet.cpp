/*************************************************************************
  > File Name: TcpNet.cpp
  > Author: 
  > Mail: 
  > Created Time: Sat 15 Dec 2018 10:35:18 PM CST
 ************************************************************************/

#include "tcpnet.h"
#include "tcpmediator.h"
TcpNet::TcpNet():m_listener(0),m_epollfd(0),m_epollListenNum(0),
    m_tcpMediator(0){
        m_tcpMediator = new TcpMediator(this);
        m_IsInitRecv = false;
    }

TcpNet::~TcpNet()
{
    delete m_tcpMediator;
    m_tcpMediator = NULL;
}


void    TcpNet::set_nonblock(int sockfd)
{
    int opts;
    opts = fcntl(sockfd,F_GETFL);
    if(opts < 0){
        cout<<"fcntl getfl error"<<endl;
        exit(1);
    }
    opts = opts | O_NONBLOCK;
    if(fcntl(sockfd,F_SETFL,opts) < 0){
        cout<<"fcntl set error"<<endl;
        exit(1);
    }
}

bool    TcpNet::init_net(unsigned short port)
{
    struct sockaddr_in TcpNetAddr;
    m_listener = socket(AF_INET,SOCK_STREAM,0);
    int on = 1;
    setsockopt( m_listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)  );
    if(m_listener == -1){
        cout<<"socket create failed"<<endl;
        return false;
    }
    bzero(&TcpNetAddr,sizeof(TcpNetAddr));
    TcpNetAddr.sin_family = AF_INET;
    inet_pton(AF_INET,"192.168.31.144",&TcpNetAddr.sin_addr);
    TcpNetAddr.sin_port = htons(port);
    if(-1 == bind(m_listener,(struct sockaddr*)&TcpNetAddr,sizeof(TcpNetAddr))){
        cout<<"bind socket failed"<<endl;
        return false;
    }
    if(-1 == listen(m_listener,20)){
        cout<<"listen socket error"<<endl;
        return false;
    }
    return true;   
}

void*   TcpNet::ThreadProc(void* pvoid)
{
    TcpNet* pthis = (TcpNet*)pvoid;
    struct epoll_event ep;
    int sockfd = 0;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    while(true)
    {
        clientLen = sizeof(clientAddr);
        bzero(&clientAddr,clientLen);
        sockfd = accept(pthis->m_listener,(struct sockaddr*)&clientAddr,&clientLen);
        if(sockfd < 0)
        {
            cout<<"accept socket error"<<endl;
            exit(1);
        }
        int recsize = 20480;
        if(0 != setsockopt(sockfd,SOL_SOCKET,SO_RCVBUF,(void*)&recsize,sizeof(recsize)))
        {
            cout<<"setsockopt occur error"<<endl;
        }
        pthis->set_nonblock(sockfd);
        char* ip = inet_ntoa(clientAddr.sin_addr);
        cout<<"connect success "<<ip<<endl;
        ep.data.fd = sockfd;
        ep.events = EPOLLIN | EPOLLET;
        epoll_ctl(pthis->m_epollfd,EPOLL_CTL_ADD,sockfd,&ep);
        ++pthis->m_epollListenNum;
    }  
}


bool    TcpNet::open(unsigned short port)
{
    if(false == init_net(port))
        return false;
    struct epoll_event ep,events[200];
    m_epollfd = epoll_create(200);
    pthread_t tid;
    pthread_create(&tid,NULL,&ThreadProc,this);
    int  fds = 0;
    int  clientfd = 0;
    int  readBytes = 0;
    struct sockaddr_in clientAddr;
    READ_BUFFER buf,result;
    socklen_t clientLen = 0;
    unsigned size = 4;
    unsigned pack_len = 0;
    bool    IsInterrupt = false;
    while(true)
    {
        fds = epoll_wait(m_epollfd,events,this->m_epollListenNum,-1);
        for(int i = 0; i < fds; i++)
        {

            if(events[i].events & EPOLLIN)
            {
                if((clientfd = events[i].data.fd) < 0)
                    continue;

                UserBuffer* userbuff = NULL;
                if(m_tcpMediator->m_userSessionMap.count(clientfd)){
                    userbuff = m_tcpMediator->m_userSessionMap[clientfd];
                }else{
                    userbuff = new UserBuffer;
                    m_tcpMediator->m_userSessionMap[clientfd] = userbuff;
                }
                while(1)
                {
                    bool    IsExit = false;
                    while(userbuff->m_len < size){
                        readBytes = read(clientfd,userbuff->m_szbuf + userbuff->m_len,size);  
                        if(readBytes < 0){
                            if(errno == EWOULDBLOCK){
                                IsExit = true;
                                break;
                            }
                            else if(errno == EAGAIN){
                                continue;
                            }
                            else{
                                epoll_ctl(m_epollfd,EPOLL_CTL_DEL,clientfd,NULL);
                                close(clientfd);
                                auto email = m_tcpMediator->m_userSessionMap[clientfd]->m_email;
                                if(m_tcpMediator->m_onlineUserMap.count(email))
                                    m_tcpMediator->m_onlineUserMap.erase(email);
                                delete m_tcpMediator->m_userSessionMap[clientfd];
                                m_tcpMediator->m_userSessionMap[clientfd] = NULL;
                                m_tcpMediator->m_userSessionMap.erase(clientfd);
                                --this->m_epollListenNum;
                                clientfd = -1;
                                IsExit = true;
                                break;
                            }
                        }else if(readBytes == 0){
                            IsExit = true;
                            break;
                        }
                        userbuff->m_len += readBytes;
                    }
                    if(IsExit)
                        break;
                    userbuff->m_packsize = *((unsigned*)userbuff->m_szbuf);
                    pack_len = userbuff->m_packsize;
                    pack_len -= userbuff->m_len;
                    while(pack_len > 0)
                    {
                        readBytes = read(clientfd,userbuff->m_szbuf + userbuff->m_len,pack_len);
                        if(readBytes < 0){
                            if(errno == EWOULDBLOCK){
                                IsExit = true;
                                break;
                            }
                            else if(errno == EAGAIN){
                                continue;
                            }
                            else{
                                IsExit = true;
                                epoll_ctl(m_epollfd,EPOLL_CTL_DEL,clientfd,NULL);
                                close(clientfd);
                                auto email = m_tcpMediator->m_userSessionMap[clientfd]->m_email;
                                if(m_tcpMediator->m_onlineUserMap.count(email))
                                    m_tcpMediator->m_onlineUserMap.erase(email);
                                delete m_tcpMediator->m_userSessionMap[clientfd];
                                m_tcpMediator->m_userSessionMap[clientfd] = NULL;
                                m_tcpMediator->m_userSessionMap.erase(clientfd);
                                clientfd = -1;
                                --this->m_epollListenNum;
                                break;
                            }
                        }else if(readBytes == 0){
                            IsExit = true;
                            break;
                        }
                        userbuff->m_len += readBytes;
                        pack_len -= readBytes;
                    }
                    if(pack_len == 0 && clientfd != -1){//接受到一个整包
                        m_tcpMediator->deal(clientfd,userbuff->m_szbuf,userbuff->m_packsize);
                        userbuff->reset();
                    }

                    if(IsExit){//读到缓冲区末尾或有错误发生
                        break;
                    }
                }
            }  
        }
    }
    close(m_listener);
    return true;
}

bool    TcpNet::send(int sockfd,char* szbuf,unsigned len)
{
    if(-1 == write(sockfd,szbuf,len))
    {
        return false;
    }
    return true;
}

bool    TcpNet::recv(int sockfd,char* szbuf,unsigned len)
{
    return true;
}









