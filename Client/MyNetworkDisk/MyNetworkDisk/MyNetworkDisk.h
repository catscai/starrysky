
// MyNetworkDisk.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <string>
using namespace std;
#include "MyTools.h"

// CMyNetworkDiskApp:
// �йش����ʵ�֣������ MyNetworkDisk.cpp
//
class TcpMediator;
class CMyMainDlg;
class CMyNetworkDiskApp : public CWinApp
{
public:
	CMyNetworkDiskApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
private:
	TcpMediator*			m_mediator;
public:
	TcpMediator*			net(){
		return m_mediator;
	}
	virtual int ExitInstance();
public:
	CString			m_nicknameCStr;
	CString			m_vip;
	string			m_email;
	string			m_nickname;
	string			m_currdir;
	long long		m_currcapacity;
	long long		m_capacity;
	string			m_home;
	string			m_downloadPath;
	int				m_downloadNum;
	int				m_uploadNum;
	//CMyMainDlg*		m_mainlockdlg;
	void	SetParams(CString n,CString v,string e,long long currcap,long long cap)
	{
		m_nicknameCStr = n;
		m_vip = v;
		m_email = e;
		m_currcapacity = currcap;
		m_capacity = cap;
		CMyTools::EnCodeWCHARToUtf(n.GetBuffer(),m_nickname);
	}
};

extern CMyNetworkDiskApp theApp;