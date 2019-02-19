#pragma once
#include "afxwin.h"


// CMyExtractFileDlg �Ի���

class CMyExtractFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyExtractFileDlg)

public:
	CMyExtractFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMyExtractFileDlg();

// �Ի�������
	enum { IDD = IDD_EXTRACT_FILE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CBitmap			m_bmp;   //λͼ
    CBrush			m_brush;  //��ˢ
	CEdit			m_ExtractEmailEdit;
	CEdit			m_ExtractGetcodeEdit;
	CEdit			m_ExtractFileNameEdit;
	CStatic			m_ExtractFileIcon;
	HBITMAP			m_bmpfile,m_bmpdir;

	CString			m_name,m_getcode;
	CString			m_fname,m_fdir;
	CEdit			m_SaveDirEdit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton7();
	afx_msg LRESULT OnExtractFileResponse(WPARAM w, LPARAM l);
	afx_msg LRESULT OnSaveFileResponse(WPARAM w, LPARAM l);
	afx_msg void OnBnClickedButton5();
	
	afx_msg void OnBnClickedSaveFile();
};
