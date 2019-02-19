#include "stdafx.h"
#include "ThreadPool.h"
//#include <iostream>

ThreadPool::ThreadPool(void)
{
	m_queue = NULL;
	m_front = 0;
	m_rear = 0;
	m_queueCurrNum = 0;
	m_queueMaxNum = 0;
	m_threadMinNum = 0;
	m_threadMaxNum = 0;
	m_threadAliveNum = 0;
	m_threadWorkingNum= 0;
	m_threadReleaseNum = 0;
	m_managerThread = NULL;
	m_threadFlag = true;
}


ThreadPool::~ThreadPool(void)
{

}

bool ThreadPool::CreateThreadPool(unsigned threadMinNum,unsigned threadMaxNum,unsigned tasMaxNum)
{
	if(threadMinNum > threadMaxNum || tasMaxNum <= 0)
		return false;
	m_threadMinNum = threadMinNum;
	m_threadMaxNum = threadMaxNum;
	m_queueMaxNum = tasMaxNum;
	m_queue = new MyTask*[tasMaxNum];
	if(m_queue == NULL)
		return false;
	InitializeConditionVariable(&m_notFull);
	InitializeConditionVariable(&m_notEmpty);
	for(unsigned i = 0; i < threadMinNum;i++)
	{
		HANDLE th = (HANDLE)_beginthreadex(NULL,0,ThreadProc,this,0,NULL);
		if((int)th != -1)
		{
			m_threadMap[th] = true;
			++m_threadAliveNum;
		}else{
			delete[] m_queue;
			return false;
		}
	}
	HANDLE m_managerThread = (HANDLE)(HANDLE)_beginthreadex(NULL,0,ThreadManager,this,0,NULL);
	return true;
}

//工作线程
unsigned __stdcall ThreadPool::ThreadProc(void* pvoid)
{
	ThreadPool* pthis = (ThreadPool*)pvoid;
	MyTask* t;
	while(true)
	{
		pthis->m_taskLock.lock();
		while(pthis->m_threadFlag && pthis->m_queueCurrNum == 0)
		{
			SleepConditionVariableCS(&pthis->m_notEmpty,&pthis->m_taskLock.getlock(),INFINITE);
		}
		if(pthis->m_threadFlag == false)
		{
			pthis->m_taskLock.unlock();
			return 0;
		}
		if(pthis->m_threadReleaseNum > 0)
		{//动态缩减线程池
			--pthis->m_threadReleaseNum;
			--pthis->m_threadAliveNum;
			auto ite = pthis->m_threadMap.find(GetCurrentThread());
			if(ite != pthis->m_threadMap.end()){
				CloseHandle(ite->first);
				const_cast<HANDLE>(ite->first) = NULL;
				pthis->m_threadMap.erase(ite);
			}
			pthis->m_taskLock.unlock();
			return 0;
		}
		++pthis->m_threadWorkingNum;
		t = pthis->m_queue[pthis->m_front];
		pthis->m_front = (pthis->m_front + 1) % pthis->m_queueMaxNum;
		WakeConditionVariable(&pthis->m_notFull);	//唤醒生产者线程
		--pthis->m_queueCurrNum;
		pthis->m_taskLock.unlock();

		t->ExecTask();
		delete t;
		t = NULL;

		pthis->m_taskLock.lock();
		--pthis->m_threadWorkingNum;
		pthis->m_taskLock.unlock();
	}
	return 0;
}

//添加任务(生产者)
bool ThreadPool::AddTask(MyTask* task)
{
	this->m_taskLock.lock();
	while(m_threadFlag && m_queueCurrNum == m_queueMaxNum)
	{
		SleepConditionVariableCS(&m_notFull,&m_taskLock.getlock(),INFINITE);
	}
	if(m_threadFlag == false)
	{
		this->m_taskLock.unlock();
		return false;
	}
	m_queue[m_rear] = task;
	m_rear = (m_rear + 1) % m_queueMaxNum;
	WakeConditionVariable(&m_notEmpty);
	++m_queueCurrNum;
	this->m_taskLock.unlock();
	return true;
}

//管理线程
unsigned __stdcall ThreadPool::ThreadManager(void* pvoid)
{
	ThreadPool* pthis = (ThreadPool*)pvoid;
	unsigned aliveNum = 0;
	unsigned idleNum = 0;
	unsigned busyNum = 0;
	unsigned minNum = 0;
	unsigned maxNum = 0;
	unsigned queueCurrNum = 0;
	unsigned releaseNum = 0;
	unsigned createNum = 0;
	char buf[1024] = {0};
	pthis->m_taskLock.lock();
	minNum = pthis->m_threadMinNum;
	maxNum = pthis->m_threadMaxNum;
	pthis->m_taskLock.unlock();
	while(pthis->m_threadFlag)
	{
		pthis->m_taskLock.lock();
		aliveNum = pthis->m_threadAliveNum;
		busyNum = pthis->m_threadWorkingNum;
		queueCurrNum = pthis->m_queueCurrNum;
		pthis->m_taskLock.unlock();

		idleNum = aliveNum - busyNum;
		//sprintf_s(buf,"min:%d\tmax:%d\talive:%d\tworking:%d\tidle:%d\tqueueCurrNum:%d\n",minNum,maxNum,aliveNum,busyNum,idleNum,queueCurrNum);
		//std::cout<<buf;
		//动态缩减线程
		if(busyNum * 3 < idleNum)
		{
			releaseNum = busyNum;
			if(aliveNum - releaseNum <= minNum)
			{
				releaseNum = aliveNum - minNum;
			}
			pthis->m_taskLock.lock();
			pthis->m_threadReleaseNum = releaseNum;
			pthis->m_taskLock.unlock();
			//发送信号、让线程不再阻塞、进行释放
			for(unsigned i = 0; i < releaseNum; i++)
			{
				WakeConditionVariable(&pthis->m_notEmpty);
			}
		}

		//动态添加线程
		if(queueCurrNum > aliveNum)
		{
			createNum = THREADPOOL_THREADNUM_ADD;
			if(aliveNum + THREADPOOL_THREADNUM_ADD >= maxNum)
			{
				createNum = maxNum - aliveNum;
			}
			for(unsigned i = 0; i < createNum; i++)
			{
				pthis->m_taskLock.lock();
				HANDLE th = (HANDLE)_beginthreadex(NULL,0,ThreadProc,pthis,0,NULL);
				pthis->m_threadMap[th] = true;
				++pthis->m_threadAliveNum;
				pthis->m_taskLock.unlock();
			}
		}
		//周期性进行检测
		Sleep(500);
	}
	return 0;
}

bool ThreadPool::DestroyThreadPool()
{
	//设置线程退出标志
	m_threadFlag = false;
	//发送信号、使消费者线程不再阻塞、而是检查到退出标志而退出
	WakeAllConditionVariable(&m_notEmpty);
	WakeAllConditionVariable(&m_notFull);
	auto ite = m_threadMap.begin();
	while(ite != m_threadMap.end())
	{
		WaitForSingleObject(ite->first,INFINITE);
		CloseHandle(ite->first);
		const_cast<HANDLE>(ite->first) = NULL;
		ite = m_threadMap.erase(ite);
	}
	WaitForSingleObject(m_managerThread,INFINITE);
	CloseHandle(m_managerThread);
	m_managerThread = NULL;
	if(m_queue)
	{
		delete[] m_queue;
		m_queue = NULL;
	}
	return true;
}

bool ThreadPool::WaitTaskFinished()
{
	while(m_queueCurrNum != 0)
	{
		Sleep(1000);
	}
	return true;
}

