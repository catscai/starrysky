#pragma once
#include "afxwin.h"
#include "MyLockDlg.h"

// CMySetDlg �Ի���

class CMySetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMySetDlg)

public:
	CMySetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMySetDlg();

// �Ի�������
	enum { IDD = IDD_MYSET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
	CComboBox		m_UploadComBox;
	CComboBox		m_DownloadComBox;
	CEdit			m_DownloadPathEdit;
	CEdit			m_TimerEdit;
	CButton			m_TimerBtnCheck;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCancelBtn();
	void	RecoverDefault();
	void	ReplaceMainDlg();
	afx_msg void OnBnClickedApplyBtn();
	afx_msg void OnBnClickedBrowseBtn();
	afx_msg void OnBnClickedLockDiskBtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
