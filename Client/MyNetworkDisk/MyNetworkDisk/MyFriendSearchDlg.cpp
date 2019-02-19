// MyFriendSearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyFriendSearchDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
// CMyFriendSearchDlg 对话框

IMPLEMENT_DYNAMIC(CMyFriendSearchDlg, CDialogEx)

CMyFriendSearchDlg::CMyFriendSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyFriendSearchDlg::IDD, pParent)
{
	//m_IsFriendFlag = false;
}
void CMyFriendSearchDlg::SetInitParams(CString e, CString n,bool flag)
{
	m_searchemail = e;
	m_searchnick = n;
	m_IsFriendFlag = flag;
}
CMyFriendSearchDlg::~CMyFriendSearchDlg()
{
}

void CMyFriendSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SEARCH_EMAIL_EDIT, m_SearchEmail);
	DDX_Control(pDX, IDC_SEARCH_NICKNAME_EDIT, m_SearchNickName);
}


BEGIN_MESSAGE_MAP(CMyFriendSearchDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyFriendSearchDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyFriendSearchDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMyFriendSearchDlg 消息处理程序

//添加好友
void CMyFriendSearchDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_IsFriendFlag == false)
	{
		string email,nick;
		CMyTools::EnCodeWCHARToUtf(m_searchemail,email);
		CMyTools::EnCodeWCHARToUtf(m_searchnick,nick);
		UserMessage mes;
		memset(&mes,0,sizeof(mes));
		mes.m_len = sizeof(mes);
		mes.m_type = CHAT_SEND_MESSAGES_MSG;
		mes.m_messagetype = CHAT_TYPE_ADDREQUEST;
		memcpy_s(mes.m_sender,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
		memcpy_s(mes.m_receiver,EMAIL_SIZE,email.c_str(),email.size());
		memcpy_s(mes.m_data,CHAT_MESSAGE_LENGTH,theApp.m_nickname.c_str(),theApp.m_nickname.size());
		
		theApp.net()->send((char*)&mes,mes.m_len);
		this->OnBnClickedButton5();
	}else{
		AfxMessageBox(_T("该好友已存在！"));
	}
}

//取消
void CMyFriendSearchDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SendMessage(WM_CLOSE);
}


BOOL CMyFriendSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->m_SearchEmail.SetWindowText(m_searchemail);
	this->m_SearchNickName.SetWindowText(m_searchnick);
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
}
