/*************************************************************************
	> File Name: tcpmediator.h
	> Author: 
	> Mail: 
	> Created Time: Sun 16 Dec 2018 05:45:42 PM CST
 ************************************************************************/

#ifndef _TCPMEDIATOR_H
#define _TCPMEDIATOR_H
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "CIoFileStream.h"
using namespace std;
#include "packdef.h"
class TcpMediator;
typedef bool (TcpMediator::*FUN_Mediator)(int,char*,unsigned);

struct Msg_Map
{
    MsgType         m_type;
    FUN_Mediator    m_fun;
};
#define USER_BUFF_SIZE 10240
struct UserBuffer
{
    char        m_szbuf[USER_BUFF_SIZE];
    unsigned    m_len;
    unsigned    m_packsize;
    unordered_map<string,UploadFile*> m_UploadFileMap;
    unordered_map<string,DownloadFile*>       m_DownloadFileMap;
    string      m_email;
    UserBuffer(){
        this->reset();
    }
    ~UserBuffer(){
        auto ite = m_UploadFileMap.begin();
        while(ite != m_UploadFileMap.end())
        {
            if(ite->second){
                delete ite->second;
                ite->second = NULL;
            }
            ++ite;
        }
        m_UploadFileMap.clear();

        auto ite2 = m_DownloadFileMap.begin();
        while(ite2 != m_DownloadFileMap.end())
        {
            if(ite2->second){
                delete ite2->second;
                ite2->second = NULL;
            }
            ++ite2;
        }
        m_DownloadFileMap.clear();
    }
    void reset(){
        memset(m_szbuf,0,USER_BUFF_SIZE);
        m_len = 0;
        m_packsize = 0;
    }
};
class TcpNet;
class CThreadPool;
class MySqlPool;
class TcpMediator
{
private:
    MySqlPool&      m_connpool;
    TcpNet*         m_net;
    CThreadPool*    m_pool;
    bool            m_IsReleaseFlag;
public:
    unordered_map<int,UserBuffer*>          m_userSessionMap;
    unordered_map<string,int>               m_onlineUserMap;
public:
    TcpMediator(TcpNet* net);
    ~TcpMediator();
public:
    bool    deal(int sockfd,char* szbuf,unsigned len);
    bool    send(int sockfd,char* szbuf,unsigned len);
    string  nowdate();
    bool    deletefile(const string& path);
    string  getCode();
public:
    bool    RegisterRequest(int sockfd,char* szbuf,unsigned len);
    bool    LoginRequest(int sockfd,char* szbuf,unsigned len);
    bool    FileListRequest(int sockfd,char* szbuf,unsigned len);
    bool    UploadFileDataRequest(int sockfd,char* szbuf,unsigned len);
    bool    UploadFileFinishRequest(int sockfd,char* szbuf,unsigned len);
    bool    UploadFileHeadRequest(int sockfs,char* szbuf,unsigned len);
    bool    AddNewDirectoryRequest(int sockfd,char* szbuf,unsigned len);
    bool    DeleteFileRequest(int sockfd,char* szbuf,unsigned len);
    bool    DownloadFileHeadRequest(int sockfd,char* szbuf,unsigned len);
    bool    DownloadFileDataRequest(int sockfd,char* szbuf,unsigned len);
    bool    UploadPauesRequest(int sockfd,char* szbuf,unsigned len);
    bool    DownloadDeleteRequest(int sockfd,char* szbuf,unsigned len);
    bool    UploadDeleteRequest(int sockfd,char* szbuf,unsigned len);
    bool    FriendListRequest(int sockfd,char* szbuf,unsigned len);
    bool    FriendSearchRequest(int sockfd,char* szbuf,unsigned len);
    bool    ReceiveUserMessage(int sockfd,char* szbuf,unsigned len);
    bool    AgreeAddFrientResponse(int sockfd,char* szbuf,unsigned len);
    bool    DeleteFriendRequest(int sockfd,char* szbuf,unsigned len);
    bool    RequestMessage(int sockfd,char* szbuf,unsigned len);
    bool    ShareFileRequest(int sockfd ,char* szbuf,unsigned len);
    bool    ShareFileListRequest(int sockfd,char* szbuf,unsigned len);
    bool    ShareFileDeleteRequest(int sockfd,char* szbuf,unsigned len);
    bool    ExtractFileRequest(int sockfd,char* szbuf,unsigned len);
    bool    SaveShareFileRequest(int sockfd,char* szbuf,unsigned len);
    bool    UnlockRequest(int sockfd,char* szbuf,unsigned len);
};
#endif
