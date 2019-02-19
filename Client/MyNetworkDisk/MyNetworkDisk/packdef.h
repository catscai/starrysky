
#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <string>
#include <fstream>
using namespace std;
#define MAX_BUFFER_SIZE 10240
#define SERVER_PORT 7799
typedef int MsgType;

//ע����Ϣ�ֶγ���
#define NICKNAME_SIZE 40
#define EMAIL_SIZE 60
#define PASSWD_SIZE 20
#define SECURITY_CODE_SIZE 10


//ע������������Ϣ
#define REGISTER_REQUEST_MSG 1
struct RegisterRq
{
	unsigned    m_len;
	MsgType		m_type;
	char		m_nickname[NICKNAME_SIZE];
	char		m_email[EMAIL_SIZE];
	char		m_pass[PASSWD_SIZE];
	char		m_securitycode[SECURITY_CODE_SIZE];
};
//ע��ظ�
#define REGISTER_RESPONSE_MSG 2
#define REGISTER_OK 1
#define REGISTER_REGISTERED 2
#define REGISTER_ERROR 3
struct  RegisterRs
{
	unsigned    m_len;
	MsgType		m_type;
	int 		m_statuscode;
};

//��¼����
#define LOGIN_REQUEST_MSG 3
struct LoginRq
{
	unsigned    m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_pass[PASSWD_SIZE];
};
//��¼�ظ�
#define LOGIN_RESPONSE_MSG 4
#define LOGIN_OK 1
#define LOGIN_UEXIST 2
#define LOGIN_PASSERROR 3
struct LoginRs
{
	unsigned    m_len;
	MsgType		m_type;
	int			m_statuscode;
	char		m_nickname[NICKNAME_SIZE];
	int			m_vipclass;
	long long	m_currcapacity;
	long long	m_capacity;
};

//�ļ��������
//�����ļ��б�
#define FILE_MD5CODE_SIZE 100
#define	FILENAME_LENGTH 100
#define FILENUM_IN_FILELIST 30
#define FILE_PATH_LENGTH 1000
#define UPLOAD_FILEDATA_SIZE 8000
#define FILELIST_REQUEST_MSG 5
#define DIR_SIZE   300		//�ļ��д�С ������ļ����������ļ�
struct	FileListRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_dir[DIR_SIZE];
	char		m_email[EMAIL_SIZE];
};


struct	FileInfo
{
	long long	m_filesize;	//����СΪ0������Ϊ��Ŀ¼
	char		m_name[FILENAME_LENGTH];
};

#define FILELIST_RESPONSE_MSG 6

struct	FileListRs
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_dir[DIR_SIZE];
	unsigned    m_filecount;
	FileInfo	m_filelist[FILENUM_IN_FILELIST];
};

//�ϴ��ļ�
#define UPLOAD_FILEREQUEST_MSG 7

struct	UploadFileDataRq
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_seek;
	int			m_datalen;
	char		m_email[EMAIL_SIZE];
	char		m_md5code[FILE_MD5CODE_SIZE];
	char		m_filedata[UPLOAD_FILEDATA_SIZE];
};

#define UPLOAD_FILERESPONSE_MSG 777
struct UploadFileDataRs
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_seek;
	char		m_md5code[FILE_MD5CODE_SIZE];
};

//�ļ��������
#define UPLOAD_FILEFINISH_MSG 8

struct	UploadFileEnd
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_filesize;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
	char		m_md5code[FILE_MD5CODE_SIZE];
};

//�ϴ��ļ�����ͷ
#define UPLOAD_FILEHEAD_REQUEST_MSG 9
struct UploadFileHeadRq
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_filesize;
	long long	m_seek;
	char		m_md5code[FILE_MD5CODE_SIZE];
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};

#define UPLOAD_FILEHEAD_RESPONSE_MSG 10
#define UPLOAD_RES_START 0
#define UPLOAD_RES_EXIST 1
#define UPLOAD_RES_SPACE_LESS 2
#define UPLOAD_RES_NAMECONFLICT 3
struct UploadFileHeadRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_statuscode;
	char		m_md5code[FILE_MD5CODE_SIZE];
};

//--------------------------
#define FILE_WAIT 0
#define FILE_TRANSFER 1
#define FILE_PAUSE 2
#define FILE_FINISH 3
#define FILE_DELETE 4
struct UploadFileInfo
{
	fstream     m_input;
	string		m_name;
	string		m_dir;
	string		m_curdir;
	string		m_p;
	string		m_md5code;
	long long	m_filesize;
	long long	m_seek;
	int			m_nIteam;
	int			m_statuscode;
	UploadFileInfo(){
		m_seek = 0;
		m_nIteam = 0;
		m_statuscode = FILE_WAIT;
	}
	~UploadFileInfo(){
		if(m_input.is_open())
			m_input.close();
	}
};

struct DownloadFileInfo
{
	fstream		m_output;
	string		m_name;
	string		m_dir;
	string		m_curdir;
	long long	m_seek;
	long long	m_filesize;
	int			m_nIteam;
	int			m_statuscode;
	DownloadFileInfo(){
		m_seek = 0;
		m_nIteam = 0;
		m_statuscode = FILE_WAIT;
	}
	~DownloadFileInfo(){
		if(m_output.is_open())
			m_output.close();
	}
};
//--------------------------

//�½��ļ���
#define ADD_NEW_DIRECTORY_REQUEST_MSG 11
struct AddNewDirectoryRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_newdir[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};
#define ADD_NEW_DIRECTORY_RESPONSE_MSG 12
#define ADD_DIRECTORY_RES_OK 0
#define ADD_DIRECTORY_RES_EXIST 1
struct AddNewDirectoryRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_statuscode;
};

//ɾ���ļ����ļ���
#define DELETE_FILE_REQUEST_MSG 13
#define DIRECTORY_TYPE 0
#define FILE_TYPE 1
struct DeleteFileRq
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_filetype;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};

#define DELETE_FILE_RESPONSE_MSG 14
#define DELETE_FILE_RES_OK 0
#define DELETE_FILE__RES_FAILED 1
struct DeleteFileRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_statuscode;
	unsigned	m_filesize;
};

//���ء������ļ����͡��ڿͻ�����Ҫ��Ŀ¼���ؽ��д���
#define DOWNLOAD_FILEHEAD_REQUEST_MSG 15
#define DOWNLOAD_HEAD_RES_OK 0
#define DOWNLOAD_HEAD_RES_ERROR 1
struct DownloadFileHeadRq
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_filetype;
	long long	m_seek;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};
#define DOWNLOAD_FILEHEAD_RESPONSE_MSG 16
struct DownloadFileHeadRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_statuscode;
	long long	m_filesize;
	long long	m_seek;
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
	char		m_md5code[FILE_MD5CODE_SIZE];
};
//�����ļ�����
#define DOWNLOAD_FILE_REQUEST_MSG 17
#define DOWNLOAD_FILEDATA_SIZE 8000
struct DownloadFileDataRq
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_seek;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};
#define DOWNLOAD_FILE_RESPONSE_MSG 18
struct DownloadFileDataRs
{
	unsigned	m_len;
	MsgType		m_type;
	long long	m_seek;
	int			m_datalen;
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
	char		m_filedata[DOWNLOAD_FILEDATA_SIZE];
};

#define DOWNLOAD_FILEFINISH_MSG 19
struct DownloadFileFinish
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};

#define UPLOAD_PAUSE_REQUEST_MSG 20
struct UploadPause
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_md5code[FILE_MD5CODE_SIZE];
};

#define DOWNLOAD_DELETE_REQUEST_MSG 21
struct DownloadDelete
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
};

#define UPLOAD_DELETE_REQUEST_MSG 22
struct UploadDelete
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_dir[DIR_SIZE];
	char		m_md5code[FILE_MD5CODE_SIZE];
};

//�����б�����
#define FRIEND_LIST_REQUEST_MSG 23
struct FriendListRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
};
#define FRIEND_NUM_PER_PACKET 50
struct	FriendInfo
{
	char	m_email[EMAIL_SIZE];
	char	m_nickname[NICKNAME_SIZE];
};
#define FRIEND_LIST_RESPONSE_MSG 24
struct FriendListRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_friendcount;
	FriendInfo  m_friendlist[FRIEND_NUM_PER_PACKET];
};

#define FRIEND_SEARCH_REQUEST_MSG 25
struct FriendSearchRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_searchemail[EMAIL_SIZE];
};
#define FRIEND_SEARCH_RESPONSE_MSG 26
#define SEARCH_NONE 0
#define SEARCH_SUCCESS 1
struct FriendSearchRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_statuscode;
	char		m_searchemail[EMAIL_SIZE];
	char		m_searchnick[NICKNAME_SIZE];
};

#define CHAT_SEND_MESSAGES_MSG 27
#define CHAT_RECEIVE_MESSAGE_MSG 28
#define CHAT_MESSAGE_LENGTH 500
#define CHAT_MESSAGE_DATETIME_LENGTH 100
#define CHAT_TYPE_NORMAL 0
#define CHAT_TYPE_ADDREQUEST 1
struct UserMessage
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_sender[EMAIL_SIZE];
	char		m_receiver[EMAIL_SIZE];
	char		m_data[CHAT_MESSAGE_LENGTH];
	int			m_messagetype;
	char		m_date[CHAT_MESSAGE_DATETIME_LENGTH];
};

#define AGREE_ADD_FRIEND_RESPONSE_MSG 29
struct AgreeAddFriend
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_sender[EMAIL_SIZE];
	char		m_receiver[EMAIL_SIZE];
	char		m_sendernick[NICKNAME_SIZE];
};

#define DELETE_FRIEND_MSG 30
struct DeleteFriend
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_sender[EMAIL_SIZE];
	char		m_receiver[EMAIL_SIZE];
};

#define REQUEST_MESSAGE_MSG 31
struct RequestMessageRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
};

#define SHARE_FILE_REQUEST_MSG 32
struct ShareFileRq
{
	unsigned	m_len;
	MsgType		m_type;
	char		m_email[EMAIL_SIZE];
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
	int			m_days;
};

#define SHARE_FILE_RESPONSE_MSG 33
#define SHARE_FILE_GETCODE_LENGTH 10
#define SHARE_SUCCESS 0
#define SHARE_EXIST 1
#define SHARE_ERROR 2
struct ShareFileRs
{
	unsigned	m_len;
	MsgType		m_type;		
	int			m_statuscode;
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_getcode[SHARE_FILE_GETCODE_LENGTH];
};

#define SHARE_FILE_LIST_REQUEST_MSG 34
struct ShareFileListRq
{
	unsigned	m_len;
	MsgType		m_type;	
	char		m_email[EMAIL_SIZE];
};
#define SHARE_START_TIME_LENGTH 30
struct ShareFileInfo
{
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_starttime[SHARE_START_TIME_LENGTH];
	char		m_getcode[SHARE_FILE_GETCODE_LENGTH];
	int			m_savetime;
	int			m_days;
};
#define SHARE_FILE_LIST_RESPONSE_MSG 35
#define SHARE_FILE_LIST_LENGTH 20
struct ShareFileListRs
{
	unsigned	m_len;
	MsgType		m_type;
	int			m_sharelistcount;
	ShareFileInfo	m_sharelist[SHARE_FILE_LIST_LENGTH];
};

#define SHARE_FILE_CANCEL_MSG 36
struct ShareFileDelete
{
	unsigned	m_len;
	MsgType		m_type;	
	char		m_email[EMAIL_SIZE];
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
};

#define EXTRACT_SHARE_FILE_REQUEST_MSG 37
struct ExtractFileRq
{
	unsigned	m_len;
	MsgType		m_type;	
	char		m_email[EMAIL_SIZE];
	char		m_otheremail[EMAIL_SIZE];
	char		m_getcode[SHARE_FILE_GETCODE_LENGTH];
};
#define EXTRACT_SHARE_FILE_RESPONSE_MSG 38
#define EXTRACT_OK 0
#define EXTRACT_ERROR 1
struct ExtractFileRs
{
	unsigned	m_len;
	MsgType		m_type;	
	int			m_statuscode;
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
	int			m_filetype;
};

#define SAVE_SHARE_FILE_REQUEST_MSG 39
struct SaveShareFileRq
{
	unsigned	m_len;
	MsgType		m_type;	
	char		m_email[EMAIL_SIZE];
	char		m_otheremail[EMAIL_SIZE];
	char		m_dir[DIR_SIZE];
	char		m_name[FILENAME_LENGTH];
	char		m_getcode[SHARE_FILE_GETCODE_LENGTH];
	char		m_mydir[DIR_SIZE];
};
#define SAVE_SHARE_FILE_RESPONSE_MSG 40
#define SAVE_SUCCESS 0
#define SAVE_FAILED 1
#define SAVE_DIR_UNEXIST 2
#define SAVE_CAPACITY_LESS 3
struct SaveShareFileRs
{
	unsigned	m_len;
	MsgType		m_type;	
	int			m_statuscode;
};

#define UNLOCK_MAINDLG_REQUEST_MSG 41
struct UnlockRq
{
	unsigned	m_len;
	MsgType		m_type;	
	char		m_email[EMAIL_SIZE];
	char		m_pass[PASSWD_SIZE];
};
#define UNLOCK_MAINDLG_RESPONSE_MSG 42
#define UNLOCK_SUCCESS 0
#define UNLOCK_FAILED 1
struct UnlockRs
{
	unsigned	m_len;
	MsgType		m_type;	
	int			m_statuscode;
};
//---------------------------------------------
//��¼�ɹ�
#define LOGIN_SUCESS_DLG_MSG WM_USER + 1
//�ļ��б�ظ�
#define FILELIST_RESPONSE_DLG_MSG WM_USER + 2
//�ϴ��ļ�ͷ�ظ�
#define UPLOAD_FILEHEAD_RESPONSE_DLG_MSG WM_USER + 3
//�½��ļ��лظ�
#define ADD_NEW_DIRECTORY_RESPONSE_DLG_MSG WM_USER + 4
//ɾ���ļ��ظ�
#define DELETE_FILE_RESPONSE_DLG_MSG WM_USER + 5
//�����ļ�ͷ�ظ�
#define DOWNLOAD_FILEHEAD_RESPONSE_DLG_MSG WM_USER + 6
//�����ļ����ݻظ�
#define DOWNLOAD_FILEDATA_RESPONSE_DLG_MSG WM_USER + 7
//�����ļ���ɻظ�
#define DOWNLOAD_FILE_FINISH_RESPONSE_DLG_MSG WM_USER + 8
//�ϴ��ļ����ݻظ�
#define UPLOAD_FILEDATA_RESPONSE_DLG_MSG WM_USER + 9
//�����б�ظ�
#define FRIEND_LIST_RESPONSE_DLG_MSG	WM_USER + 10
//�˺������ظ�
#define FRIEND_SEARCH_RESPONSE_DLG_MSG WM_USER + 11
//�յ���Ϣ
#define CHAT_RECEIVE_MESSAGE_DLG_MSG WM_USER + 12
//������Ӻ���
#define AGREE_ADD_FRIEND_RESPONSE_DLG_MSG WM_USER + 13
//ɾ������
#define DELETE_FRIEND_DLG_MSG WM_USER + 14
//�����ļ��ظ�
#define SHARE_FILE_RESPONSE_DLG_MSG WM_USER + 15
//�����ļ��б�ظ�
#define SHARE_FILE_LIST_RESPONSE_DLG_MSG WM_USER + 16
//��ȡ�ļ��ظ�
#define EXTRACT_FILE_RESPONSE_DLG_MSG WM_USER + 17
//�����ļ��ظ�
#define SAVE_FILE_RESPONSE_DLG_MSG WM_USER + 18
//�����ظ�
#define UNLOCK_MAINDLG_RESPONSE_DLG_MSG WM_USER + 19
#endif