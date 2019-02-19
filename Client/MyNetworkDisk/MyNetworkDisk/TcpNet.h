#pragma once
#include <winsock2.h>
#include <MSWSock.h>
#include <memory>
using namespace std;
#include "packdef.h"
struct RECV_BUFFER
{
	unsigned	m_len;
	char	m_szbuf[MAX_BUFFER_SIZE];
	RECV_BUFFER(){
		this->reset();
	}
	void	reset(){
		ZeroMemory(m_szbuf,MAX_BUFFER_SIZE);
		m_len = 0;
	}
};

class TcpMediator;
class TcpNet
{
private:
	SOCKET						m_sock;
	shared_ptr<RECV_BUFFER>		m_recvBuff;
	TcpMediator*				m_tcpMediator;
	bool						m_isCloseFlag;
	bool						m_IsExitThreadFlag;
	HANDLE						m_hThread;
public:
	TcpNet(TcpMediator* med);
	~TcpNet(void);
public:
	bool	Connection();
	bool	send(char* szbuf,unsigned len);
	static unsigned __stdcall ThreadProc(void* pvoid);
	bool	close();
};

