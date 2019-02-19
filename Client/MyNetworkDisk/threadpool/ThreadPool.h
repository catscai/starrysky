#pragma once
#include <Windows.h>
#include <process.h>
#include <list>
#include <map>
using std::map;
using std::list;
class MyTask
{
public:
	virtual ~MyTask(){};
	virtual void ExecTask()=0;
};
#define THREADPOOL_THREADNUM_ADD 10			//动态添加默认一次10个
/*
框架：使用生产者-多消费者模式、外加管理者线程管理线程的创建和删除；
关键知识：生产者和多消费者共同操作一个任务队列(互斥锁)、生-消之间通信(条件变量)；
关于线程池类：不可继承、不可拷贝；
*/
/*内部类：封装关键段的互斥锁类*/
class MyLock
{
private:
	CRITICAL_SECTION m_lock;
public:
	MyLock()
	{
		InitializeCriticalSection(&m_lock);
	}
	~MyLock()
	{
		DeleteCriticalSection(&m_lock);
	}
public:
	void lock()
	{
		
		EnterCriticalSection(&m_lock);
	}
	void unlock()
	{
		LeaveCriticalSection(&m_lock);
	}
	CRITICAL_SECTION& getlock()
	{
		return m_lock;
	}
};
class ThreadPool final						
{
private:
	MyTask**				m_queue;				//任务队列(环形)
	unsigned				m_front;				//队列头
	unsigned				m_rear;					//队列尾
	volatile unsigned		m_queueCurrNum;			//当前任务数
	unsigned				m_queueMaxNum;			//队列最大长度
	map<HANDLE,bool>		m_threadMap;			//map容器存储线程句柄
	unsigned				m_threadMinNum;			//初始化线程最小数
	unsigned				m_threadMaxNum;			//初始化线程最大数
	unsigned				m_threadAliveNum;		//当前存活线程数
	unsigned				m_threadWorkingNum;		//当前正在工作的线程数
	unsigned				m_threadReleaseNum;		//动态缩减线程数(由管理线程支配)
	MyLock					m_taskLock;				//任务队列锁
	MyLock					m_varLock;				//变量锁
	CONDITION_VARIABLE		m_notFull;				//条件变量生产者使用
	CONDITION_VARIABLE		m_notEmpty;				//条件变量消费者使用
	HANDLE					m_managerThread;		//线程池线程数量的管理线程
	volatile bool			m_threadFlag;			//线程退出标志
public:
	ThreadPool(void);
	~ThreadPool(void);
private:
	ThreadPool(const ThreadPool& pool);				//禁止对象拷贝
	ThreadPool& operator=(ThreadPool& pool);
private:
	static unsigned __stdcall ThreadProc(void* pvoid);
	static unsigned __stdcall ThreadManager(void* pvoid);
public:
	/*初始化线程池：线程最小数量，最大数量，任务队列最大长度*/
	bool CreateThreadPool(unsigned threadMinNum,unsigned threadMaxNum,unsigned taskMaxNum = 200);
	/*销毁线程池：释放所有资源*/
	bool DestroyThreadPool();
	/*添加任务：用户可自定义一个类继承MyTask实现方法ExecTask、若任务队列已满则会阻塞*/
	bool AddTask(MyTask* task);	
	/*阻塞等待所有任务完成：一般用于销毁线程池(DestroyThreadPool)之前*/
	bool WaitTaskFinished();			
};

