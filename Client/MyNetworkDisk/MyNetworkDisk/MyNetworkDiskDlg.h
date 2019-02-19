
// MyNetworkDiskDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <string>
using namespace std;
#include "MyMainDlg.h"
//ǰ��˵��������
class CMyMainDlg;
// CMyNetworkDiskDlg �Ի���
class CMyNetworkDiskDlg : public CDialogEx
{
// ����
public:
	CMyNetworkDiskDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MYNETWORKDISK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
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
