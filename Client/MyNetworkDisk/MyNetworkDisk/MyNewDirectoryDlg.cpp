// MyNewDirectoryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyNewDirectoryDlg.h"
#include "afxdialogex.h"
#include "MyTools.h"
#include "packdef.h"
#include "TcpMediator.h"

// CMyNewDirectoryDlg �Ի���

IMPLEMENT_DYNAMIC(CMyNewDirectoryDlg, CDialogEx)

CMyNewDirectoryDlg::CMyNewDirectoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyNewDirectoryDlg::IDD, pParent)
{

}

CMyNewDirectoryDlg::~CMyNewDirectoryDlg()
{
}

void CMyNewDirectoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NEW_DIRECTORY_EDIT, m_DirEdit);
	DDX_Control(pDX, IDC_NEW_DIRECTORY_NAME_EDIT, m_NewDirEdit);
}


BEGIN_MESSAGE_MAP(CMyNewDirectoryDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DIRECTORY_OK, &CMyNewDirectoryDlg::OnBnClickedDirectoryOk)
	ON_BN_CLICKED(IDC_DIRECTORY_CANCEL, &CMyNewDirectoryDlg::OnBnClickedDirectoryCancel)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyNewDirectoryDlg ��Ϣ�������


BOOL CMyNewDirectoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString dir;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_currdir.c_str(),dir);
	m_DirEdit.SetWindowText(dir);
	UpdateData();
	::SetWindowLong(m_hWnd , GWL_EXSTYLE , GetWindowLong(m_hWnd , GWL_EXSTYLE)| WS_EX_LAYERED);
	::SetLayeredWindowAttributes(m_hWnd , 0 , static_cast<BYTE>(200) , LWA_ALPHA);
	return TRUE;  // return TRUE unless you set the focus to a control

}


void CMyNewDirectoryDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}


HBRUSH CMyNewDirectoryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		pDC->SetBkMode(TRANSPARENT); 
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);  
	}
	return hbr;
}

//ȷ�ϴ���
void CMyNewDirectoryDlg::OnBnClickedDirectoryOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	CString dir,newdir;
	m_DirEdit.GetWindowText(dir);
	m_NewDirEdit.GetWindowText(newdir);
	if(dir.IsEmpty() || newdir.IsEmpty()){
		AfxMessageBox(_T("����Ϊ��"));
		return;
	}
	string dirs,newdirs;
	CMyTools::EnCodeWCHARToUtf(dir.GetBuffer(),dirs);
	CMyTools::EnCodeWCHARToUtf(newdir.GetBuffer(),newdirs);
	AddNewDirectoryRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = ADD_NEW_DIRECTORY_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_dir,DIR_SIZE,dirs.c_str(),dirs.size());
	memcpy_s(rq.m_newdir,FILENAME_LENGTH,newdirs.c_str(),newdirs.size());
	
	theApp.net()->send((char*)&rq,rq.m_len);
}

//ȡ������
void CMyNewDirectoryDlg::OnBnClickedDirectoryCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->SendMessage(WM_CLOSE);
}


void CMyNewDirectoryDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����������µ�λ��ת�Ƶ���������
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}


BOOL CMyNewDirectoryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
