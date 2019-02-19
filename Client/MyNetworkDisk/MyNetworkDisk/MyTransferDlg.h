#pragma once
#include "MyTransferDownloadDlg.h"
#include "MyTransferUploadDlg.h"
#include "MyTransferFinishDlg.h"
#include "afxwin.h"

// CMyTransferDlg 对话框

class CMyTransferDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTransferDlg)

public:
	CMyTransferDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTransferDlg();

// 对话框数据
	enum { IDD = IDD_TRANSTER_DLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CMyTransferDownloadDlg		m_downloaddlg;
	CMyTransferUploadDlg		m_uploaddlg;
	CMyTransferFinishDlg		m_finishdlg;
	CStatic						m_pictureWindow;
	CRect						m_pictureRect;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void	HideAllListControl();
	void    RefreshButtonText();
	afx_msg void OnBnClickedDownloading();
	afx_msg void OnBnClickedUploading();
	afx_msg void OnBnClickedTransferFinish();
};
