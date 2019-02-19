#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "packdef.h"
#include <map>
#include <list>
using namespace std;
// CMyWangPanDlg 对话框
class ThreadPool;
class CMyMainDlg;
class CMyWangPanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyWangPanDlg)

public:
	CMyWangPanDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyWangPanDlg();

// 对话框数据
	enum { IDD = IDD_MYWANGPAN_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CFont			fon1,fon2,fon3;
	CEdit			m_PathEdit;
	CListCtrl		m_FileListControl;
	CImageList		m_ImageList;
	ThreadPool*		m_pool;
	CMyMainDlg*		m_parent;
private:
	map<string,UploadFileInfo*>		m_uploadFileMap;
	map<string,DownloadFileInfo*>	m_downloadFileMap;
	list<DownloadFileDataRq>		m_downLst;
	list<UploadFileDataRq>			m_upLst;
	int								m_downtime;
	int								m_uptime;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	void	SetStaticFont();
	void	AddHeaderImage();
	void	RefreshPath(CString path);
	void	InsertPicAndWord(int PicIndex,const CString& fname,const CString& size);
	afx_msg void OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg LRESULT OnFileListResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUploadFileHeadResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnUploadFileDataResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnAddNewDirectoryResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDeleteFileResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDownloadFileHeadResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDownloadFileDataResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnDownloadFileFinishResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnShareFileResponse(WPARAM w, LPARAM l);
	afx_msg void OnStnClickedRefresh();
	afx_msg void OnStnClickedLastPath();
	afx_msg void OnBnClickedUpload();
	void SelectUploadFile(CString& outfile);
	afx_msg void OnBnClickedNewDirectory();
	afx_msg void OnBnClickedDeleteSource();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedDownload();
	afx_msg void OnNMClickWangpanFileList(NMHDR *pNMHDR, LRESULT *pResult);
	void	DeleteDownloadTask(DownloadFileInfo* info);
	void	DeleteUploadTask(UploadFileInfo* info);
	void	AddDownloadTask(DownloadFileInfo* info);
	void	AddUploadTask(UploadFileInfo* info);
	afx_msg void OnBnClickedShareSource();
};
