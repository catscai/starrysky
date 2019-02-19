#pragma once
#include "afxwin.h"


// CMyShareFileDlg 对话框
class CMyShareDlg;
class CMyShareFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyShareFileDlg)

public:
	CMyShareFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyShareFileDlg();

// 对话框数据
	enum { IDD =IDD_SHARE_FILE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	int					m_RadioGroup;
	int					m_days;
	CEdit				m_ShareFileNameEdit;
	CEdit				m_ShareGetCodeEdit;
	CString				m_ShareFileName;
	CString				m_ShareGetCode;
	CString				m_ShareFileDir;
	CFont				m_font1,m_font2;
	CMyShareDlg*		m_parent;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void	SetInitParams(const CString& name);
	void	SetDirName(const CString& fdir,const CString& fname);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg LRESULT OnShareFileResponse(WPARAM w, LPARAM l);
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
