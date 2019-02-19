/*************************************************************************
	> File Name: MyDao.h
	> Author: 
	> Mail: 
	> Created Time: ven. 21 déc. 2018 10:29:48 CST
 ************************************************************************/

#ifndef _MYDAO_H
#define _MYDAO_H
#include <string>
#include <vector>
using namespace std;
class CMySql;
class MyDao
{
private:
    CMySql*     m_mysql;
public:
    MyDao(const string& host,const string& user,const string& pass,
            const string& db_name);
    ~MyDao();
public:
    bool    IsExistTheEmail(const string& email);
    bool    IsExistTheFile(const string& md5code);
    bool    QueryUserByEmail(const string& email,vector<string>& res);
    bool    QueryUserByEmailAndPass(const string& email,const string& pass,
                    vector<string>& res);
    bool    QueryFileListByEmailAndDirectory(const string& email,
                    const string& dir,vector<string>& res);
    bool    InsertIntoUser(const string& e,const string& nick,const string& pass,
                    const string& date,int isEffect);

    int     VipClassByEmail(const string& email);
    bool    InsertFileInfoByEmail(const string& email,const string& md5code,const string& fdirectory,int ftype,const string& fname,unsigned size,const string& path);
    long long     QueryUserResidueCapacity(const string& email);
    bool    IsExistTheFileByUserAndMd5code(const string& email,const string& md5code,const string& fdir);
    bool    IsExistDirectoryByEmailAndDir(const string& email,const string& dir,const string& newdir);
    bool    DeleteFileByEmailAndDir(const string& email,const string& dir,const string& fname);
    bool    DeleteDirectoryByEmailAndDir(const string& email,const string& dir,const string& fname);
    bool    ModifyUserCapacity(const string& email,int cap,int flag);
    long long     QueryUserFileSize(const string& email,const string& dir,const string& fname);
    bool    QueryFilePathByEmailAndDir(const string& email,const string& dir,const string& fname,string& p);
    bool    QueryFileInfoByMd5code(const string& md5code,vector<string>& res);
    bool    IsExistTheFileByUserAndDir(const string& email,const string& dir,const string& fname);
    bool    GetAllFilesInDirectory(const string& email,const string& dir,const string& fname,vector<string>& res);
    bool    IntoFiles(const string& md5code,const string& name,long long size,const string& path);
    bool    IsExistFileFromFiles(const string& md5code);
    bool    QueryFileInfoFromFiles(const string& md5code,vector<string>& res);
    bool    DeleteFcountNullFiles();
    bool    QueryFileMd5SizeByEmailAndDir(const string& email,const string& dir,const string& fname,vector<string>& res);
    bool    QueryFriendList(const string& email,vector<string>& res);
    bool    SearchUserNickName(const string& email,string& nickname);
    bool    InsertChatMessage(const string& receiver,const string& sender,const string& data,int type);
    bool    InsertFriend(const string& semail,const string& remail);
    bool    DeleteFriend(const string& semail,const string& remail);
    bool    QueryFriendMessage(const string& email,vector<string>& res);
    bool    DeleteFriendMessage(const string& email);
    bool    IsExistGetCodeByUser(const string& email,const string& code);
    bool    IsExistShareFileByUser(const string& email,const string& fdir,const string& fname);
    bool    InsertShareFile(const string& email,const string& fdir,const string& fname,const string& getcode,int days);
    bool    QueryShareFileList(const string& email,vector<string>& res);
    bool    DeleteShareFile(const string& email,const string& fdir,const string& fname);
    bool    IsExistShareFile(const string& email,const string& getcode);
    bool    QueryShareFileInfo(const string& email,const string& getcode,vector<string>& res);
    bool    SaveShareFile(const string& email,const string& other,const string& fdir,const string& fname,const string& mydir);
    long long    GetSizeFromDirectory(const string& email,const string& fdir);
};
#endif
