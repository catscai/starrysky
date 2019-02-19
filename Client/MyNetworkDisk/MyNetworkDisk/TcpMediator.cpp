#include "stdafx.h"
#include "TcpMediator.h"
#include "TcpNet.h"
#include "ThreadPool.h"
#include "MyNetworkDisk.h"
#include "MyMainDlg.h"
#include "MyTools.h"

TcpMediator* m_mediator = NULL;
TcpMediator::TcpMediator(void)
{
	m_net = new TcpNet(this);
	m_pool = new ThreadPool;
	m_pool->CreateThreadPool(2,5,50);
	m_IsReleaseFlag = false;
	m_mediator = this;
}


TcpMediator::~TcpMediator(void)
{
	this->close();
}

static NetMsgMap  g_NetMsgMap[]=
{
	{REGISTER_RESPONSE_MSG,&TcpMediator::RegisterResponse},
	{LOGIN_RESPONSE_MSG,&TcpMediator::LoginResponse},
	{FILELIST_RESPONSE_MSG,&TcpMediator::FileListResponse},
	{UPLOAD_FILEHEAD_RESPONSE_MSG,&TcpMediator::UploadHeadResponse},
	{UPLOAD_FILERESPONSE_MSG,&TcpMediator::UploadFileDataResponse},
	{ADD_NEW_DIRECTORY_RESPONSE_MSG,&TcpMediator::AddNewDirectoryResponse},
	{DELETE_FILE_RESPONSE_MSG,&TcpMediator::DeleteFileResponse},
	{DOWNLOAD_FILEHEAD_RESPONSE_MSG,&TcpMediator::DownloadFileHeadResponse},
	{DOWNLOAD_FILE_RESPONSE_MSG,&TcpMediator::DownloadFileDataResponse},
	{DOWNLOAD_FILEFINISH_MSG,&TcpMediator::DownloadFileFinishResponse},
	{FRIEND_LIST_RESPONSE_MSG,&TcpMediator::FriendListResponse},
	{FRIEND_SEARCH_RESPONSE_MSG,&TcpMediator::FriendSearchResponse},
	{CHAT_RECEIVE_MESSAGE_MSG,&TcpMediator::ReceiveMessageResponse},
	{AGREE_ADD_FRIEND_RESPONSE_MSG,&TcpMediator::AgreeAddFriendResponse},
	{DELETE_FRIEND_MSG,&TcpMediator::DeleteFriendResponse},
	{SHARE_FILE_RESPONSE_MSG,&TcpMediator::ShareFileResponse},
	{SHARE_FILE_LIST_RESPONSE_MSG,&TcpMediator::ShareFileListResponse},
	{EXTRACT_SHARE_FILE_RESPONSE_MSG,&TcpMediator::ExtractShareFileResponse},
	{SAVE_SHARE_FILE_RESPONSE_MSG,&TcpMediator::SaveFileResponse},
	{UNLOCK_MAINDLG_RESPONSE_MSG,&TcpMediator::UnlockResponse},
	{0,0}
};

class Task : public MyTask
{
private:
	char			szbuf[MAX_BUFFER_SIZE];
	unsigned			len;
	FUN_Mediator	fun;
public:
	Task(char* buf,unsigned size,FUN_Mediator f):fun(f){
		memcpy_s(szbuf,MAX_BUFFER_SIZE,buf,size);
		len = size;
	}
public:
	virtual void ExecTask()
	{
		(m_mediator->*fun)(szbuf,len);
	}
};

bool	TcpMediator::connect()
{
	return m_net->Connection();
}

bool	TcpMediator::send(char* szbuf,unsigned len)
{
	return m_net->send(szbuf,len);
}

bool	TcpMediator::dealNetMsg(char* szbuf,unsigned len)
{
	MsgType* type = (MsgType*)(szbuf + 4);
	int pos = 0;
	while(true)
	{
		if(*type == g_NetMsgMap[pos].m_type){
			Task* t = new Task(szbuf,len,g_NetMsgMap[pos].m_fun);
			m_pool->AddTask(t);
			break;
		}else if(0 == g_NetMsgMap[pos].m_type){
			break;
		}
		++pos;
	}
	return true;
}

bool    TcpMediator::close()
{
	if(m_IsReleaseFlag == false)
	{
		m_IsReleaseFlag = true;
		m_net->close();
		m_pool->DestroyThreadPool();
		delete m_net;
		delete m_pool;
		m_net = NULL;
		m_pool = NULL;
	}
	return true;
}

bool	TcpMediator::RegisterResponse(char* szbuf,unsigned len)
{
	RegisterRs* rs = (RegisterRs*)szbuf;
	if(rs->m_statuscode == REGISTER_OK)
	{
		AfxMessageBox(_T("注册成功"));
	}
	return true;
}

bool	TcpMediator::LoginResponse(char* szbuf,unsigned len)
{
	theApp.m_pMainWnd->SendMessage(LOGIN_SUCESS_DLG_MSG,(WPARAM)szbuf,NULL);
	return true;
}

bool	TcpMediator::FileListResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(FILELIST_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::UploadHeadResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(UPLOAD_FILEHEAD_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::UploadFileDataResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(UPLOAD_FILEDATA_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

//新建文件夹回复
bool	TcpMediator::AddNewDirectoryResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(ADD_NEW_DIRECTORY_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::DeleteFileResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(DELETE_FILE_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::DownloadFileHeadResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(DOWNLOAD_FILEHEAD_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::DownloadFileDataResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(DOWNLOAD_FILEDATA_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::DownloadFileFinishResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyWangPanDlg.SendMessage(DOWNLOAD_FILE_FINISH_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::FriendListResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyFriendsDlg.SendMessage(FRIEND_LIST_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::FriendSearchResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyFriendsDlg.SendMessage(FRIEND_SEARCH_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::ReceiveMessageResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyFriendsDlg.SendMessage(CHAT_RECEIVE_MESSAGE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::AgreeAddFriendResponse(char*szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyFriendsDlg.SendMessage(AGREE_ADD_FRIEND_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::DeleteFriendResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyFriendsDlg.SendMessage(DELETE_FRIEND_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::ShareFileResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyShareDlg.m_sharedlg.SendMessage(SHARE_FILE_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::ShareFileListResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyShareDlg.SendMessage(SHARE_FILE_LIST_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::ExtractShareFileResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyExtractDlg.SendMessage(EXTRACT_FILE_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::SaveFileResponse(char* szbuf,unsigned len)
{
	CMyMainDlg* mainDlg = (CMyMainDlg*)theApp.m_pMainWnd;
	mainDlg->m_MyExtractDlg.SendMessage(SAVE_FILE_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}

bool	TcpMediator::UnlockResponse(char* szbuf,unsigned len)
{
	theApp.m_pMainWnd->SendMessage(UNLOCK_MAINDLG_RESPONSE_DLG_MSG,(WPARAM)szbuf,0);
	return true;
}