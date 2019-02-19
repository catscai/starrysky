// MySetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MySetDlg.h"
#include "afxdialogex.h"
#include "MyLockDlg.h"
#include "MyMainDlg.h"
// CMySetDlg 对话框

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


// CMySetDlg 消息处理程序


BOOL CMySetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //这里将位图加载进资源后，再Load
	m_brush.CreatePatternBrush(&m_bmp);
	this->RecoverDefault();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMySetDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_bmp.DeleteObject();   //位图
	m_brush.DeleteObject();  //画刷
}


HBRUSH CMySetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	else if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{   
		pDC->SetBkMode(TRANSPARENT);   
		pDC->SetTextColor(RGB(250,250,0));  
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);  
	}
	return hbr;
}


void CMySetDlg::OnBnClickedCancelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
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
	MessageBox(_T("已保存"));
}


void CMySetDlg::OnBnClickedBrowseBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path = CMyTools::SerachFolder();
	m_DownloadPathEdit.SetWindowText(path);
	UpdateData();
}

//锁定网盘
void CMySetDlg::OnBnClickedLockDiskBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	int check = m_TimerBtnCheck.GetCheck();
	if(check){
		UpdateData(FALSE);
		CString minute;
		m_TimerEdit.GetWindowText(minute);
		long long second = _wtoi64(minute) * 60;
		SetTimer(100,second * 1000,NULL);
		MessageBox(_T("已设置定时锁定"));
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 100)
	{
		this->ReplaceMainDlg();
		KillTimer(100);
	}
	CDialogEx::OnTimer(nIDEvent);
}
