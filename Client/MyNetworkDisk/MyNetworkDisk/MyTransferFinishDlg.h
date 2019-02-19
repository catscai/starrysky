#pragma once
#include "afxcmn.h"
#include <string>
#include <stack>
using namespace std;

// CMyTransferFinishDlg 对话框
struct UploadFileInfo;
struct DownloadFileInfo;
struct FinishRecord
{
	string		name;
	string  	size;
	string		status;
	string		time;
	FinishRecord(const string& a,const string& b,const string& c,
		const string& d):name(a),size(b),status(c),time(d)
	{}
	FinishRecord(){}
};
class CMyTransferFinishDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyTransferFinishDlg)

public:
	CMyTransferFinishDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyTransferFinishDlg();

// 对话框数据
	enum { IDD = IDD_TRANSFER_FINISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl				m_FinishList;
	stack<FinishRecord*>	m_FinishStack;
	CMenu					m_menu;
public:
	
	virtual BOOL OnInitDialog();
	void	AddUploadFinishItemToList(UploadFileInfo* up);
	void	AddDownloadFinishItemToList(DownloadFileInfo* down);
	void	InsertFinishListItem(const CString& n,const CString& s,const CString& ss,const CString& t);
	afx_msg void OnNMClickFinishList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnNMRClickFinishList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFinisgClear();
};
