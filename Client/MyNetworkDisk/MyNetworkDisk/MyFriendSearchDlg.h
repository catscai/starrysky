#pragma once
#include "afxwin.h"


// CMyFriendSearchDlg �Ի���

class CMyFriendSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyFriendSearchDlg)

public:
	CMyFriendSearchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyFriendSearchDlg();

// �Ի�������
	enum { IDD = IDD_FRIEND_SEARCH_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit			m_SearchEmail;
	CEdit			m_SearchNickName;
	CString			m_searchemail;
	CString			m_searchnick;
	bool			m_IsFriendFlag;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	void SetInitParams( CString e, CString n,bool flag = false);
	virtual BOOL OnInitDialog();
};
