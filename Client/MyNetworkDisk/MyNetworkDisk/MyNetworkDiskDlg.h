
// MyNetworkDiskDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <string>
using namespace std;
#include "MyMainDlg.h"
//前置说明主窗口
class CMyMainDlg;
// CMyNetworkDiskDlg 对话框
class CMyNetworkDiskDlg : public CDialogEx
{
// 构造
public:
	CMyNetworkDiskDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYNETWORKDISK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //位图
    CBrush			m_brush;  //画刷
	CBrush			m_brushEdit;
	CFont			fon,fon2;
	CEdit			m_UidEdit;
	CEdit			m_PasswdEdit;
	CButton			m_RememberPassBtn;
	CButton			m_AutoLogBtn;
	bool			m_IsAutoLoginFlag;
	string			m_uidStr,m_passStr;
	CString			m_uidCStr,m_passwdCStr;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCancelBtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedLoginBtn();
	afx_msg void OnBnClickedRegisterBtn();
	afx_msg LRESULT OnLoginSucessDlg(WPARAM w, LPARAM l);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
