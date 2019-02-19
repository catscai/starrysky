// MyAddFriendRequestDlg.cpp : ʵ���ļ�
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
// CMyAddFriendRequestDlg �Ի���

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


// CMyAddFriendRequestDlg ��Ϣ�������

//ͬ�����
void CMyAddFriendRequestDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//ȡ��
void CMyAddFriendRequestDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
