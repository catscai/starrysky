// MyTransferDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyTransferDlg.h"
#include "afxdialogex.h"


// CMyTransferDlg �Ի���

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


// CMyTransferDlg ��Ϣ�������


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

	// TODO: �ڴ˴������Ϣ����������
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
	if(nCtlColor ==  CTLCOLOR_STATIC) //��̬�ı�����͸��             
	{              
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//��������
void CMyTransferDlg::OnBnClickedDownloading()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_DOWNLOADING))->SetWindowText(_T("���ڡ�����"));
	UpdateData();
	this->HideAllListControl();
	this->m_downloaddlg.ShowWindow(SW_SHOW);
}

//�����ϴ�
void CMyTransferDlg::OnBnClickedUploading()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_UPLOADING))->SetWindowText(_T("���ڡ��ϴ�"));
	UpdateData();
	this->HideAllListControl();
	this->m_uploaddlg.ShowWindow(SW_SHOW);
}

//�������
void CMyTransferDlg::OnBnClickedTransferFinish()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->RefreshButtonText();
	((CButton*)GetDlgItem(IDC_TRANSFER_FINISH))->SetWindowText(_T("��������"));
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
	((CButton*)GetDlgItem(IDC_DOWNLOADING))->SetWindowText(_T("��������"));
	((CButton*)GetDlgItem(IDC_UPLOADING))->SetWindowText(_T("�����ϴ�"));
	((CButton*)GetDlgItem(IDC_TRANSFER_FINISH))->SetWindowText(_T("�������"));
	UpdateData();
}