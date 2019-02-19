// MyFriendsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyFriendsDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "TcpMediator.h"
#include "MyTools.h"
#include "MyFriendSearchDlg.h"
#include "MyAddFriendRequestDlg.h"
// CMyFriendsDlg 对话框

IMPLEMENT_DYNAMIC(CMyFriendsDlg, CDialogEx)

CMyFriendsDlg::CMyFriendsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyFriendsDlg::IDD, pParent)
{

}

CMyFriendsDlg::~CMyFriendsDlg()
{
}

void CMyFriendsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FRIEND_SEARCH, m_SearchEdit);
	DDX_Control(pDX, IDC_FRIENDS_LIST, m_FriendListBox);
	DDX_Control(pDX, IDC_FRIENDS_CHAT_STATUS, m_ChatStatusEdit);
	DDX_Control(pDX, IDC_CHAT_WINDOW_PIC, m_PictureChild);
}


BEGIN_MESSAGE_MAP(CMyFriendsDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_FRIEND_SEND_MESS, &CMyFriendsDlg::OnFriendSendMess)
	ON_COMMAND(ID_FRIEND_DELETE, &CMyFriendsDlg::OnFriendDelete)
	ON_COMMAND(ID_FRIEND_DELETE_CHAT_RECORD, &CMyFriendsDlg::OnFriendDeleteChatRecord)
	ON_COMMAND(ID_FRIEND_REFRESH_LIST, &CMyFriendsDlg::OnFriendRefreshList)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyFriendsDlg::OnBnClickedButton6)
	ON_LBN_DBLCLK(IDC_FRIENDS_LIST, &CMyFriendsDlg::OnLbnDblclkFriendsList)
	ON_MESSAGE(FRIEND_LIST_RESPONSE_DLG_MSG,&CMyFriendsDlg::OnFriendListResponse)
	ON_MESSAGE(FRIEND_SEARCH_RESPONSE_DLG_MSG,&CMyFriendsDlg::OnFriendSearchResponse)
	ON_MESSAGE(CHAT_RECEIVE_MESSAGE_DLG_MSG,&CMyFriendsDlg::OnReceiveChatMessageResponse)
	ON_MESSAGE(AGREE_ADD_FRIEND_RESPONSE_DLG_MSG,&CMyFriendsDlg::OnAgreeAddFriendResponse)
	ON_MESSAGE(DELETE_FRIEND_DLG_MSG,&CMyFriendsDlg::OnDeleteFriendResponse)
END_MESSAGE_MAP()


// CMyFriendsDlg 消息处理程序


BOOL CMyFriendsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_menu.LoadMenu(IDR_FRIEND_MENU);
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //这里将位图加载进资源后，再Load
    m_brush.CreatePatternBrush(&m_bmp);
	m_FriendListBox.SetMyMenu(&m_menu);
	this->RequestFriendList();
	m_PictureChild.GetWindowRect(&m_PictureRect);
	ScreenToClient(m_PictureRect);
	m_PictureChild.ShowWindow(SW_HIDE);
	/*m_chatdlg.Create(IDD_CHAT_DLG,this);
	m_chatdlg.MoveWindow(m_PictureRect);
	m_chatdlg.ShowWindow(SW_SHOW);*/
	m_ChatStatusEdit.SetWindowText(_T("聊天状态"));
	this->RequestOfflineMessage();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMyFriendsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_bmp.DeleteObject();
	m_brush.DeleteObject();
	m_menu.DestroyMenu();

	auto ite = m_friendinfoMap.begin();
	while(ite != m_friendinfoMap.end())
	{
		delete ite->second;
		ite->second = NULL;
		++ite;
	}
	m_friendinfoMap.clear();

	auto ite2 = m_chatdlgMap.begin();
	while(ite2 != m_chatdlgMap.end())
	{
		ite2->second->DestroyWindow();
		delete ite2->second;
		ite2->second = NULL;
		++ite2;
	}
	m_chatdlgMap.clear();
}


HBRUSH CMyFriendsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{              
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(150,200,60));
		//pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);               
	}
	return hbr;
}

//打开与该好友聊天的窗口
void CMyFriendsDlg::OnFriendSendMess()
{
	// TODO: 在此添加命令处理程序代码
	int nRow = m_FriendListBox.GetCurSel();
	if(nRow != -1)
	{
		UpdateData(FALSE);
		CString name;
		m_FriendListBox.GetText(nRow,name);
		CMyChatDlg* chat = NULL;
		if(m_chatdlgMap.count(name))
		{
			chat = m_chatdlgMap[name];
		}else{
			chat = new CMyChatDlg;
			chat->Create(IDD_CHAT_DLG,this);
			chat->MoveWindow(m_PictureRect);
			chat->SetFriendInfo(m_friendinfoMap[name]);
			m_chatdlgMap[name] = chat;
		}
		if(!m_chating.IsEmpty()){
			if(m_chating == name)
				return;
			m_chatdlgMap[m_chating]->ShowWindow(SW_HIDE);
		}
		if(chat){
			chat->ShowWindow(SW_SHOW);
			m_chating = name;
			this->ChatStatusUpdate(name);
		}
	}
}

//删除该好友
void CMyFriendsDlg::OnFriendDelete()
{
	// TODO: 在此添加命令处理程序代码
	int nRow = m_FriendListBox.GetCurSel();
	if(-1 == nRow)
		return;
	CString mark;
	m_FriendListBox.GetText(nRow,mark);
	auto info = m_friendinfoMap[mark];
	DeleteFriend df;
	memset(&df,0,sizeof(df));
	df.m_len = sizeof(df);
	df.m_type = DELETE_FRIEND_MSG;
	memcpy_s(df.m_sender,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(df.m_receiver,EMAIL_SIZE,info->m_email,strlen(info->m_email));
	
	theApp.net()->send((char*)&df,df.m_len);
	if(m_chatdlgMap.count(mark)){
		m_chatdlgMap[mark]->ShowWindow(SW_HIDE);
		m_chatdlgMap[mark]->DestroyWindow();
		delete m_chatdlgMap[mark];
		m_chatdlgMap[mark] = NULL;
		m_chatdlgMap.erase(mark);
	}
	delete m_friendinfoMap[mark];
	m_friendinfoMap[mark] = NULL;
	m_friendinfoMap.erase(mark);
	m_strfriendinfoMap.erase(df.m_receiver);
	m_FriendListBox.DeleteString(nRow);
}

//清空聊天记录
void CMyFriendsDlg::OnFriendDeleteChatRecord()
{
	// TODO: 在此添加命令处理程序代码
	m_chatdlgMap[this->m_chating]->ClearTextArea();
}

//刷新好友列表
void CMyFriendsDlg::OnFriendRefreshList()
{
	// TODO: 在此添加命令处理程序代码
	if(m_FriendListBox.GetCount() > 0)
		m_FriendListBox.ResetContent();
	this->RequestFriendList();
}

//搜索好友
void CMyFriendsDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	CString email;
	m_SearchEdit.GetWindowText(email);
	if(!email.IsEmpty())
	{
		string e;
		CMyTools::EnCodeWCHARToUtf(email.GetBuffer(),e);
		if(e == theApp.m_email)
		{
			MessageBox(_T("搜索账号为当前用户"));
			return;
		}
		if(CMyTools::MatchUid(e))
		{
			FriendSearchRq rq;
			memset(&rq,0,sizeof(rq));
			rq.m_len = sizeof(rq);
			rq.m_type = FRIEND_SEARCH_REQUEST_MSG;
			memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
			memcpy_s(rq.m_searchemail,EMAIL_SIZE,e.c_str(),e.size());
			theApp.net()->send((char*)&rq,rq.m_len);
		}else{
			MessageBox(_T("请输入正确的邮箱格式."));
		}
	}
}

void CMyFriendsDlg::OnLbnDblclkFriendsList()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnFriendSendMess();
}

//网络请求好友列表
void	CMyFriendsDlg::RequestFriendList()
{
	FriendListRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = FRIEND_LIST_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}

void	CMyFriendsDlg::ChatStatusUpdate(CString name)
{
	this->m_ChatStatusEdit.SetWindowText(_T("正在与 ") + name +_T(" 聊天中"));
	UpdateData();
}
//好友列表回复
LRESULT CMyFriendsDlg::OnFriendListResponse(WPARAM w, LPARAM l)
{
	FriendListRs* rs = (FriendListRs*)w;
	CString nick,email;
	for(int i = 0; i < rs->m_friendcount; i++)
	{
		CMyTools::EnCodeUtfToWCHAR(rs->m_friendlist[i].m_nickname,nick);
		CMyTools::EnCodeUtfToWCHAR(rs->m_friendlist[i].m_email,email);
		this->AddFriend(email,nick);
	}
	return 0;
}

LRESULT CMyFriendsDlg::OnFriendSearchResponse(WPARAM w, LPARAM l)
{
	FriendSearchRs* rs = (FriendSearchRs*)w;
	if(rs->m_statuscode == SEARCH_NONE){
		MessageBox(_T("搜索用户不存在"));
	}else if(rs->m_statuscode == SEARCH_SUCCESS){
		CString email,nick,mark;
		CMyTools::EnCodeUtfToWCHAR(rs->m_searchemail,email);
		CMyTools::EnCodeUtfToWCHAR(rs->m_searchnick,nick);
		mark = nick + _T("(") + email + _T(")");
		bool flag = false;
		if(m_friendinfoMap.count(mark))
			flag = true;
		CMyFriendSearchDlg searchdlg(this);
		searchdlg.SetInitParams(email,nick,flag);
		searchdlg.DoModal();
	}
	return 0;
}

LRESULT CMyFriendsDlg::OnReceiveChatMessageResponse(WPARAM w, LPARAM l)
{
	UserMessage* buff = (UserMessage*)w;
	if(buff->m_messagetype == CHAT_TYPE_ADDREQUEST)
	{//请求添加好友
		CString e,n;
		CMyTools::EnCodeUtfToWCHAR(buff->m_sender,e);
		CMyTools::EnCodeUtfToWCHAR(buff->m_data,n);
		CMyAddFriendRequestDlg dlg(this);
		dlg.SetInitParams(e,n);
		dlg.DoModal();
		return 0;
	}
	if(!m_strfriendinfoMap.count(buff->m_sender))
		return 0;
	auto info = m_strfriendinfoMap[buff->m_sender];
	CString nick,email,mark;
	CMyTools::EnCodeUtfToWCHAR(info->m_email,email);
	CMyTools::EnCodeUtfToWCHAR(info->m_nickname,nick);
	mark = nick + _T("(") + email + _T(")");
	CMyChatDlg* chat = NULL;
	if(m_chatdlgMap.count(mark)){
		chat = m_chatdlgMap[mark];
	}else{
		chat = new CMyChatDlg;
		chat->Create(IDD_CHAT_DLG,this);
		chat->MoveWindow(m_PictureRect);
		chat->SetFriendInfo(info);
		m_chatdlgMap[mark] = chat;
		if(this->IsWindowVisible() == FALSE){
			this->GetParent()->GetDlgItem(IDC_FRIEND_MESSAGE)->ShowWindow(SW_SHOW);
		}
	}
	CString text,out;
	CMyTools::EnCodeUtfToWCHAR(buff->m_date,out);
	CMyTools::EnCodeUtfToWCHAR(buff->m_data,text);
	chat->AddTextToChatArea(text,1,out);
	if(chat->IsWindowVisible() == FALSE){
		this->m_ChatStatusEdit.SetWindowText(_T("收到来自: ")+mark+_T("的消息"));
	}
	return 0;
}

void	CMyFriendsDlg::AddFriend( CString& email, CString& nick)
{
	CString content;
	string e,n;
	CMyTools::EnCodeWCHARToUtf(email.GetBuffer(),e);
	CMyTools::EnCodeWCHARToUtf(nick.GetBuffer(),n);
	content = nick + _T("(") + email + _T(")");
	m_FriendListBox.AddString(content);
	FriendInfo* info = new FriendInfo;
	memset(info,0,sizeof(FriendInfo));
	memcpy_s(info->m_email,EMAIL_SIZE,e.c_str(),e.size());
	memcpy_s(info->m_nickname,NICKNAME_SIZE,n.c_str(),n.size());
	m_friendinfoMap[content] = info;
	m_strfriendinfoMap[info->m_email] = info;
}

LRESULT CMyFriendsDlg::OnAgreeAddFriendResponse(WPARAM w, LPARAM l)
{
	AgreeAddFriend* aa = (AgreeAddFriend*)w;
	CString email,nick;
	CMyTools::EnCodeUtfToWCHAR(aa->m_sender,email);
	CMyTools::EnCodeUtfToWCHAR(aa->m_sendernick,nick);
	this->AddFriend(email,nick);
	return 0;
}

LRESULT CMyFriendsDlg::OnDeleteFriendResponse(WPARAM w, LPARAM l)
{
	DeleteFriend* df = (DeleteFriend*)w;
	if(m_strfriendinfoMap.count(df->m_sender))
	{
		auto info = m_strfriendinfoMap[df->m_sender];
		CString mark,email,nick;
		CMyTools::EnCodeUtfToWCHAR(info->m_email,email);
		CMyTools::EnCodeUtfToWCHAR(info->m_nickname,nick);
		mark = nick + _T("(") + email + _T(")");
		if(m_chatdlgMap.count(mark)){
			m_chatdlgMap[mark]->DestroyWindow();
			delete m_chatdlgMap[mark];
			m_chatdlgMap[mark] = NULL;
			m_chatdlgMap.erase(mark);
		}
		CString tmp;
		int nRow = m_FriendListBox.GetCount();
		for(int i = 0; i < nRow; i++){
			m_FriendListBox.GetText(i,tmp);
			if(tmp == mark){
				m_FriendListBox.DeleteString(i);
				UpdateData();
				break;
			}
		}
		m_strfriendinfoMap.erase(df->m_sender);
		if(m_friendinfoMap.count(mark)){
			delete m_friendinfoMap[mark];
			m_friendinfoMap[mark] = NULL;
			m_friendinfoMap.erase(mark);
		}

	}
	return 0;
}

void	CMyFriendsDlg::RequestOfflineMessage()
{
	RequestMessageRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = REQUEST_MESSAGE_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}