#pragma once
#include "afxwin.h"


// CMyLockDlg �Ի���

class CMyLockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyLockDlg)

public:
	CMyLockDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyLockDlg();

// �Ի�������
	enum { IDD = IDD_LOCK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CEdit			m_pass;
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
	CFont			fon1,fon2;
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnUnlockResponse(WPARAM w, LPARAM l);
	afx_msg void OnStnClickedLockClose();
	afx_msg void OnStnClickedLockMin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
