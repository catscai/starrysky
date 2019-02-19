#pragma once
#include "afxwin.h"


// CMyLockDlg 对话框

class CMyLockDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyLockDlg)

public:
	CMyLockDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyLockDlg();

// 对话框数据
	enum { IDD = IDD_LOCK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CEdit			m_pass;
	CBitmap			m_bmp;   //位图
    CBrush			m_brush;  //画刷
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
