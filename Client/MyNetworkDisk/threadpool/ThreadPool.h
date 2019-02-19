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
#define THREADPOOL_THREADNUM_ADD 10			//��̬���Ĭ��һ��10��
/*
��ܣ�ʹ��������-��������ģʽ����ӹ������̹߳����̵߳Ĵ�����ɾ����
�ؼ�֪ʶ�������ߺͶ������߹�ͬ����һ���������(������)����-��֮��ͨ��(��������)��
�����̳߳��ࣺ���ɼ̳С����ɿ�����
*/
/*�ڲ��ࣺ��װ�ؼ��εĻ�������*/
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
	MyTask**				m_queue;				//�������(����)
	unsigned				m_front;				//����ͷ
	unsigned				m_rear;					//����β
	volatile unsigned		m_queueCurrNum;			//��ǰ������
	unsigned				m_queueMaxNum;			//������󳤶�
	map<HANDLE,bool>		m_threadMap;			//map�����洢�߳̾��
	unsigned				m_threadMinNum;			//��ʼ���߳���С��
	unsigned				m_threadMaxNum;			//��ʼ���߳������
	unsigned				m_threadAliveNum;		//��ǰ����߳���
	unsigned				m_threadWorkingNum;		//��ǰ���ڹ������߳���
	unsigned				m_threadReleaseNum;		//��̬�����߳���(�ɹ����߳�֧��)
	MyLock					m_taskLock;				//���������
	MyLock					m_varLock;				//������
	CONDITION_VARIABLE		m_notFull;				//��������������ʹ��
	CONDITION_VARIABLE		m_notEmpty;				//��������������ʹ��
	HANDLE					m_managerThread;		//�̳߳��߳������Ĺ����߳�
	volatile bool			m_threadFlag;			//�߳��˳���־
public:
	ThreadPool(void);
	~ThreadPool(void);
private:
	ThreadPool(const ThreadPool& pool);				//��ֹ���󿽱�
	ThreadPool& operator=(ThreadPool& pool);
private:
	static unsigned __stdcall ThreadProc(void* pvoid);
	static unsigned __stdcall ThreadManager(void* pvoid);
public:
	/*��ʼ���̳߳أ��߳���С������������������������󳤶�*/
	bool CreateThreadPool(unsigned threadMinNum,unsigned threadMaxNum,unsigned taskMaxNum = 200);
	/*�����̳߳أ��ͷ�������Դ*/
	bool DestroyThreadPool();
	/*��������û����Զ���һ����̳�MyTaskʵ�ַ���ExecTask����������������������*/
	bool AddTask(MyTask* task);	
	/*�����ȴ�����������ɣ�һ�����������̳߳�(DestroyThreadPool)֮ǰ*/
	bool WaitTaskFinished();			
};

