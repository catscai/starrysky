#include "stdafx.h"
#include "TcpNet.h"
#include "TcpMediator.h"
#pragma comment(lib, "Ws2_32.lib")
TcpNet::TcpNet(TcpMediator* med)
{
	m_tcpMediator = med;
	m_isCloseFlag = false;
	m_IsExitThreadFlag = false;
	m_hThread = NULL;
}


TcpNet::~TcpNet(void)
{
	this->close();
}

bool	TcpNet::Connection()
{
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(ret != 0){
		return false;
	}
	m_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_sock == INVALID_SOCKET){
		return false;
	}
	sockaddr_in ServerAddr;
	ZeroMemory(&ServerAddr,sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("192.168.31.144");
	ServerAddr.sin_port = htons(SERVER_PORT);
	if(0 != connect(m_sock,(const sockaddr*)&ServerAddr,sizeof(ServerAddr)))
		return false;
	m_hThread = (HANDLE)_beginthreadex(NULL,0,ThreadProc,this,0,NULL);
	return true;
}

bool	TcpNet::send(char* szbuf,unsigned len)
{
	if(SOCKET_ERROR == ::send(this->m_sock,szbuf,len,0))
		return false;
	return true;
}

unsigned __stdcall TcpNet::ThreadProc(void* pvoid)
{
	TcpNet*pthis = (TcpNet*)pvoid;
	RECV_BUFFER buf;
	int			readBytes = 0;
	unsigned	size = sizeof(unsigned);
	unsigned	pack_len = 0;
	bool		IsInterrupt = false;
	while(pthis->m_IsExitThreadFlag == false)
	{
		while(buf.m_len != size){
			readBytes = recv(pthis->m_sock,buf.m_szbuf + buf.m_len,size - buf.m_len,0);
			if(readBytes < 0){
				AfxMessageBox(_T("连接中断"));
				IsInterrupt = true;
				break;
			}else if(readBytes == 0)
				continue;
			buf.m_len += readBytes;
		}
		if(IsInterrupt)
			break;
		pack_len = *((unsigned*)buf.m_szbuf);
		pack_len -= size;
		while(pack_len > 0)
		{
			readBytes = recv(pthis->m_sock,buf.m_szbuf + buf.m_len,pack_len,0);
			if(readBytes < 0){
				AfxMessageBox(_T("连接中断"));
				IsInterrupt = true;
				break;
			}else if(readBytes == 0)
				continue;
			pack_len -= readBytes;
			buf.m_len += readBytes;
		}
		if(IsInterrupt)
			break;
		pthis->m_tcpMediator->dealNetMsg(buf.m_szbuf,buf.m_len);
		buf.reset();
	}
	return 0;
}

bool	TcpNet::close()
{
	if(m_isCloseFlag == false)
	{
		m_isCloseFlag = true;
		closesocket(m_sock);
		WSACleanup();
	}
	return true;
}