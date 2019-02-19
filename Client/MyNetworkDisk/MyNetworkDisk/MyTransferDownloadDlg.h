#pragma once
#include "afxcmn.h"
#include "ProcessList.h"
#include<map>
using namespace std;
// CMyTransferDownloadDlg 对话框
struct DownloadFileInfo;
class CMyTransferDownloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTransferDownloadDlg)

public:
	CMyTransferDownloadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTransferDownloadDlg();

// 对话框数据
	enum { IDD = IDD_TRANSFER_DOWNLOAD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProcessList		m_DownloadList;
	LVFINDINFO			m_info;
private:
	map<CString,DownloadFileInfo*>	m_DownLoadMap;
	CMenu							m_menu;
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void	AddDownTask(DownloadFileInfo* info);
	void	FinishTask(DownloadFileInfo* info);
	void	PauseTask(DownloadFileInfo* info);
	void	UpdateRate(DownloadFileInfo* info);
	void	DeleteTask(DownloadFileInfo* info);
	afx_msg void OnNMRClickDownloadList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDownloadPause();
	afx_msg void OnDownloadContinue();
	afx_msg void OnDownloadDelete();
	afx_msg void OnDownloadPauseAll();
	afx_msg void OnDownloadContinueAll();
	afx_msg void OnDownloadDeleteAll();
	afx_msg void OnNMClickDownloadList(NMHDR *pNMHDR, LRESULT *pResult);
};
