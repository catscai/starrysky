// MyTransferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyTransferDlg.h"
#include "afxdialogex.h"


// CMyTransferDlg 对话框

IMPLEMENT_DYNAMIC(CMyTransferDlg, CDialogEx)

CMyTransferDlg::CMyTransferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTransferDlg::IDD, pParent)
{

}

CMyTransferDlg::~CMyTransferDlg()
{
}

void CMyTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSFER_PIC, m_pictureWindow);
}


BEGIN_MESSAGE_MAP(CMyTransferDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DOWNLOADING, &CMyTransferDlg::OnBnClickedDownloading)
	ON_BN_CLICKED(IDC_UPLOADING, &CMyTransferDlg::OnBnClickedUploading)
	ON_BN_CLICKED(IDC_TRANSFER_FINISH, &CMyTransferDlg::OnBnClickedTransferFinish)
END_MESSAGE_MAP()


// CMyTransferDlg 消息处理程序


BOOL CMyTransferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_pictureWindow.GetWindowRect(&m_pictureRect);
	ScreenToClient(m_pictureRect);
	m_pictureWindow.ShowWindow(SW_HIDE);

	m_downloaddlg.Create(IDD_TRANSFER_DOWNLOAD_DLG,this);
	m_downloaddlg.MoveWindow(m_pictureRect);
	m_downloaddlg.ShowWindow(SW_SHOW);

	m_uploaddlg.Create(IDD_TRANSFER_UPLOAD_DLG,this);
	m_uploaddlg.MoveWindow(m_pictureRect);
	m_uploaddlg.ShowWindow(SW_HIDE);

	m_finishdlg.Create(IDD_TRANSFER_FINISH,this);
	m_finishdlg.MoveWindow(m_pictureRect);
	m_finishdlg.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control

}


void CMyTransferDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_downloaddlg.DestroyWindow();
	m_uploaddlg.DestroyWindow();
	m_finishdlg.DestroyWindow();
}


HBRUSH CMyTransferDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG){
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	//HWND h = pWnd->GetSafeHwnd();
	if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{              
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//正在下载
void CMyTransferDlg::OnBnClickedDownloading()
{
	// TODO: 在此添加控件通知处理程序代码
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_DOWNLOADING))->SetWindowText(_T("正在☆下载"));
	UpdateData();
	this->HideAllListControl();
	this->m_downloaddlg.ShowWindow(SW_SHOW);
}

//正在上传
void CMyTransferDlg::OnBnClickedUploading()
{
	// TODO: 在此添加控件通知处理程序代码
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_UPLOADING))->SetWindowText(_T("正在☆上传"));
	UpdateData();
	this->HideAllListControl();
	this->m_uploaddlg.ShowWindow(SW_SHOW);
}

//传输完成
void CMyTransferDlg::OnBnClickedTransferFinish()
{
	// TODO: 在此添加控件通知处理程序代码
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_TRANSFER_FINISH))->SetWindowText(_T("传输☆完成"));
	UpdateData();
	this->HideAllListControl();
	this->m_finishdlg.ShowWindow(SW_SHOW);
}

void	CMyTransferDlg::HideAllListControl()
{
	this->m_downloaddlg.ShowWindow(SW_HIDE);
	this->m_uploaddlg.ShowWindow(SW_HIDE);
	this->m_finishdlg.ShowWindow(SW_HIDE);
}

void    CMyTransferDlg::RefreshButtonText()
{
	((CButton*)GetDlgItem(IDC_DOWNLOADING))->SetWindowText(_T("正在下载"));
	((CButton*)GetDlgItem(IDC_UPLOADING))->SetWindowText(_T("正在上传"));
	((CButton*)GetDlgItem(IDC_TRANSFER_FINISH))->SetWindowText(_T("传输完成"));
	UpdateData();
}