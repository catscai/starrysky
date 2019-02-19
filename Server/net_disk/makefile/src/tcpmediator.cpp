/*************************************************************************
> File Name: tcpmediator.cpp
> Author: 
> Mail: 
> Created Time: Sun 16 Dec 2018 06:36:04 PM CST
************************************************************************/

#include "tcpmediator.h"
#include "CThreadPool.h"
#include "tcpnet.h"
#include <ctime>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "packdef.h"
#include "CIoFileStream.h"
#include <fstream>
#include "MySqlPool.h"
#include "MyDao.h"
using namespace std;
TcpMediator* mediator = NULL;
TcpMediator::TcpMediator(TcpNet* net):m_net(net),m_connpool(MySqlPool::PoolHandle())
{
    m_pool = new CThreadPool;
    m_pool->InitPool(10,1000,1000);
    m_IsReleaseFlag = false;
    mediator = this;
    m_connpool.InitPool("127.0.0.1","root","root","starrysky",100,500);
}

TcpMediator::~TcpMediator()
{
    if(m_IsReleaseFlag == false)
    {
        m_IsReleaseFlag = true;
        m_pool->Release();
        delete m_pool;
        m_pool = NULL;

        auto ite2 = m_userSessionMap.begin();
        while(ite2 != m_userSessionMap.end())
        {
            delete ite2->second;
            ite2->second = NULL;
            ++ite2;
        }
        m_userSessionMap.clear();
    }
}

static const Msg_Map g_MsgMap[]=
{
    {REGISTER_REQUEST_MSG,&TcpMediator::RegisterRequest},
    {LOGIN_REQUEST_MSG,&TcpMediator::LoginRequest},
    {FILELIST_REQUEST_MSG,&TcpMediator::FileListRequest},
    {UPLOAD_FILEREQUEST_MSG,&TcpMediator::UploadFileDataRequest},
    {UPLOAD_FILEFINISH_MSG,&TcpMediator::UploadFileFinishRequest},
    {UPLOAD_FILEHEAD_REQUEST_MSG,&TcpMediator::UploadFileHeadRequest},
    {ADD_NEW_DIRECTORY_REQUEST_MSG,&TcpMediator::AddNewDirectoryRequest},
    {DELETE_FILE_REQUEST_MSG,&TcpMediator::DeleteFileRequest},
    {DOWNLOAD_FILEHEAD_REQUEST_MSG,&TcpMediator::DownloadFileHeadRequest},
    {DOWNLOAD_FILE_REQUEST_MSG,&TcpMediator::DownloadFileDataRequest},
    {UPLOAD_PAUSE_REQUEST_MSG,&TcpMediator::UploadPauesRequest},
    {DOWNLOAD_DELETE_REQUEST_MSG,&TcpMediator::DownloadDeleteRequest},
    {UPLOAD_DELETE_REQUEST_MSG,&TcpMediator::UploadDeleteRequest},
    {FRIEND_LIST_REQUEST_MSG,&TcpMediator::FriendListRequest},
    {FRIEND_SEARCH_REQUEST_MSG,&TcpMediator::FriendSearchRequest},
    {CHAT_SEND_MESSAGE_MSG,&TcpMediator::ReceiveUserMessage},
    {AGREE_ADD_FRIEND_RESPONSE_MSG,&TcpMediator::AgreeAddFrientResponse},
    {DELETE_FRIEND_MSG,&TcpMediator::DeleteFriendRequest},
    {REQUEST_MESSAGE_MSG,&TcpMediator::RequestMessage},
    {SHARE_FILE_REQUEST_MSG,&TcpMediator::ShareFileRequest},
    {SHARE_FILE_LIST_REQUEST_MSG,&TcpMediator::ShareFileListRequest},
    {SHARE_FILE_CANCEL_MSG,&TcpMediator::ShareFileDeleteRequest},
    {EXTRACT_SHARE_FILE_REQUEST_MSG,&TcpMediator::ExtractFileRequest},
    {SAVE_SHARE_FILE_REQUEST_MSG,&TcpMediator::SaveShareFileRequest},
    {UNLOCK_MAINDLG_REQUEST_MSG,&TcpMediator::UnlockRequest},
    {0,0}
};

class  MyTask : public CTask
{
    private:
    int                 m_sockfd;
    char                m_szbuf[MAX_BUFFER_SIZE];
    unsigned            m_len;
    FUN_Mediator           m_fun;
    public:
    MyTask(int fd,char* buf,unsigned len,FUN_Mediator f):m_fun(f){
        memcpy(m_szbuf,buf,len);
        m_len = len;
        m_sockfd = fd;
    }
    ~MyTask(){}
    virtual void working()
    {
        (mediator->*m_fun)(m_sockfd,m_szbuf,m_len);
    }
};


bool   TcpMediator::deal(int sockfd,char* szbuf,unsigned len)
{
    int* type = (int*)(szbuf + 4);
    int i = 0;
    while(true)
    {
        if(g_MsgMap[i].m_type == *type)
        {
            MyTask* t = new MyTask(sockfd,szbuf,len,g_MsgMap[i].m_fun);
            m_pool->AddTask(t);
            break;
        }else if(g_MsgMap[i].m_type == 0)
        break;
        ++i;
    }
    return true;
}

bool    TcpMediator::send(int sockfd,char* szbuf,unsigned len)
{
    return m_net->send(sockfd,szbuf,len);
}

bool    TcpMediator::RegisterRequest(int sockfd,char* szbuf,unsigned len)
{
    RegisterRq* rq = (RegisterRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    RegisterRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = REGISTER_RESPONSE_MSG;
    rs.m_statuscode = REGISTER_OK;

    if(m_mysql->IsExistTheEmail(rq->m_email)){
        rs.m_statuscode = REGISTER_REGISTERED;
        this->send(sockfd,(char*)&rs,rs.m_len);
        m_connpool.ReleaseConn(m_mysql);
        return false;
    }
    string date = this->nowdate();
    m_mysql->InsertIntoUser(rq->m_email,rq->m_nickname,rq->m_pass,
                            date,1);
    this->send(sockfd,(char*)&rs,rs.m_len);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}


bool    TcpMediator::LoginRequest(int sockfd,char* szbuf,unsigned len)
{
    LoginRq* rq = (LoginRq*)szbuf;
    if(m_onlineUserMap.count(rq->m_email))
        return true;
    MyDao* m_mysql = m_connpool.GetConn();
    LoginRs rs;

    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = LOGIN_RESPONSE_MSG;
    rs.m_statuscode = LOGIN_OK;
    if(!m_mysql->IsExistTheEmail(rq->m_email))
    {
        rs.m_statuscode = LOGIN_UEXIST;
        this->send(sockfd,(char*)&rs,rs.m_len);
        m_connpool.ReleaseConn(m_mysql);
        return false;
    }
    vector<string> res;
    m_mysql->QueryUserByEmailAndPass(rq->m_email,rq->m_pass,res);
    if(res.empty()){
        rs.m_statuscode = LOGIN_PASSERROR;
        this->send(sockfd,(char*)&rs,rs.m_len);
        m_connpool.ReleaseConn(m_mysql);
        return false;
    }
    memcpy(rs.m_nickname,res[0].c_str(),res[0].size());
    rs.m_currcapacity = atoll(res[1].c_str());
    rs.m_capacity = atoll(res[2].c_str());
    int vipclass = m_mysql->VipClassByEmail(rq->m_email);
    rs.m_vipclass = vipclass;
    this->send(sockfd,(char*)&rs,rs.m_len);
    m_userSessionMap[sockfd]->m_email = rq->m_email;
    m_onlineUserMap[rq->m_email] = sockfd;
    string ss = homeStr + '/' + rq->m_email + '/';
    CIoFileStream io;
    io.createdir(ss);
    
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

string  TcpMediator::nowdate()
{
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20] = {0};
    sprintf(buf,"%d-%d-%d",1900 + ltm->tm_year,1 + ltm->tm_mon,
            ltm->tm_mday);
    return buf;
}
bool    TcpMediator::FileListRequest(int sockfd,char* szbuf,unsigned len)
{
    FileListRq* rq = (FileListRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    vector<string> res;
    m_mysql->QueryFileListByEmailAndDirectory(rq->m_email,rq->m_dir,res);
    unsigned length = res.size();
    FileListRs  info;
    memset(&info,0,sizeof(info));
    info.m_len = sizeof(info);
    info.m_type = FILELIST_RESPONSE_MSG;
    info.m_filecount = 0;
    memcpy(info.m_dir,rq->m_dir,strlen(rq->m_dir));
    int count = 0;
    int pos = 0;
    while(count < (length + 1)/2)
    {
        memcpy(info.m_filelist[count].m_name,res[pos].c_str(),res[pos].size());
        info.m_filelist[count].m_filesize = atoll(res[pos+1].c_str());
        pos = pos + 2;
        ++info.m_filecount;    
        ++count;
        if(count > 0 && count % FILENUM_IN_FILELIST == 0)
        {
            this->send(sockfd,(char*)&info,info.m_len);
            memset(&info,0,sizeof(info));
            info.m_len = sizeof(info);
            info.m_type = FILELIST_RESPONSE_MSG;
            info.m_filecount = 0;
            memcpy(info.m_dir,rq->m_dir,strlen(rq->m_dir));
        }
    }

    this->send(sockfd,(char*)&info,info.m_len);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::UploadFileDataRequest(int sockfd,
                                           char* szbuf,unsigned len)
{
    UploadFileDataRq* fdata = (UploadFileDataRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    UploadFileDataRs rs;
    auto sess = m_userSessionMap[sockfd];
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = UPLOAD_FILERESPONSE_MSG;
    memcpy(rs.m_md5code,fdata->m_md5code,strlen(fdata->m_md5code));
    string mark(fdata->m_md5code);
    UploadFile* info = NULL;
    if(sess->m_UploadFileMap.count(mark))
    {
        info = sess->m_UploadFileMap[mark];
    }else{
        //不存在的上传文件 错误
        m_connpool.ReleaseConn(m_mysql);
        return false;
    }
    info->m_output->write(fdata->m_filedata,fdata->m_datalen);
    rs.m_seek = fdata->m_seek + fdata->m_datalen;
    if(info->m_statuscode == FILE_TRANSFER)
        this->send(sockfd,(char*)&rs,rs.m_len);
    else if(info->m_statuscode == FILE_PAUSE){
        delete info;
        info = NULL;
        sess->m_UploadFileMap.erase(mark);
    }else if(info->m_statuscode == FILE_DELETE)
    {
        string path = info->m_path;
        delete info;
        info = NULL;
        sess->m_UploadFileMap.erase(mark);
        this->deletefile(path.c_str());
    }
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::UploadFileFinishRequest(int sockfd,char* szbuf,
                                             unsigned len)
{
    UploadFileEnd* info = (UploadFileEnd*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    auto sess = m_userSessionMap[sockfd];
    string mark(info->m_md5code);
    if(sess->m_UploadFileMap.count(mark))
    {
        delete sess->m_UploadFileMap[mark];
        sess->m_UploadFileMap[mark] = NULL;
        sess->m_UploadFileMap.erase(mark);
        string path = homeStr + '/' + info->m_email + info->m_dir + info->m_name;
        string fdirectory = info->m_dir;


        m_mysql->InsertFileInfoByEmail(info->m_email,info->m_md5code,fdirectory,0,info->m_name,info->m_filesize,path);
        m_mysql->ModifyUserCapacity(info->m_email,info->m_filesize,0);
    }else
    cout<<"upload file finish error"<<endl;

    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::UploadFileHeadRequest(int sockfd,char* szbuf,unsigned len)
{
    UploadFileHeadRq* head = (UploadFileHeadRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    UploadFileHeadRs rs;
    auto sess = m_userSessionMap[sockfd];
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = UPLOAD_FILEHEAD_RESPONSE_MSG;
    //1.根据md5查询文件是否已经存在在服务器端
    if(m_mysql->IsExistTheFile(head->m_md5code))
    {//2.若存在，根据信息直接插入该用户的表项中，向客户端发送已存在
     rs.m_statuscode = UPLOAD_RES_EXIST;
     vector<string> res;
     m_mysql->QueryFileInfoFromFiles(head->m_md5code,res);
     if(res.size() == 5)
     m_mysql->InsertFileInfoByEmail(head->m_email,head->m_md5code,head->m_dir,0,head->m_name,atoll(res[2].c_str()),res[3]);
    }else if(m_mysql->IsExistTheFileByUserAndMd5code(head->m_email,
                                                     head->m_md5code,head->m_dir)){
                                                         //该用户以存在相同文件名
                                                         rs.m_statuscode = UPLOAD_RES_NAMECONFLICT;
                                                     }else{
                                                         long long s = m_mysql->QueryUserResidueCapacity(head->m_email);
                                                         if(s < head->m_filesize)
                                                         {//容量不足
                                                          rs.m_statuscode = UPLOAD_RES_SPACE_LESS;
                                                         }else
                                                         {//3.若不存在向客户端发送可以开始文件传输的消息
                                                          rs.m_statuscode = UPLOAD_RES_START;
                                                          string path = homeStr + '/' + head->m_email + head->m_dir + head->m_name;
                                                          UploadFile* info = new UploadFile;
                                                          info->m_output = new fstream;
                                                          info->m_path = path;
                                                          info->m_output->open(path,ios::binary | ios::out);
                                                          if(info->m_output->bad())
                                                          {
                                                            m_connpool.ReleaseConn(m_mysql);
                                                            return false;
                                                          }
                                                          info->m_output->seekg(head->m_seek,ios::beg);
                                                          if(sess->m_UploadFileMap.count(head->m_md5code)){
                                                              auto& ite = sess->m_UploadFileMap[head->m_md5code];
                                                              delete ite;
                                                              ite = NULL;
                                                              sess->m_UploadFileMap.erase(head->m_md5code);
                                                          }

                                                          m_userSessionMap[sockfd]->m_UploadFileMap[head->m_md5code] = info;
                                                          info->m_statuscode = FILE_TRANSFER;
                                                         }
                                                     }
    memcpy(rs.m_md5code,head->m_md5code,strlen(head->m_md5code));
    this->send(sockfd,(char*)&rs,rs.m_len);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::AddNewDirectoryRequest(int sockfd,char* szbuf,unsigned len)
{
    AddNewDirectoryRq* rq = (AddNewDirectoryRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    AddNewDirectoryRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = ADD_NEW_DIRECTORY_RESPONSE_MSG;
    if(m_mysql->IsExistDirectoryByEmailAndDir(rq->m_email,rq->m_dir,rq->m_newdir))
    {//在该目录下已经存在同名目录
     rs.m_statuscode = ADD_DIRECTORY_RES_EXIST;
    }else{
        rs.m_statuscode = ADD_DIRECTORY_RES_OK;
        string dir = homeStr + '/' + rq->m_email + rq->m_dir + rq->m_newdir + '/';
        CIoFileStream io;
        io.createdir(dir);
        m_mysql->InsertFileInfoByEmail(rq->m_email,"",rq->m_dir,1,rq->m_newdir,0,dir);
    }

    this->send(sockfd,(char*)&rs,rs.m_len);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::DeleteFileRequest(int sockfd,char* szbuf,unsigned len)
{
    DeleteFileRq* rq = (DeleteFileRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    DeleteFileRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = DELETE_FILE_RESPONSE_MSG;
    rs.m_statuscode = DELETE_FILE_RES_OK;
    rs.m_filesize = 0;
    if(rq->m_filetype == FILE_TYPE){
        long long size = m_mysql->QueryUserFileSize(rq->m_email,rq->m_dir,rq->m_name); 
        m_mysql->DeleteFileByEmailAndDir(rq->m_email,rq->m_dir,rq->m_name);
        m_mysql->ModifyUserCapacity(rq->m_email,size,1);
    }else if(rq->m_filetype == DIRECTORY_TYPE){
        string dir = rq->m_dir;
        dir += rq->m_name;
        dir += '/';
        long long size = m_mysql->GetSizeFromDirectory(rq->m_email,dir);
        m_mysql->DeleteDirectoryByEmailAndDir(rq->m_email,rq->m_dir,rq->m_name);
        m_mysql->ModifyUserCapacity(rq->m_email,size,1);
    }
    this->send(sockfd,(char*)&rs,rs.m_len);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::DownloadFileHeadRequest(int sockfd,char* szbuf,unsigned len)
{
    DownloadFileHeadRq* rq = (DownloadFileHeadRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    DownloadFileHeadRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = DOWNLOAD_FILEHEAD_RESPONSE_MSG;
    rs.m_statuscode = DOWNLOAD_HEAD_RES_OK;

    if(m_mysql->IsExistTheFileByUserAndDir(rq->m_email,rq->m_dir,rq->m_name) == false)
    {    
        rs.m_statuscode = DOWNLOAD_HEAD_RES_ERROR;
        this->send(sockfd,(char*)&rs,rs.m_len);
        m_connpool.ReleaseConn(m_mysql);
        return true;
    }

    if(rq->m_filetype == FILE_TYPE)
    {
        vector<string> res;
        m_mysql->QueryFileMd5SizeByEmailAndDir(rq->m_email,rq->m_dir,rq->m_name,res);
        memcpy(rs.m_md5code,res[0].c_str(),res[0].size());
        rs.m_filesize = atoll(res[1].c_str());
        rs.m_seek = rq->m_seek;
        memcpy(rs.m_name,rq->m_name,strlen(rq->m_name));
        memcpy(rs.m_dir,rq->m_dir,strlen(rq->m_dir));
        this->send(sockfd,(char*)&rs,rs.m_len);
    }else
    {//若是文件夹类型就需要将该文件夹下的所有文件进行下载请求,直接转发给DownloadFileDataRequest消息处理
     vector<string> res;
     vector<string> ress;
     m_mysql->GetAllFilesInDirectory(rq->m_email,rq->m_dir,rq->m_name,res);
     DownloadFileHeadRs datarq;
     memset(&datarq,0,sizeof(datarq));
     datarq.m_len = sizeof(datarq);
     datarq.m_type = DOWNLOAD_FILEHEAD_RESPONSE_MSG;
     datarq.m_seek = 0;
     datarq.m_statuscode = DOWNLOAD_HEAD_RES_OK;
     string dir,name;
     for(int i = 0; i < res.size(); i++)
     {
         auto pos = res[i].find_last_of('/');
         dir = res[i].substr(0,pos + 1);
         name = res[i].substr(pos + 1);
         memcpy(datarq.m_name,name.c_str(),name.size());
         memcpy(datarq.m_dir,dir.c_str(),dir.size());

         m_mysql->QueryFileMd5SizeByEmailAndDir(rq->m_email,datarq.m_dir,datarq.m_name,ress);
         memcpy(datarq.m_md5code,ress[0].c_str(),ress[0].size());
         datarq.m_filesize = atoll(ress[1].c_str());
         this->send(sockfd,(char*)&datarq,datarq.m_len);
         memset(datarq.m_dir,0,DIR_SIZE);
         memset(datarq.m_name,0,FILENAME_LENGTH);
         memset(datarq.m_md5code,0,FILE_MD5CODE_SIZE);
         ress.clear();
     }
    }
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::DownloadFileDataRequest(int sockfd,char* szbuf,unsigned len)
{
    DownloadFileDataRq* rq = (DownloadFileDataRq*)szbuf;
    MyDao* m_mysql = m_connpool.GetConn();
    DownloadFileDataRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = DOWNLOAD_FILE_RESPONSE_MSG;
    rs.m_seek = rq->m_seek;
    auto& Map = m_userSessionMap[sockfd]->m_DownloadFileMap;
    string s(rq->m_dir);
    s += rq->m_name;
    DownloadFile* load = NULL;
    if(Map.count(s)){
        load = Map[s];
    }else{
        string p;
        m_mysql->QueryFilePathByEmailAndDir(rq->m_email,rq->m_dir,rq->m_name,p);
        load = new DownloadFile;
        load->m_input = new fstream;
        load->m_input->open(p,ios::binary | ios::in);
        Map[s] = load;
        load->m_statuscode = FILE_TRANSFER;
        load->m_input->seekg(rq->m_seek,ios::beg);
    }

    if(!load->m_input->eof()){
        load->m_input->read(rs.m_filedata,DOWNLOAD_FILEDATA_SIZE);
        rs.m_datalen = static_cast<int>(load->m_input->gcount());
    }

    else{
        load->m_input->close();
        delete load->m_input;
        load->m_input = NULL;
        delete load;
        load = NULL;
        m_userSessionMap[sockfd]->m_DownloadFileMap.erase(s);
        DownloadFileFinish finish;
        memset(&finish,0,sizeof(finish));
        finish.m_len = sizeof(finish);
        finish.m_type = DOWNLOAD_FILEFINISH_MSG;
        memcpy(finish.m_name,rq->m_name,strlen(rq->m_name));
        memcpy(finish.m_dir,rq->m_dir,strlen(rq->m_dir));
        this->send(sockfd,(char*)&finish,finish.m_len);
        m_connpool.ReleaseConn(m_mysql);
        return true;
    }
    memcpy(rs.m_name,rq->m_name,strlen(rq->m_name));
    memcpy(rs.m_dir,rq->m_dir,strlen(rq->m_dir));
    if(load->m_statuscode == FILE_TRANSFER)
        this->send(sockfd,(char*)&rs,rs.m_len);
    else if(load->m_statuscode == FILE_DELETE)
    {
        cout<<"xxxxxxxxxxxxxxxxxxx"<<endl;
    }
    //只有下载状态为正在下载时才返回数据/否则停止数据发送等待客户端的下一次请求
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::UploadPauesRequest(int sockfd,char* szbuf,unsigned len)
{
    auto sess = m_userSessionMap[sockfd];
    UploadPause* rq = (UploadPause*)szbuf;
    if(sess->m_UploadFileMap.count(rq->m_md5code))
    {
        auto info = sess->m_UploadFileMap[rq->m_md5code];
        info->m_statuscode = FILE_PAUSE;
    }
    return true;
}

bool    TcpMediator::DownloadDeleteRequest(int sockfd,char* szbuf,unsigned len)
{
    DownloadDelete* rq = (DownloadDelete*)szbuf;
    string s(rq->m_dir);
    s += rq->m_name;
    auto sess = m_userSessionMap[sockfd];
    if(sess->m_DownloadFileMap.count(s))
    {
        delete sess->m_DownloadFileMap[s];
        sess->m_DownloadFileMap[s];
        sess->m_DownloadFileMap.erase(s);
    }
    return true;
}

bool    TcpMediator::UploadDeleteRequest(int sockfd,char* szbuf,unsigned len)
{
    UploadDelete* rq = (UploadDelete*)szbuf;
    auto sess = m_userSessionMap[sockfd];
    string s(rq->m_md5code);
    if(sess->m_UploadFileMap.count(s))
    {
        auto& info = sess->m_UploadFileMap[s];
        string path = info->m_path;
        delete info;
        info = NULL;
        sess->m_UploadFileMap.erase(s);
        this->deletefile(path);
    }
    return true;
}

bool    TcpMediator::deletefile(const string& path)
{
    if(0 != remove(path.c_str()))
        return false;
    return true;
}

bool    TcpMediator::FriendListRequest(int sockfd,char* szbuf,unsigned len)
{
    FriendListRq* rq = (FriendListRq*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    vector<string> res;
    m_mysql->QueryFriendList(rq->m_email,res);
    FriendListRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = FRIEND_LIST_RESPONSE_MSG;
    rs.m_friendcount = 0;
    int count = 0;
    int time = res.size() / 2;
    for(int i = 0; i < time; i++)
    {
        memcpy(rs.m_friendlist[rs.m_friendcount].m_email,res[count].c_str(),res[count].size());
        memcpy(rs.m_friendlist[rs.m_friendcount].m_nickname,res[count + 1].c_str(),res[count + 1].size());
        count = count + 2;
        ++rs.m_friendcount;
        if(rs.m_friendcount == FRIEND_NUM_PER_PACKET)
        {
            this->send(sockfd,(char*)&rs,rs.m_len);
            rs.m_friendcount = 0;
        }
    }
    if(rs.m_friendcount > 0)
    {
        this->send(sockfd,(char*)&rs,rs.m_len);
    }
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::FriendSearchRequest(int sockfd,char* szbuf,unsigned len)
{
    FriendSearchRq* rq = (FriendSearchRq*)szbuf;
    auto m_mysql =m_connpool.GetConn();
    FriendSearchRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = FRIEND_SEARCH_RESPONSE_MSG;
    rs.m_statuscode = SEARCH_NONE;
    memcpy(rs.m_searchemail,rq->m_searchemail,strlen(rq->m_searchemail));
    string nickname;
    m_mysql->SearchUserNickName(rq->m_searchemail,nickname);
    if(nickname.size() > 0)
    {
        rs.m_statuscode = SEARCH_SUCCESS;
        memcpy(rs.m_searchnick,nickname.c_str(),nickname.size());
    }
    m_connpool.ReleaseConn(m_mysql);
    this->send(sockfd,(char*)&rs,rs.m_len);

    return true;
}

bool    TcpMediator::ReceiveUserMessage(int sockfd,char* szbuf,unsigned len)
{
    UserMessage* message = (UserMessage*)szbuf;
    if(m_onlineUserMap.count(message->m_receiver)){
        UserMessage s;
        memset(&s,0,sizeof(s));
        s.m_len = sizeof(s);
        s.m_type = CHAT_RECEIVE_MESSAGE_MSG;
        memcpy(s.m_sender,message->m_sender,strlen(message->m_sender));
        memcpy(s.m_receiver,message->m_receiver,strlen(message->m_receiver));
        memcpy(s.m_data,message->m_data,strlen(message->m_data));
        s.m_messagetype = message->m_messagetype;
        bool flag = this->send(m_onlineUserMap[message->m_receiver],(char*)&s,s.m_len);
    }else{
        auto m_mysql = m_connpool.GetConn();
        m_mysql->InsertChatMessage(message->m_receiver,message->m_sender,message->m_data,message->m_messagetype);
        m_connpool.ReleaseConn(m_mysql);
    }
    return true;
}

bool    TcpMediator::AgreeAddFrientResponse(int sockfd,char* szbuf,unsigned len)
{
    AgreeAddFriend* aa = (AgreeAddFriend*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    m_mysql->InsertFriend(aa->m_sender,aa->m_receiver);
    m_connpool.ReleaseConn(m_mysql);
    if(m_onlineUserMap.count(aa->m_receiver))
    {
        AgreeAddFriend aas;
        memset(&aas,0,sizeof(aas));
        aas.m_len = sizeof(aas);
        aas.m_type = aa->m_type;
        memcpy(aas.m_sender,aa->m_sender,strlen(aa->m_sender));
        memcpy(aas.m_receiver,aa->m_receiver,strlen(aa->m_receiver));
        memcpy(aas.m_sendernick,aa->m_sendernick,strlen(aa->m_sendernick));
        this->send(m_onlineUserMap[aa->m_receiver],(char*)&aas,aas.m_len);
    }
    return true;
}

bool    TcpMediator::DeleteFriendRequest(int sockfd,char* szbuf,unsigned len)
{
    DeleteFriend* df = (DeleteFriend*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    m_mysql->DeleteFriend(df->m_sender,df->m_receiver);
    m_connpool.ReleaseConn(m_mysql);

    if(m_onlineUserMap.count(df->m_receiver))
    {//如果删除用户在线则发送删除信息
        DeleteFriend ddf;
        memset(&ddf,0,sizeof(ddf));
        ddf.m_len = sizeof(ddf);
        ddf.m_type = df->m_type;
        memcpy(ddf.m_sender,df->m_sender,strlen(df->m_sender));
        memcpy(ddf.m_receiver,df->m_receiver,strlen(df->m_receiver));

        this->send(m_onlineUserMap[df->m_receiver],(char*)&ddf,ddf.m_len);
    }
    return true;
}

bool    TcpMediator::RequestMessage(int sockfd,char* szbuf,unsigned len)
{
    RequestMessageRq* rq = (RequestMessageRq*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    //查询该用户是否有离线消息
    vector<string> usermess;
    m_mysql->QueryFriendMessage(rq->m_email,usermess);
    m_connpool.ReleaseConn(m_mysql);
    auto count = usermess.size() / 5;
    UserMessage message;
    memset(&message,0,sizeof(message));
    message.m_len = sizeof(message);
    message.m_type = CHAT_RECEIVE_MESSAGE_MSG;
    memcpy(message.m_receiver,rq->m_email,strlen(rq->m_email));
    int index = 0;
    for(int i = 0; i < count; i++)
    {
        memcpy(message.m_sender,usermess[index + 1].c_str(),usermess[index + 1].size());
        memcpy(message.m_data,usermess[index + 2].c_str(),usermess[index + 2].size());
        message.m_messagetype = atoi(usermess[index + 3].c_str());
        memcpy(message.m_date,usermess[index + 4].c_str(),usermess[index + 4].size());
        index = index + 5;
        this->send(sockfd,(char*)&message,message.m_len);
        memset(message.m_sender,0,EMAIL_SIZE);
        memset(message.m_data,0,CHAT_MESSAGE_LENGTH);
    }
    m_mysql = m_connpool.GetConn();
    m_mysql->DeleteFriendMessage(rq->m_email);
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::ShareFileRequest(int sockfd,char* szbuf,unsigned len)
{
    ShareFileRq* rq = (ShareFileRq*)szbuf;
    ShareFileRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = SHARE_FILE_RESPONSE_MSG;
    memcpy(rs.m_dir,rq->m_dir,strlen(rq->m_dir));
    memcpy(rs.m_name,rq->m_name,strlen(rq->m_name));
    auto m_mysql = m_connpool.GetConn();
    if(m_mysql->IsExistShareFileByUser(rq->m_email,rq->m_dir,rq->m_name)){
        rs.m_statuscode = SHARE_EXIST;
        m_connpool.ReleaseConn(m_mysql);
        this->send(sockfd,(char*)&rs,rs.m_len);
        return true;
    }
    string code = this->getCode();
    while(m_mysql->IsExistGetCodeByUser(rq->m_email,code)){
        code = this->getCode();
    }
    
    m_mysql->InsertShareFile(rq->m_email,rq->m_dir,rq->m_name,code,rq->m_days);
    m_connpool.ReleaseConn(m_mysql);
    rs.m_statuscode = SHARE_SUCCESS;
    memcpy(rs.m_getcode,code.c_str(),code.size());

    this->send(sockfd,(char*)&rs,rs.m_len);
    return true;
}

string  TcpMediator::getCode()
{
    srand((int)time(0));
    const string table="0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string code;
    for(int i = 0; i < GETCODE_BIT_NUM;i++)
    {
        code += table[rand()%62];
    }
    return code;
}

bool    TcpMediator::ShareFileListRequest(int sockfd,char* szbuf,unsigned len)
{
    ShareFileListRq* rq = (ShareFileListRq*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    vector<string> res;
    m_mysql->QueryShareFileList(rq->m_email,res);
    m_connpool.ReleaseConn(m_mysql);
    if(res.size() > 0)
    {
        int count = res.size() / 6;
        int index = 0;
        ShareFileListRs rs;
        memset(&rs,0,sizeof(rs));
        rs.m_len = sizeof(rs);
        rs.m_type = SHARE_FILE_LIST_RESPONSE_MSG;
        rs.m_sharelistcount = 0;
        for(int i = 0; i < count; i++)
        {
            memcpy(rs.m_sharelist[rs.m_sharelistcount].m_dir,res[index].c_str(),res[index].size());
            memcpy(rs.m_sharelist[rs.m_sharelistcount].m_name,res[index + 1].c_str(),res[index + 1].size());
            rs.m_sharelist[rs.m_sharelistcount].m_savetime = atoi(res[index + 2].c_str());
            memcpy(rs.m_sharelist[rs.m_sharelistcount].m_starttime,res[index + 3].c_str(),res[index + 3].size());
            rs.m_sharelist[rs.m_sharelistcount].m_days = atoi(res[index + 4].c_str());
            memcpy(rs.m_sharelist[rs.m_sharelistcount].m_getcode,res[index + 5].c_str(),res[index + 5].size());
            ++rs.m_sharelistcount;
            index = index + 6;
            if(rs.m_sharelistcount == SHARE_FILE_LIST_LENGTH)
            {
                this->send(sockfd,(char*)&rs,rs.m_len);
                rs.m_sharelistcount = 0;
                memset(rs.m_sharelist,0,sizeof(rs.m_sharelist));
            }
        }

        if(rs.m_sharelistcount > 0)
        {
            this->send(sockfd,(char*)&rs,rs.m_len);
        }
    }
    return true;
}

bool    TcpMediator::ShareFileDeleteRequest(int sockfd,char* szbuf,unsigned len)
{
    ShareFileDelete* sfd = (ShareFileDelete*)szbuf;
    auto m_mysql = m_connpool.GetConn();
    m_mysql->DeleteShareFile(sfd->m_email,sfd->m_dir,sfd->m_name); 
    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::ExtractFileRequest(int sockfd,char* szbuf,unsigned len)
{
    ExtractFileRq* rq = (ExtractFileRq*)szbuf;
    ExtractFileRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = EXTRACT_SHARE_FILE_RESPONSE_MSG;
    rs.m_statuscode = EXTRACT_OK;

    auto m_mysql = m_connpool.GetConn();
    if(m_mysql->IsExistShareFile(rq->m_otheremail,rq->m_getcode) == false)
    {
        rs.m_statuscode = EXTRACT_ERROR;
        m_connpool.ReleaseConn(m_mysql);
        this->send(sockfd,(char*)&rs,rs.m_len);
        return true;
    }
    vector<string> res;
    m_mysql->QueryShareFileInfo(rq->m_otheremail,rq->m_getcode,res);
    m_connpool.ReleaseConn(m_mysql);
    rs.m_filetype = (atoi(res[0].c_str()) == 0) ? FILE_TYPE : DIRECTORY_TYPE;
    memcpy(rs.m_dir,res[1].c_str(),res[1].size());
    memcpy(rs.m_name,res[2].c_str(),res[2].size());

    this->send(sockfd,(char*)&rs,rs.m_len);
    return 0;
}

bool    TcpMediator::SaveShareFileRequest(int sockfd,char* szbuf,unsigned len)
{
    SaveShareFileRq* rq = (SaveShareFileRq*)szbuf;
    SaveShareFileRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = SAVE_SHARE_FILE_RESPONSE_MSG;
    rs.m_statuscode = SAVE_SUCCESS;

    string dir = rq->m_mydir;
    string name;
    if(dir != "/")
    {
        auto pos = dir.find_last_of('/');
        name = dir.substr(pos + 1);
        dir = dir.substr(0,pos + 1);
    }
    auto m_mysql = m_connpool.GetConn();
    if(dir != "/" && m_mysql->IsExistDirectoryByEmailAndDir(rq->m_email,dir,name) == false){
        rs.m_statuscode = SAVE_DIR_UNEXIST;
    }else{
        long long size = m_mysql->QueryUserFileSize(rq->m_otheremail,rq->m_dir,rq->m_name);
        if(size == 0){
            string dir = rq->m_dir;
            dir += rq->m_name;
            dir += '/';
            size = m_mysql->GetSizeFromDirectory(rq->m_otheremail,dir);
        }
        long long cap = m_mysql->QueryUserResidueCapacity(rq->m_email);
        if(cap < size)
        {//容量不足不能保存
            rs.m_statuscode = SAVE_CAPACITY_LESS;
        }
        else{
            string dd = rq->m_mydir;
            if(dd != "/")
                dd += '/';
            m_mysql->SaveShareFile(rq->m_email,rq->m_otheremail,rq->m_dir,rq->m_name,dd); 
            m_mysql->ModifyUserCapacity(rq->m_email,size,0);
        }
    }
    this->send(sockfd,(char*)&rs,rs.m_len);

    m_connpool.ReleaseConn(m_mysql);
    return true;
}

bool    TcpMediator::UnlockRequest(int sockfd,char* szbuf,unsigned len)
{
    UnlockRq* rq = (UnlockRq*)szbuf;
    UnlockRs rs;
    memset(&rs,0,sizeof(rs));
    rs.m_len = sizeof(rs);
    rs.m_type = UNLOCK_MAINDLG_RESPONSE_MSG;
    rs.m_statuscode = UNLOCK_SUCCESS;
    auto m_mysql = m_connpool.GetConn();
    vector<string> res;
    m_mysql->QueryUserByEmailAndPass(rq->m_email,rq->m_pass,res);
    if(res.empty())
        rs.m_statuscode = UNLOCK_FAILED;
    m_connpool.ReleaseConn(m_mysql);
    this->send(sockfd,(char*)&rs,rs.m_len);
    return true;
}
