/*************************************************************************
	> File Name: MySqlPool.h
	> Author: 
	> Mail: 
	> Created Time: jeu. 10 janv. 2019 11:57:11 CST
 ************************************************************************/

#ifndef _MYSQLPOOL_H
#define _MYSQLPOOL_H
#include <string>
#include <list>
#include <pthread.h>
using namespace std;
class MyDao;
class   MySqlPool
{
private:
    string              m_host;
    string              m_user;
    string              m_pass;
    string              m_dbName;
    list<MyDao*>        m_connLst;
    int                 m_minNum;
    int                 m_maxNum;
    int                 m_curNum;
    static pthread_mutex_t lock;
private:
    MySqlPool():m_minNum(0),m_maxNum(0),m_curNum(0){}
    MyDao*  CreateConn();
    void    DestoryPool();
public:
    ~MySqlPool();
    static  MySqlPool& PoolHandle();
    bool    InitPool(const string&,const string&,const string&,const string&,int min,int max);
    MyDao*  GetConn();
    bool    ReleaseConn(MyDao*);
};

#endif
