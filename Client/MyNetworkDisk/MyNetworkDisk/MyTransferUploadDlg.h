#pragma once
#include "afxcmn.h"
#include "ProcessList.h"
#include <map>
using namespace std;
// CMyTransferUploadDlg 对话框
struct UploadFileInfo;
class CMyTransferUploadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTransferUploadDlg)

public:
	CMyTransferUploadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTransferUploadDlg();

// 对话框数据
	enum { IDD = IDD_TRANSFER_UPLOAD_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CProcessList			m_UploadList;
	LVFINDINFO				m_info;
private:
	map<CString,UploadFileInfo*>	m_UploadMap;
	CMenu							m_menu;
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void	AddUploadTask(UploadFileInfo* info);
	void	FinishTask(UploadFileInfo* info);
	void	PauseTask(UploadFileInfo* info);
	void	UpdateRate(UploadFileInfo* info);
	void	DeleteTask(UploadFileInfo* info);
	afx_msg void OnUploadPause();
	afx_msg void OnUploadContinue();
	afx_msg void OnUploadDelete();
	afx_msg void OnUploadPauseAll();
	afx_msg void OnUploadContinueAll();
	afx_msg void OnUploadDeleteAll();
	afx_msg void OnNMRClickUploadList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickUploadList(NMHDR *pNMHDR, LRESULT *pResult);
};
