#pragma once
#include "afxwin.h"


// CMyAddFriendRequestDlg �Ի���

class CMyAddFriendRequestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyAddFriendRequestDlg)

public:
	CMyAddFriendRequestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyAddFriendRequestDlg();

// �Ի�������
	enum { IDD = IDD_REQUEST_ADD_FRIEND_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit			m_RequestEmail;
	CEdit			m_RequestNick;
	CString			m_requestemail;
	CString			m_requestnick;
public:
	
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	void	SetInitParams(const CString& email,const CString& nick);
	virtual BOOL OnInitDialog();
};
