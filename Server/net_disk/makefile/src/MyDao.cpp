/*************************************************************************
	> File Name: MyDao.cpp
	> Author: 
	> Mail: 
	> Created Time: ven. 21 déc. 2018 11:02:18 CST
 ************************************************************************/

#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
#include "MyDao.h"
#include "CMySql.h"
MyDao::MyDao(const string& host,const string& user,const string& pass,
        const string& db_name)
{
    m_mysql = new CMySql();
    m_mysql->initDB(host,user,pass,db_name);
}

MyDao::~MyDao()
{
    if(m_mysql)
    {
        delete m_mysql;
        m_mysql = NULL;
    }
}

bool    MyDao::IsExistTheEmail(const string& email)
{
    vector<string> res;
    string sql = "select count(*) from user where email='" + email
                    + "'";
    m_mysql->QuerySql(sql,1,res);
    if(atoll(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::IsExistTheFile(const string& md5code)
{
    return this->IsExistFileFromFiles(md5code);
}

bool    MyDao::QueryUserByEmail(const string& email,vector<string>& res)
{
    string sql = "select * from user where email='" + email + "'";
    m_mysql->QuerySql(sql,7,res);
    return true;
}

bool    MyDao::QueryFileListByEmailAndDirectory(const string& email,
            const string& dir,vector<string>& res)
{
    char buf[300] = {0};
    sprintf(buf,"select fname,size from userfile where email='%s' and fdirectory='%s'",email.c_str(),dir.c_str());
    m_mysql->QuerySql(buf,2,res);
    return true;
}

bool    MyDao::InsertIntoUser(const string& e,const string& n,
        const string& p,const string& date,int isEffect)
{
    char buf[500] = {0};
    sprintf(buf,"insert into user values('%s','%s','%s','%s',%d,%lld,%lld)",
            e.c_str(),n.c_str(),p.c_str(),date.c_str(),isEffect,0,2048000000000);
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::QueryUserByEmailAndPass(const string& email,const string& pass, vector<string>& res)
{
    char buf[500] = {0};
    sprintf(buf,"select nickname,currcapacity,capacity from user where email='%s' and passwd='%s'",email.c_str(),
           pass.c_str());
    m_mysql->QuerySql(buf,3,res);
    return true;
}


int    MyDao::VipClassByEmail(const string& email)
{
    vector<string> res;
    char buf[200] = {0};
    sprintf(buf,"select vipclass,iseffect from vip where vipemail='%s'",email.c_str());
    m_mysql->QuerySql(buf,2,res);
    if(res.empty()){
        return 0;
    }else{
        if(res[1] == "0")
            return 0;
        else{
            return atoi(res[0].c_str());
        }
    }
} 


bool    MyDao::InsertFileInfoByEmail(const string& email,const string&
    md5code,const string& fdirectory,int ftype,const string& fname,
    unsigned size,const string& path)
{
    char buf[1000] = {0};
    sprintf(buf,"insert into userfile values('%s','%s','%s',\
           %d,'%s',%d,'%s')",email.c_str(),md5code.c_str(),
           fdirectory.c_str(),ftype,fname.c_str(),size,path.c_str());
    if(false == m_mysql->UpdateSql(buf))
    {
        return false;
    }
    if(ftype == 0 && this->IsExistFileFromFiles(md5code) == false){
        this->IntoFiles(md5code,fname,size,path);
    }
    return true;
}

long long    MyDao::QueryUserResidueCapacity(const string& email)
{
    vector<string> res;
    char sql[1000] = {0};
    sprintf(sql,"select currcapacity,capacity from user where email='%s'",email.c_str());
    m_mysql->QuerySql(sql,2,res);

    return atoll(res[1].c_str()) - atoll(res[0].c_str());
}

bool    MyDao::IsExistTheFileByUserAndMd5code(const string& email,  
        const string& md5code,const string& fdirectory)
{
    vector<string> res;
    char buf[300] = {0};
    sprintf(buf,"select count(*) from userfile where email='%s' and md5code='%s' and fdirectory='%s'",email.c_str(),md5code.c_str(),fdirectory.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoll(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::IsExistDirectoryByEmailAndDir(const string& email,const string& dir,const string& newdir)
{
    vector<string> res;
    char buf[2000] = {0};
    sprintf(buf,"select count(*) from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),newdir.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoll(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::DeleteFileByEmailAndDir(const string& email,const string& dir,const string& fname)
{
    char buf[2000] = {0};
    string path;
    this->QueryFilePathByEmailAndDir(email,dir,fname,path);
    sprintf(buf,"update files set fcount= fcount - 1 where path='%s'",path.c_str());
    m_mysql->UpdateSql(buf);
    this->DeleteFcountNullFiles();
    sprintf(buf,"delete from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::DeleteDirectoryByEmailAndDir(const string& email,const string& dir,const string& fname)
{
    char buf[2000];
    string path;
    //查找文件对应的路径
    this->QueryFilePathByEmailAndDir(email,dir,fname,path);
    auto pos = path.find_last_of('/');
    path = path.substr(0,pos + 1);
    //对所有这一路径下的文件引用计数-1
    sprintf(buf,"update files set fcount=fcount - 1 where path like '%s%'",path.c_str());
    m_mysql->UpdateSql(buf);
    //删除所有引用技术小于1的
    this->DeleteFcountNullFiles();
    string s = dir + fname + '/';
    sprintf(buf,"delete from userfile where email='%s' and fdirectory='%s'",email.c_str(),s.c_str());
    m_mysql->UpdateSql(buf);
    sprintf(buf,"delete from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::ModifyUserCapacity(const string& email,int cap,int flag)
{
    char buf[500] = {0};
    vector<string> res;
    sprintf(buf,"select currcapacity from user where email='%s'",email.c_str());
    m_mysql->QuerySql(buf,1,res);
    long long curr;
    if(flag == 0)
        curr = cap + atoll(res[0].c_str());
    else
        curr = atoll(res[0].c_str()) - cap;
    sprintf(buf,"update user set currcapacity='%d' where email='%s'",curr,email.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

long long     MyDao::QueryUserFileSize(const string& email,const string& dir,const string& fname)
{
    vector<string> res;
    char buf[2000] = {0};
    sprintf(buf,"select size from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,1,res);
    long long size = atoll(res[0].c_str());
    return size;
}

bool    MyDao::QueryFilePathByEmailAndDir(const string& email,const string& dir,const string& fname,string& p)
{
    char buf[2000] = {0};
    memset(buf,0,2000);
    vector<string> res;
    sprintf(buf,"select path from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(!res.empty())
        p = res[0];
    return true;
}

bool    MyDao::QueryFileInfoByMd5code(const string& md5code,vector<string>& res)
{
    char    buf[100];
    memset(buf,0,100);
    sprintf(buf,"select size,path from userfile where md5code='%s'",md5code.c_str());
    m_mysql->QuerySql(buf,2,res);
    return true;
}
//指定用户是否存在这个文件
bool    MyDao::IsExistTheFileByUserAndDir(const string& email,const string& dir,const string& fname)
{
    vector<string> res;
    char buf[2000] = {0};
    memset(buf,0,1000);
    sprintf(buf,"select count(*) from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoi(res[0].c_str()) == 0)
        return false;
    return true;
}
//得到指定文件夹中的所有文件名
bool    MyDao::GetAllFilesInDirectory(const string& email,const string& dir,const string& fname,vector<string>& res)
{
    char buf[2000]= {0};
    memset(buf,0,2000);
    queue<string> queuedir;
    queuedir.push(dir + fname + '/');
    string s;
    vector<string> tmp;
    while(!queuedir.empty())
    {
        s = queuedir.front();
        tmp.clear();
        queuedir.pop();
        sprintf(buf,"select fname,ftype from userfile where email='%s' and fdirectory='%s'",email.c_str(),s.c_str());
        m_mysql->QuerySql(buf,2,tmp);
        for(int i = 0; i < tmp.size(); i = i + 2){
            if(tmp[i + 1] == "0"){
                res.push_back(s + tmp[i]);
            }else{
                queuedir.push(s + tmp[i] + '/');
            }
        }
    }
    return true;
}

//向files表中插入数据项
bool    MyDao::IntoFiles(const string& md5code,const string& name,long long size,const string& path)
{
    char buf[2000];
    memset(buf,0,2000);
    if(this->IsExistFileFromFiles(md5code) == false){
        sprintf(buf,"insert into files values('%s','%s',%lld,'%s',%d)",md5code.c_str(),name.c_str(),size,path.c_str(),1);
    }
    else{
        sprintf(buf,"update files fcount=fcount + 1 where md5code='%s'",md5code.c_str());
    }
    
    m_mysql->UpdateSql(buf);
    return true;
}

//是否存在文件
bool    MyDao::IsExistFileFromFiles(const string& md5code)
{
    char    buf[2000];
    vector<string> res;
    sprintf(buf,"select count(*) from files where md5code='%s'",md5code.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoll(res[0].c_str()) == 0)
        return false;
    return true;
}

//从files中查询文件信息
bool    MyDao::QueryFileInfoFromFiles(const string& md5code,vector<string>& res)
{
    char buf[2000];
    memset(buf,0,2000);
    sprintf(buf,"select * from files where md5code='%s'",md5code.c_str());
    m_mysql->QuerySql(buf,5,res);
    return true;
}
//删除所有引用技术小于1的文件
bool    MyDao::DeleteFcountNullFiles()
{
    char buf[1000] = {0};
    sprintf(buf,"delete from files where fcount < 1");
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::QueryFileMd5SizeByEmailAndDir(const string& email,const string& dir,const string& fname,vector<string>& res)
{
    char buf[1000] = {0};
    memset(buf,0,1000);
    sprintf(buf,"select md5code,size from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),dir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,2,res);
    return true;
}

bool    MyDao::QueryFriendList(const string& email,vector<string>& res)
{
    vector<string> tmp;
    char buf[1000] = {0};
    sprintf(buf,"select femail from friends where email='%s'",email.c_str());
    m_mysql->QuerySql(buf,1,tmp);
    vector<string> ss;
    vector<string> nicks;
    for(int i = 0; i < tmp.size(); i++)
    {
        sprintf(buf,"select nickname from user where email='%s'",tmp[i].c_str());
        m_mysql->QuerySql(buf,1,ss);
        nicks.push_back(ss[0]);
    }
    for(int i = 0; i < tmp.size(); i++)
    {
        res.push_back(tmp[i]);
        res.push_back(nicks[i]);
    }
    return true;
}

bool    MyDao::SearchUserNickName(const string& email,string& nickname)
{
    char buf[500] = {0};
    vector<string> res;
    sprintf(buf,"select nickname from user where email='%s'",email.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(res.empty())
        nickname = "";
    else
        nickname = res[0];
    return true;
}

bool    MyDao::InsertChatMessage(const string& receiver,const string& sender,const string& data,int type)
{
    char buf[2000] = {0};
    sprintf(buf,"insert into chatmessages values('%s','%s','%s',%d,localtime())",receiver.c_str(),sender.c_str(),data.c_str(),type);
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::InsertFriend(const string& semail,const string& remail)
{
    char buf[1000] = {0};
    sprintf(buf,"insert into friends values('%s','%s')",semail.c_str(),remail.c_str());
    m_mysql->UpdateSql(buf);
    sprintf(buf,"insert into friends values('%s','%s')",remail.c_str(),semail.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::DeleteFriend(const string& semail,const string& remail)
{
    char buf[1000] = {0};
    sprintf(buf,"delete from friends where email='%s' and femail='%s'",semail.c_str(),remail.c_str());
    m_mysql->UpdateSql(buf);
    sprintf(buf,"delete from friends where email='%s' and femail='%s'",remail.c_str(),semail.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::QueryFriendMessage(const string& email,vector<string>& res)
{
    char buf[1000] = {0};
    sprintf(buf,"select receiver,sender,content,mestype,mesdate from chatmessages where receiver='%s' order by mesdate asc",email.c_str());
    m_mysql->QuerySql(buf,5,res);
    return true;
}

bool    MyDao::DeleteFriendMessage(const string& email)
{
    char buf[1000] = {0};
    sprintf(buf,"delete from chatmessages where receiver='%s'",email.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::IsExistGetCodeByUser(const string& email,const string& code)
{
    char buf[1000] = {0};
    vector<string> res;
    sprintf(buf,"select count(*) from sharefiles where email='%s' and getcode='%s'",email.c_str(),code.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoi(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::IsExistShareFileByUser(const string& email,const string& fdir,const string& fname)
{
    char buf[1000] = {0};
    vector<string> res;
    sprintf(buf,"select count(*) from sharefiles where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),fdir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoi(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::InsertShareFile(const string& email,const string& fdir,const string& fname,const string& getcode,int days)
{
    char buf[1000] = {0};
    sprintf(buf,"insert into sharefiles values('%s','%s','%s','%s',0,localtime(),%d)",email.c_str(),fdir.c_str(),fname.c_str(),getcode.c_str(),days);
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::QueryShareFileList(const string& email,vector<string>& res)
{
    char buf[1000] = {0};
    sprintf(buf,"select fdirectory,fname,savetime,startdate,days,getcode from sharefiles where email='%s'",email.c_str());
    m_mysql->QuerySql(buf,6,res);
    return true;
}

bool    MyDao::DeleteShareFile(const string& email,const string& fdir,const string& fname)
{
    char buf[1000] = {0};
    sprintf(buf,"delete from sharefiles where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),fdir.c_str(),fname.c_str());
    m_mysql->UpdateSql(buf);
    return true;
}

bool    MyDao::IsExistShareFile(const string& email,const string& getcode)
{
    char buf[1000] = {0};
    vector<string> res;
    sprintf(buf,"select count(*) from sharefiles where email='%s' and getcode='%s'",email.c_str(),getcode.c_str());
    m_mysql->QuerySql(buf,1,res);
    if(atoi(res[0].c_str()) == 0)
        return false;
    return true;
}

bool    MyDao::QueryShareFileInfo(const string& email,const string& getcode,vector<string>& res)
{
    char buf[1000] = {0};
    vector<string> tmp;
    sprintf(buf,"select fdirectory,fname from sharefiles where email='%s' and getcode='%s'",email.c_str(),getcode.c_str());
    m_mysql->QuerySql(buf,2,tmp);
    if(tmp.size() != 2)
    {
        exit(1);
    }
    sprintf(buf,"select ftype from userfile where email='%s' and fdirectory='%s' and fname='%s'",email.c_str(),tmp[0].c_str(),tmp[1].c_str());
    m_mysql->QuerySql(buf,1,res);
    res.push_back(tmp[0]);
    res.push_back(tmp[1]);
    return true;
}

bool    MyDao::SaveShareFile(const string& email,const string& other,const string& fdir,const string& fname,const string& mydir)
{
    char buf[1000] = {0};
    vector<string> res;
    sprintf(buf,"select md5code,ftype,size,path from userfile where email='%s' and fdirectory='%s' and fname='%s'",other.c_str(),fdir.c_str(),fname.c_str());
    m_mysql->QuerySql(buf,4,res);
    if(res.size() != 4)
        return false;
    int type = atoi(res[1].c_str());
    if(type == 0)
    {//单文件类型
        sprintf(buf,"insert into userfile values('%s','%s','%s',%d,'%s',%s,'%s')",email.c_str(),res[0].c_str(),mydir.c_str(),type,fname.c_str()
                ,res[2].c_str(),res[3].c_str());
        m_mysql->UpdateSql(buf);
    }else{//文件夹类型
        queue<pair<string,string>> dirs;
        dirs.push(pair<string,string>(mydir + fname + '/',fdir + fname + '/'));
        string lastdir = mydir;
        while(!dirs.empty())
        {
            auto pp  = dirs.front();
            dirs.pop();
            sprintf(buf,"select md5code,fname,ftype,size,path from userfile where email='%s' and fdirectory='%s'",
                    other.c_str(),pp.second.c_str());
            vector<string> tmp;
            m_mysql->QuerySql(buf,5,tmp);
            int index = 0;
            int count = tmp.size() / 5;
            for(int i = 0; i < count; i++)
            {
                if(tmp[index + 2] == "1")
                {//文件夹类型
                    dirs.push(pair<string,string>(pp.first + tmp[index + 1] + '/',pp.second + tmp[index + 1] + '/'));
                }else{
                    this->InsertFileInfoByEmail(email,tmp[index],pp.first,atoi(tmp[index + 2].c_str()),
                                                tmp[index + 1],atoll(tmp[index + 3].c_str()),tmp[index + 4]);
                }
                index = index + 5;
            }
            
            if(pp.first != "/")
            {
                string s = pp.first.substr(0,pp.first.size() - 1);
                string ss;
                auto pos = s.find_last_of('/');
                ss = s.substr(pos + 1);
                s = s.substr(0,pos + 1);
                this->InsertFileInfoByEmail(email,"",s,1,ss,0,"");
            }   
        }
    }
    return true;
}

long long MyDao::GetSizeFromDirectory(const string& email,const string& fdir)
{
    char buf[1000];
    long long totalSize = 0;
    queue<string> dirs;
    dirs.push(fdir);
    string lastdir = fdir;
    string tmp;
    while(!dirs.empty())
    {
        tmp = dirs.front();
        dirs.pop();
        sprintf(buf,"select ftype,fname,size,fdirectory from userfile where email='%s' and fdirectory='%s'",email.c_str(),tmp.c_str());
        vector<string> res;
        m_mysql->QuerySql(buf,4,res);
        int index = 0;
        int count = res.size() / 4;
        for(int i = 0; i < count; i++)
        {
            if(atoi(res[index].c_str()) == 0){
                totalSize += atoll(res[index + 2].c_str());
            }else{
                dirs.push(res[index + 3] + res[index + 1] + '/');
            }
            index = index + 4;
        }
    }

    return totalSize;
}
