#pragma once
#include "afxwin.h"


// CMyChatDlg �Ի���
struct FriendInfo;
class CMyChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyChatDlg)

public:
	CMyChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyChatDlg();

// �Ի�������
	enum { IDD = IDD_CHAT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	FriendInfo*			m_friendinfo;
	CEdit				m_ChatTextAreaEdit;
	CEdit				m_SendTextAreaEdit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void	SetFriendInfo(FriendInfo* info);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	void	AddTextToChatArea(const CString& text,int mark,CString t = _T(""));
	void	ClearTextArea();
};
