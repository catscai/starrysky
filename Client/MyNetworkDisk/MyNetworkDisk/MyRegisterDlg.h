#pragma once
#include "afxwin.h"


// CMyRegisterDlg 对话框

class CMyRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyRegisterDlg)

public:
	CMyRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyRegisterDlg();

// 对话框数据
	enum { IDD = IDD_REGISTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //位图
    CBrush			m_brush;  //画刷
	CFont*          m_fon1;
	CFont*			m_fon2;
	CEdit			m_nickNameEdit;
	CEdit			m_passwdEdit1;
	CEdit			m_passwdEdit2;
	CEdit			m_emailEdit;
	CString			m_securityCodeStr;
	CButton			m_securityBtn;
public:

	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRegisterCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCaptchaBtn();
	afx_msg void OnBnClickedRegisterOk();
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void	HidePromptWindow();
};
