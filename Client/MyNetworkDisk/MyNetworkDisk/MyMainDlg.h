#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyWangPanDlg.h"
#include "MyTransferDlg.h"
#include "MyFriendsDlg.h"
#include "MyShareDlg.h"
#include "MyExtractFileDlg.h"
#include "MySetDlg.h"
#include <string>
using namespace std;
// CMyMainDlg 对话框

class CMyMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyMainDlg)

public:
	CMyMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyMainDlg();

// 对话框数据
	enum { IDD = IDD_MAIN_DLG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //位图
    CBrush			m_brush;  //画刷
	CFont			fon1,fon2,fon3,fon4,fon5,fon6;
	CStatic			m_XingKong;
	CStatic			m_RecentAccess;
	CStatic			m_MyShare;
	CStatic			m_Recycle;
	CStatic			m_SetUp;
	CStatic			m_NickName;
	CStatic			m_VipClass;
	CStatic			m_UpdateVip;
	CStatic			m_MyWangPan;
	CStatic			m_FriendShare;
	CStatic			m_TransferList;
	CStatic			m_GetSource;
	CStatic			m_CloseWindow;
	CStatic			m_MinWindow;
private:
	CStatic			m_PictureChild;		//picture控件让子窗口显示在这个控件的位置
	CRect			m_PictureRect;
public://窗口
	CProgressCtrl	m_CapacityProgress;
	CMyWangPanDlg	m_MyWangPanDlg;
	CMyTransferDlg	m_MyTransferDlg;
	CMyFriendsDlg	m_MyFriendsDlg;
	CMyShareDlg		m_MyShareDlg;
	CMyExtractFileDlg m_MyExtractDlg;
	CMySetDlg		m_MySetDlg;
	CStatic			m_Capacity;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void	SetControlFont();
	void    HideAllStatic();
	void	FirstRequestFile();
	void	HideAllDialogs();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedMainMywangpan();
	afx_msg void OnStnClickedMainFriendShare();
	afx_msg void OnStnClickedMainTranferlist();
	afx_msg void OnStnClickedMainGetsource();
	afx_msg void OnStnClickedMainRecent();
	afx_msg void OnStnClickedMainShare();
	afx_msg void OnStnClickedMainRecycle();
	afx_msg void OnStnClickedMainSet();
	
	afx_msg void OnStnClickedMainClose();
	afx_msg void OnStnClickedMainMin();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
