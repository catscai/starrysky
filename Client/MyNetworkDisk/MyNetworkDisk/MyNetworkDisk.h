
// MyNetworkDisk.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <string>
using namespace std;
#include "MyTools.h"

// CMyNetworkDiskApp:
// 有关此类的实现，请参阅 MyNetworkDisk.cpp
//
class TcpMediator;
class CMyMainDlg;
class CMyNetworkDiskApp : public CWinApp
{
public:
	CMyNetworkDiskApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

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