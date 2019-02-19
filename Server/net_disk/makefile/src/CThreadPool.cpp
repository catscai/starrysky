/*************************************************************************
	> File Name: CThreadPool.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat 17 Nov 2018 09:23:58 AM CST
 ************************************************************************/
#include <iostream>
#include "CThreadPool.h"

CThreadPool::CThreadPool():m_QueueTasks(0),m_QueueMaxSize(0),m_QueueCurSize(0),
m_QueueFront(0),m_QueueRear(0),m_ThreadMinNum(0),m_ThreadMaxNum(0),
m_ThreadAliveNum(0),m_ThreadReleaseNum(0),m_ThreadExitFlag(false),m_ManagerThread(0),m_ThreadWorkingNum(0){}

CThreadPool::~CThreadPool(){
    Release();
}

bool CThreadPool::InitPool(unsigned threadMinNum,unsigned threadMaxNum,unsigned taskMaxNum)
{
    if((threadMinNum == 0) || (threadMinNum > threadMaxNum))
        return false;
    pthread_t tid;
    for(unsigned i = 0; i < threadMinNum; i++){
        if(0 == pthread_create(&tid,NULL,ThreadWork,this)){
            pthread_detach(tid);
            m_ThreadSet.insert(tid);
            ++m_ThreadAliveNum;
        }
    }
    m_ThreadMinNum = threadMinNum;
    m_ThreadMaxNum = threadMaxNum;
    m_QueueTasks = new CTask*[taskMaxNum];
    if(m_QueueTasks == NULL)
        throw string("TaskQueue init failed!");
    m_QueueMaxSize = taskMaxNum;
    if(0 != pthread_create(&m_ManagerThread,NULL,Manager,this))
        throw string("Create ManagerThread failed!");
    return true;
}

void* CThreadPool::ThreadWork(void* pvoid)
{
    CThreadPool* pthis = static_cast<CThreadPool*>(pvoid);
    CTask* ptask = NULL;
    while(true)
    {
        pthis->m_Lock.lock();
        while(!pthis->m_ThreadExitFlag && pthis->m_QueueCurSize == 0){
            pthis->m_NotEmpty.wait(pthis->m_Lock.get());
        }
        if(pthis->m_ThreadExitFlag){
            pthis->m_Lock.unlock();
           // cout<<"thread exit........................................................"<<endl;
            return NULL;
        }
        if(pthis->m_ThreadReleaseNum > 0){
            --pthis->m_ThreadReleaseNum;
            --pthis->m_ThreadAliveNum;
            if(pthis->m_ThreadSet.count(pthread_self()))
                pthis->m_ThreadSet.erase(pthread_self());
            pthis->m_Lock.unlock();
           // cout<<"thread exit........................................................"<<endl;
            return NULL;
        }
        ptask = pthis->m_QueueTasks[pthis->m_QueueFront];
        pthis->m_QueueFront = (pthis->m_QueueFront + 1) % pthis->m_QueueMaxSize;
        --pthis->m_QueueCurSize;
        ++pthis->m_ThreadWorkingNum;
        pthis->m_NotFull.signal();
        pthis->m_Lock.unlock();

        ptask->working();
        delete ptask;
        ptask = NULL;

        pthis->m_Lock.lock();
        --pthis->m_ThreadWorkingNum;
        pthis->m_Lock.unlock();
    }
   // cout<<"thread exit........................................................"<<endl;
    return NULL;
}

bool CThreadPool::AddTask(CTask* task)
{
    if(m_ThreadExitFlag){
        return true;
    }
    m_Lock.lock();
    while(!m_ThreadExitFlag && m_QueueCurSize == m_QueueMaxSize){
        m_NotFull.wait(m_Lock.get());
    }
    if(m_ThreadExitFlag){
        m_Lock.unlock();
        return true;
    }
    m_QueueTasks[m_QueueRear] = task;
    m_QueueRear =(m_QueueRear + 1) % m_QueueMaxSize;
    ++m_QueueCurSize;
    m_NotEmpty.signal();
    m_Lock.unlock();
    return true;
}

void* CThreadPool::Manager(void* pvoid)
{
    CThreadPool* pthis = static_cast<CThreadPool*>(pvoid);
    unsigned threadAlive = 0;
    unsigned threadRelease = 0;
    unsigned threadWorking = 0;
    unsigned taskCur = 0;
    unsigned createThreadNum = 0;
    unsigned idleNum = 0;
    unsigned deleteNum = 0;
    unsigned logCount = 0;
    pthis->m_Lock.lock();
    unsigned threadMaxNum = pthis->m_ThreadMaxNum;
    unsigned threadMinNum = pthis->m_ThreadMinNum;
    unsigned taskMax = pthis->m_QueueMaxSize;
    pthis->m_Lock.unlock();
    /*
    ofstream output;
    output.open("./threadpoolLog.txt",ios::out);
    */
    while(!pthis->m_ThreadExitFlag)
    {
        pthis->m_Lock.lock();
        threadAlive = pthis->m_ThreadAliveNum;
        threadWorking = pthis->m_ThreadWorkingNum;
        taskCur = pthis->m_QueueCurSize;
        pthis->m_Lock.unlock();

        if(taskCur > threadAlive){
            createThreadNum = pthis->THREAD_CHANGE_NUM;
            if(createThreadNum + threadAlive > threadMaxNum){
                createThreadNum = threadMaxNum - threadAlive;
            }
            pthis->m_Lock.lock();
            pthread_t tid;
            for(int i = 0; i < createThreadNum; i++){
                if(0 == pthread_create(&tid,NULL,ThreadWork,pthis)){
                    pthis->m_ThreadSet.insert(tid);
                    ++pthis->m_ThreadAliveNum;
                }
            }
            threadAlive = pthis->m_ThreadAliveNum;
            threadWorking = pthis->m_ThreadWorkingNum;
            pthis->m_Lock.unlock();
        }else{
            idleNum = threadAlive - threadWorking;
            if(idleNum > threadWorking * 3){
                deleteNum = threadWorking;
                if(threadAlive - deleteNum < threadMinNum){
                    deleteNum = threadAlive - threadMinNum;
                }
                pthis->m_Lock.lock();
                pthis->m_ThreadReleaseNum = deleteNum;
                pthis->m_Lock.unlock();
                for(unsigned i = 0; i < deleteNum; i++){
                    pthis->m_NotEmpty.signal();
                }
            }
        }
        /*
        if(logCount == 0){
            output<<"ThreadBusy\tThreadAlive\tThreadMin\tThreadMax\tTaskCurr\tTaskMax\t"<<endl;
        }else{
            output<<"\t\t"<<threadWorking<<"\t\t"<<threadAlive<<"\t\t"<<threadMinNum<<"\t\t"
            <<threadMaxNum<<"\t\t"<<taskCur<<"\t\t"<<taskMax<<"\t\t"<<endl;
        }
       if(logCount == 1000){
            logCount = 900;
            ifstream in;
            string text;
            string str;
            in.open("./threadpoolLog.txt",ios::in);
            int nCount = 0;
            while(!in.eof()){
                in>>str;
                if(nCount > 100){
                    text += str;
                    text += '\n';
                }
                ++nCount;
            }
            output.seekp(0,ios::beg);
            output<<text;
        }
        output.flush();
        ++logCount;
        */
        sleep(1);
    }
    return NULL;
}

bool CThreadPool::Release()
{
    if(m_ThreadExitFlag == false){
        m_ThreadExitFlag = true;
        m_NotEmpty.broadcast();
        m_NotFull.broadcast();
        while(m_QueueCurSize){
            delete m_QueueTasks[m_QueueFront];
            m_QueueTasks[m_QueueFront] = NULL;
            m_QueueFront = (m_QueueFront + 1) % m_QueueMaxSize;
            --m_QueueCurSize;
        }
    }
    return true;
}

bool CThreadPool::WaitTaskFinished()
{
    if(m_ThreadExitFlag == false){
        while(m_QueueCurSize != 0){
            cout<<"The current Task num:"<<m_QueueCurSize<<endl;
            sleep(2);
        }
    }
    return true;
}
