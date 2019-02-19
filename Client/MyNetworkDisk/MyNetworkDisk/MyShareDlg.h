#pragma once
#include "afxcmn.h"
#include "MyShareFileDlg.h"
#include <map>
using namespace std;
// CMyShareDlg �Ի���
struct ShareFileInfo;
class CMyShareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyShareDlg)

public:
	CMyShareDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyShareDlg();

// �Ի�������
	enum { IDD = IDD_MYSHARE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
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
