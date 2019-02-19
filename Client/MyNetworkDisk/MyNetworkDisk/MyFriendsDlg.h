#pragma once
#include "afxwin.h"
#include "FriendList.h"
#include "MyChatDlg.h"
// CMyFriendsDlg �Ի���
#include <map>
#include <list>
using namespace std;
struct FriendInfo;
class CMyFriendsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFriendsDlg)

public:
	CMyFriendsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyFriendsDlg();

// �Ի�������
	enum { IDD = IDD_FRIENDS_CHAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
	CMenu			m_menu;
	CEdit			m_SearchEdit;
	CFriendList		m_FriendListBox;
	CEdit			m_ChatStatusEdit;
	CStatic			m_PictureChild;
	CRect			m_PictureRect;
private:
	//list<FriendInfo*>			m_friendlist;
	map<CString,CMyChatDlg*>	m_chatdlgMap;
	map<string,FriendInfo*>		m_strfriendinfoMap;
	map<CString,FriendInfo*>	m_friendinfoMap;
	CString						m_chating;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnFriendSendMess();
	afx_msg void OnFriendDelete();
	afx_msg void OnFriendDeleteChatRecord();
	afx_msg void OnFriendRefreshList();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnLbnDblclkFriendsList();
	void	RequestFriendList();
	void	RequestOfflineMessage();
	void	ChatStatusUpdate(CString name);
	void	AddFriend( CString& email, CString& nick);
	afx_msg LRESULT OnFriendListResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnFriendSearchResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnReceiveChatMessageResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnAgreeAddFriendResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDeleteFriendResponse(WPARAM w, LPARAM l);
};
