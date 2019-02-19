#pragma once
#include "afxwin.h"


// CMyNewDirectoryDlg �Ի���

class CMyNewDirectoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyNewDirectoryDlg)

public:
	CMyNewDirectoryDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyNewDirectoryDlg();

// �Ի�������
	enum { IDD = IDD_NEW_DIRECTORY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit		m_DirEdit;
	CEdit		m_NewDirEdit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedDirectoryOk();
	afx_msg void OnBnClickedDirectoryCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
