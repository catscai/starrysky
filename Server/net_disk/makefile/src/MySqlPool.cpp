/*************************************************************************
	> File Name: MySqlPool.cpp
	> Author: 
	> Mail: 
	> Created Time: jeu. 10 janv. 2019 12:13:15 CST
 ************************************************************************/

#include<iostream>
using namespace std;
#include "MySqlPool.h"
#include "MyDao.h"
MySqlPool::~MySqlPool()
{
    this->DestoryPool();
}

pthread_mutex_t MySqlPool::lock = PTHREAD_MUTEX_INITIALIZER;

MySqlPool&  MySqlPool::PoolHandle()
{
    static MySqlPool pool;
    return pool;
}

bool    MySqlPool::InitPool(const string& host,const string& user,const string& pass,const string& dbname,int min,int max)
{
    this->m_host = host;
    this->m_user = user;
    this->m_pass = pass;
    this->m_dbName = dbname;
    this->m_minNum = min;
    this->m_maxNum = max;
    this->m_curNum = 0;
    
    pthread_mutex_lock(&lock);
    MyDao* db = NULL;
    for(int i = 0; i < min; i++)
    {
        db = this->CreateConn();
        if(db)
        {
            m_connLst.push_back(db);
            ++this->m_curNum;
        }
    }
    pthread_mutex_unlock(&lock);
    return true;
}

MyDao*  MySqlPool::GetConn()
{
    MyDao* db = NULL;
    
    pthread_mutex_lock(&lock);
    if(m_connLst.size() > 0)
    {
        db = m_connLst.front();
        m_connLst.pop_front();
        --this->m_curNum;
    }else{
        if(this->m_curNum < this->m_maxNum)
        {
            db = this->CreateConn();
            if(db)
            {
                ++this->m_curNum;
            }
        }
    }
    pthread_mutex_unlock(&lock);
    return db;
}

bool    MySqlPool::ReleaseConn(MyDao* db)
{
    if(db)
    {
        pthread_mutex_lock(&lock);
        this->m_connLst.push_back(db);
        ++this->m_curNum;
        pthread_mutex_unlock(&lock);
    }
    return true;
}

MyDao*  MySqlPool::CreateConn()
{
    MyDao* db = NULL;
    try{
        db = new MyDao(this->m_host,this->m_user,this->m_pass,this->m_dbName);
    }catch(bad_alloc& e){
        cout<<"new MyDao failed"<<endl;
        db = NULL;
    }
    return db;
}

void    MySqlPool::DestoryPool()
{
    pthread_mutex_lock(&lock);
    auto ite = this->m_connLst.begin();
    while(ite != this->m_connLst.end())
    {
        delete (*ite);
        *ite = NULL;
        ++ite;
    }
    this->m_connLst.clear();
    pthread_mutex_unlock(&lock);
}
