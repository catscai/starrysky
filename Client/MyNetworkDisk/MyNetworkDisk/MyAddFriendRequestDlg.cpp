// MyAddFriendRequestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyAddFriendRequestDlg.h"
#include "afxdialogex.h"
#include "MyFriendsDlg.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
#include "MyMainDlg.h"
// CMyAddFriendRequestDlg 对话框

IMPLEMENT_DYNAMIC(CMyAddFriendRequestDlg, CDialogEx)

CMyAddFriendRequestDlg::CMyAddFriendRequestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyAddFriendRequestDlg::IDD, pParent)
{

}

CMyAddFriendRequestDlg::~CMyAddFriendRequestDlg()
{
}

void CMyAddFriendRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REQUEST_EMAIL, m_RequestEmail);
	DDX_Control(pDX, IDC_REQUEST_NICK, m_RequestNick);
}


BEGIN_MESSAGE_MAP(CMyAddFriendRequestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyAddFriendRequestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyAddFriendRequestDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMyAddFriendRequestDlg 消息处理程序

//同意添加
void CMyAddFriendRequestDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyMainDlg* p = (CMyMainDlg*)theApp.m_pMainWnd;
	p->m_MyFriendsDlg.AddFriend(m_requestemail,m_requestnick);
	string recvemail;
	CMyTools::EnCodeWCHARToUtf(m_requestemail,recvemail);
	AgreeAddFriend aa;
	memset(&aa,0,sizeof(aa));
	aa.m_len = sizeof(aa);
	aa.m_type = AGREE_ADD_FRIEND_RESPONSE_MSG;
	memcpy_s(aa.m_sender,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(aa.m_receiver,EMAIL_SIZE,recvemail.c_str(),recvemail.size());
	memcpy_s(aa.m_sendernick,NICKNAME_SIZE,theApp.m_nickname.c_str(),theApp.m_nickname.size());
	theApp.net()->send((char*)&aa,aa.m_len);
	this->OnBnClickedButton5();
}

//取消
void CMyAddFriendRequestDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SendMessage(WM_CLOSE);
}

void	CMyAddFriendRequestDlg::SetInitParams(const CString& email,const CString& nick)
{
	this->m_requestemail = email;
	this->m_requestnick = nick;
}

BOOL CMyAddFriendRequestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	m_RequestEmail.SetWindowText(m_requestemail);
	m_RequestNick.SetWindowText(m_requestnick);
	UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control
}
