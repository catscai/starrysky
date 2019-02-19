// MySetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MySetDlg.h"
#include "afxdialogex.h"
#include "MyLockDlg.h"
#include "MyMainDlg.h"
// CMySetDlg �Ի���

IMPLEMENT_DYNAMIC(CMySetDlg, CDialogEx)

CMySetDlg::CMySetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMySetDlg::IDD, pParent)
{

}

CMySetDlg::~CMySetDlg()
{
}

void CMySetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPLOAD_COMBOX, m_UploadComBox);
	DDX_Control(pDX, IDC_DOWNLOAD_COMBOX, m_DownloadComBox);
	DDX_Control(pDX, IDC_DOWNLOAD_SAVE_PATH, m_DownloadPathEdit);
	DDX_Control(pDX, IDC_TIMER_DURATION, m_TimerEdit);
	DDX_Control(pDX, IDC_TIMER_LOCK, m_TimerBtnCheck);
}


BEGIN_MESSAGE_MAP(CMySetDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CANCEL_BTN, &CMySetDlg::OnBnClickedCancelBtn)
	ON_BN_CLICKED(IDC_APPLY_BTN, &CMySetDlg::OnBnClickedApplyBtn)
	ON_BN_CLICKED(IDC_BROWSE_BTN, &CMySetDlg::OnBnClickedBrowseBtn)
	ON_BN_CLICKED(IDC_LOCK_DISK_BTN, &CMySetDlg::OnBnClickedLockDiskBtn)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMySetDlg ��Ϣ�������


BOOL CMySetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //���ｫλͼ���ؽ���Դ����Load
	m_brush.CreatePatternBrush(&m_bmp);
	this->RecoverDefault();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMySetDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_bmp.DeleteObject();   //λͼ
	m_brush.DeleteObject();  //��ˢ
}


HBRUSH CMySetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	else if(nCtlColor ==  CTLCOLOR_STATIC) //��̬�ı�����͸��             
	{   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(250,250,0));  
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);  
	}
	return hbr;
}


void CMySetDlg::OnBnClickedCancelBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->RecoverDefault();
}


void	CMySetDlg::RecoverDefault()
{
	m_DownloadComBox.SetCurSel(theApp.m_downloadNum - 1);
	m_UploadComBox.SetCurSel(theApp.m_uploadNum - 1);
	CString path;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_downloadPath.c_str(),path);
	m_DownloadPathEdit.SetWindowText(path);
}

void CMySetDlg::OnBnClickedApplyBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	CString dn,un,path;
	m_DownloadComBox.GetWindowText(dn);
	m_UploadComBox.GetWindowText(un);
	m_DownloadPathEdit.GetWindowText(path);
	theApp.m_downloadNum = _wtoi(dn);
	theApp.m_uploadNum = _wtoi(un);
	theApp.m_downloadPath = "";
	CMyTools::EnCodeWCHARToUtf(path,theApp.m_downloadPath);
	if(theApp.m_downloadPath[theApp.m_downloadPath.size() - 1] != '/')
		theApp.m_downloadPath += '/';
	CMyTools::createdir(theApp.m_downloadPath + theApp.m_email + '/');
	MessageBox(_T("�ѱ���"));
}


void CMySetDlg::OnBnClickedBrowseBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString path = CMyTools::SerachFolder();
	m_DownloadPathEdit.SetWindowText(path);
	UpdateData();
}

//��������
void CMySetDlg::OnBnClickedLockDiskBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int check = m_TimerBtnCheck.GetCheck();
	if(check){
		UpdateData(FALSE);
		CString minute;
		m_TimerEdit.GetWindowText(minute);
		long long second = _wtoi64(minute) * 60;
		SetTimer(100,second * 1000,NULL);
		MessageBox(_T("�����ö�ʱ����"));
	}
	else{
		this->ReplaceMainDlg();
	}
}

void	CMySetDlg::ReplaceMainDlg()
{
	CMyMainDlg* p = (CMyMainDlg*)theApp.m_pMainWnd;
	CMyLockDlg lock;
	theApp.m_pMainWnd = &lock;
	p->EndDialog(IDOK);
	p->DestroyWindow();
	lock.DoModal();
}

void CMySetDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent == 100)
	{
		this->ReplaceMainDlg();
		KillTimer(100);
	}
	CDialogEx::OnTimer(nIDEvent);
}
