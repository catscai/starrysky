#pragma once
#include "packdef.h"
class TcpMediator;
typedef bool (TcpMediator::*FUN_Mediator)(char*,unsigned);

struct NetMsgMap
{
	MsgType			m_type;
	FUN_Mediator	m_fun;
};

class TcpNet;
class ThreadPool;
class TcpMediator
{
private:
	TcpNet*			m_net;
	ThreadPool*		m_pool;
	bool			m_IsReleaseFlag;
public:
	TcpMediator(void);
	~TcpMediator(void);
public:
	bool	connect();
	bool	send(char* szbuf,unsigned len);
	bool	dealNetMsg(char* szbuf,unsigned len);
	bool    close();
public:
	bool	RegisterResponse(char* szbuf,unsigned len);
	bool	LoginResponse(char* szbuf,unsigned len);
	bool	FileListResponse(char* szbuf,unsigned len);
	bool	UploadHeadResponse(char* szbuf,unsigned len);
	bool	UploadFileDataResponse(char* szbuf,unsigned len);
	bool	AddNewDirectoryResponse(char* szbuf,unsigned len);
	bool	DeleteFileResponse(char* szbuf,unsigned len);
	bool	DownloadFileHeadResponse(char* szbuf,unsigned len);
	bool	DownloadFileDataResponse(char* szbuf,unsigned len);
	bool	DownloadFileFinishResponse(char* szbuf,unsigned len);
	bool	FriendListResponse(char* szbuf,unsigned len);
	bool	FriendSearchResponse(char* szbuf,unsigned len);
	bool	ReceiveMessageResponse(char* szbuf,unsigned len);
	bool	AgreeAddFriendResponse(char*szbuf,unsigned len);
	bool	DeleteFriendResponse(char* szbuf,unsigned len);
	bool	ShareFileResponse(char* szbuf,unsigned len);
	bool	ShareFileListResponse(char* szbuf,unsigned len);
	bool	ExtractShareFileResponse(char* szbuf,unsigned len);
	bool	SaveFileResponse(char* szbuf,unsigned len);
	bool	UnlockResponse(char* szbuf,unsigned len);
};

