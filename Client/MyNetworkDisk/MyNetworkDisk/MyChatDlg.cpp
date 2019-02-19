// MyChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyChatDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
// CMyChatDlg 对话框

IMPLEMENT_DYNAMIC(CMyChatDlg, CDialogEx)

CMyChatDlg::CMyChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyChatDlg::IDD, pParent)
{
	m_friendinfo = NULL;
}

CMyChatDlg::~CMyChatDlg()
{
}

void CMyChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_TEXTAREA, m_ChatTextAreaEdit);
	DDX_Control(pDX, IDC_SEND_TEXTAREA, m_SendTextAreaEdit);
}


BEGIN_MESSAGE_MAP(CMyChatDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, &CMyChatDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyChatDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMyChatDlg 消息处理程序


BOOL CMyChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMyChatDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void	CMyChatDlg::SetFriendInfo(FriendInfo* info)
{
	m_friendinfo = info;
}

//发送消息
void CMyChatDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	CString text;
	m_SendTextAreaEdit.GetWindowText(text);
	string data;
	CMyTools::EnCodeWCHARToUtf(text,data);
	if(data.size() > CHAT_MESSAGE_LENGTH){
		AfxMessageBox(_T("内容过长"));
		return;
	}
	CString out;
	string tim;
	CMyTools::NowTime(out);
	CMyTools::EnCodeWCHARToUtf(out.GetBuffer(),tim);
	UserMessage mes;
	memset(&mes,0,sizeof(mes));
	mes.m_len = sizeof(mes);
	mes.m_type = CHAT_SEND_MESSAGES_MSG;
	memcpy_s(mes.m_sender,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(mes.m_receiver,EMAIL_SIZE,m_friendinfo->m_email,strlen(m_friendinfo->m_email));
	memcpy_s(mes.m_data,CHAT_MESSAGE_LENGTH,data.c_str(),data.size());
	mes.m_messagetype = CHAT_TYPE_NORMAL;
	memcpy_s(mes.m_date,CHAT_MESSAGE_DATETIME_LENGTH,tim.c_str(),tim.size());
	//bool flag = true;
	bool flag = theApp.net()->send((char*)&mes,mes.m_len);
	if(flag){
		m_SendTextAreaEdit.SetWindowText(_T(""));
		this->AddTextToChatArea(text,0);
	}
}

//清空
void CMyChatDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_SendTextAreaEdit.SetWindowText(_T(""));
}

void	CMyChatDlg::AddTextToChatArea(const CString& text,int mark,CString tt)
{
	CString message,t;
	if(mark == 0){
		message = _T("我:\r\n");	
		CMyTools::NowTime(t);
	}else{
		CString f;
		CMyTools::EnCodeUtfToWCHAR(m_friendinfo->m_nickname,f);
		message = f + _T(":\r\n");
		t = tt;
	}
	message += _T("时间: ") + t + _T("\r\n");
	message += text + _T("\r\n\r\n");
	int nLength = m_ChatTextAreaEdit.GetWindowTextLength();
	m_ChatTextAreaEdit.SetSel(nLength,nLength);
	m_ChatTextAreaEdit.ReplaceSel(message);
}

void	CMyChatDlg::ClearTextArea()
{
	this->m_ChatTextAreaEdit.SetWindowText(_T(""));
}