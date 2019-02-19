/*************************************************************************
	> File Name: mypool.h
	> Author: 
	> Mail: 
	> Created Time: Fri 16 Nov 2018 09:20:46 PM CST
 ************************************************************************/

#ifndef _MYPOOL_H
#define _MYPOOL_H
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pthread.h>
#include<string>
#include<set>
#include<fstream>
using namespace std;
class CTask
{
public:
    virtual ~CTask(){};
public:
    virtual void working()=0;
};
class CThreadPool
{
private:
    class MyLock
    {
    private:
        pthread_mutex_t mutex;
    public:
        MyLock(){
            pthread_mutex_init(&mutex,NULL);
        }
        ~MyLock(){
            pthread_mutex_destroy(&mutex);
        }
        void    lock(){
            pthread_mutex_lock(&mutex);
        }
        void    unlock(){
            pthread_mutex_unlock(&mutex);
        }
        pthread_mutex_t* get(){
            return &mutex;
        }
    };
    class MyCond
    {
    private:
        pthread_cond_t cond;
    public:
        MyCond(){
            pthread_cond_init(&cond,NULL);
        }
        ~MyCond(){
            pthread_cond_destroy(&cond);
        }
        int wait(pthread_mutex_t* mutex){
            return pthread_cond_wait(&cond,mutex);
        }
        int signal(){
            return pthread_cond_signal(&cond);
        }
        int broadcast(){
            return pthread_cond_broadcast(&cond);
        }
    };
private:
    enum        POOL{THREAD_CHANGE_NUM = 10,QUIT_WAIT = 5};
    CTask**     m_QueueTasks;
    unsigned    m_QueueMaxSize;
    volatile unsigned    m_QueueCurSize;
    unsigned    m_QueueFront;
    unsigned    m_QueueRear;
    unsigned    m_ThreadMinNum;
    unsigned    m_ThreadMaxNum;
    unsigned    m_ThreadAliveNum;
    unsigned    m_ThreadWorkingNum;
    unsigned    m_ThreadReleaseNum;
    MyLock      m_Lock;
    MyCond      m_NotFull;
    MyCond      m_NotEmpty;
    pthread_t   m_ManagerThread;
    volatile bool        m_ThreadExitFlag;
    set<pthread_t> m_ThreadSet;
public:
    CThreadPool();
    ~CThreadPool();
public:
    bool    InitPool(unsigned threadMinNum = 10,unsigned threadMaxNum = 50,unsigned taskMaxNum = 100);
    bool    AddTask(CTask* task);
    static  void*   ThreadWork(void* pvoid);
    static  void*   Manager(void* pvoid);
    bool    Release();
    bool    WaitTaskFinished();
};
#endif
