#pragma once
#include "afxcmn.h"
#include "MyShareFileDlg.h"
#include <map>
using namespace std;
// CMyShareDlg 对话框
struct ShareFileInfo;
class CMyShareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyShareDlg)

public:
	CMyShareDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyShareDlg();

// 对话框数据
	enum { IDD = IDD_MYSHARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //位图
    CBrush			m_brush;  //画刷
	CListCtrl		m_ShareFileListControl;
	map<CString,ShareFileInfo*>		m_sharefileinfoMap;	
public:
	CMyShareFileDlg m_sharedlg;
public:
	
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	void	ShowShareFileDlg();
	void	SetShareFileDlgFName(const CString& path);
	void	SetShareFileDlgParams(const CString& fdir,const CString& fname);
	bool	IsShareShowing();
	void	AddShareFile(const CString& fname,const CString& sharetime,const 
		CString& days,const CString& savetime,const CString& getcode);
	void	RequestShareFileList();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg LRESULT OnShareFileListResponse(WPARAM w, LPARAM l);
	afx_msg void OnNMClickShareFileList(NMHDR *pNMHDR, LRESULT *pResult);
};
