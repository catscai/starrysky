// MyExtractFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyExtractFileDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
// CMyExtractFileDlg �Ի���

IMPLEMENT_DYNAMIC(CMyExtractFileDlg, CDialogEx)

CMyExtractFileDlg::CMyExtractFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyExtractFileDlg::IDD, pParent)
{

}

CMyExtractFileDlg::~CMyExtractFileDlg()
{
}

void CMyExtractFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXTRACT_EMAIL, m_ExtractEmailEdit);
	DDX_Control(pDX, IDC_EXTRACT_CODE, m_ExtractGetcodeEdit);
	DDX_Control(pDX, IDC_EXTRACT_FILE_NAME_EDIT, m_ExtractFileNameEdit);
	DDX_Control(pDX, IDC_EXTRACT_FILE_ICON, m_ExtractFileIcon);
	DDX_Control(pDX, IDC_SAVE_DIR, m_SaveDirEdit);
}


BEGIN_MESSAGE_MAP(CMyExtractFileDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &CMyExtractFileDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &CMyExtractFileDlg::OnBnClickedButton7)
	ON_MESSAGE(EXTRACT_FILE_RESPONSE_DLG_MSG,&CMyExtractFileDlg::OnExtractFileResponse)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyExtractFileDlg::OnBnClickedButton5)
	ON_MESSAGE(SAVE_FILE_RESPONSE_DLG_MSG,&CMyExtractFileDlg::OnSaveFileResponse)
	ON_BN_CLICKED(IDC_SAVE_FILE, &CMyExtractFileDlg::OnBnClickedSaveFile)
END_MESSAGE_MAP()


// CMyExtractFileDlg ��Ϣ�������


BOOL CMyExtractFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //���ｫλͼ���ؽ���Դ����Load
	m_brush.CreatePatternBrush(&m_bmp);
	m_bmpfile = ::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_FILE_BMP));
	m_bmpdir = ::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_DIRECTORY_BMP));
	GetDlgItem(IDC_SAVE_FILE)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMyExtractFileDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_brush.DeleteObject();
	m_bmp.DeleteObject();
	::DeleteObject(m_bmpfile);
	::DeleteObject(m_bmpdir);
}


HBRUSH CMyExtractFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}else if(nCtlColor == CTLCOLOR_STATIC){
		pDC->SetBkMode(TRANSPARENT);
		HWND h = pWnd->GetSafeHwnd();
		if(GetDlgItem(IDC_TIQVWENJIAN)->GetSafeHwnd() == h
			|| GetDlgItem(IDC_WENJIANXINXI)->GetSafeHwnd() == h)
		{
			pDC->SetTextColor(RGB(255,255,68));
		}else
		{
			pDC->SetTextColor(RGB(255,255,255));
		}
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	}
	return hbr;
}

//��ȡ��Դ
void CMyExtractFileDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	m_ExtractEmailEdit.GetWindowText(m_name);
	m_ExtractGetcodeEdit.GetWindowText(m_getcode);
	if(m_name.IsEmpty() || m_getcode.IsEmpty()){
		AfxMessageBox(_T("�û��˺�����ȡ�������Ϊ�գ�"));
		return;
	}
	string e,g;
	CMyTools::EnCodeWCHARToUtf(m_name,e);
	CMyTools::EnCodeWCHARToUtf(m_getcode,g);
	if(CMyTools::MatchUid(e) == false){
		AfxMessageBox(_T("�����˺Ÿ�ʽ����"));
		return;
	}
	ExtractFileRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = EXTRACT_SHARE_FILE_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_otheremail,EMAIL_SIZE,e.c_str(),e.size());
	memcpy_s(rq.m_getcode,SHARE_FILE_GETCODE_LENGTH,g.c_str(),g.size());
	
	theApp.net()->send((char*)&rq,rq.m_len);
}


void CMyExtractFileDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->m_ExtractEmailEdit.SetWindowText(_T(""));
	this->m_ExtractGetcodeEdit.SetWindowText(_T(""));
	this->m_ExtractFileNameEdit.SetWindowText(_T(""));
	GetDlgItem(IDC_SAVE_FILE)->EnableWindow(FALSE);
}

LRESULT CMyExtractFileDlg::OnExtractFileResponse(WPARAM w, LPARAM l)
{
	ExtractFileRs* rs = (ExtractFileRs*)w;
	if(rs->m_statuscode == EXTRACT_OK)
	{//��ȡ�ļ��ɹ�
		CMyTools::EnCodeUtfToWCHAR(rs->m_name,m_fname);
		CMyTools::EnCodeUtfToWCHAR(rs->m_dir,m_fdir);
		if(rs->m_filetype == FILE_TYPE){
			m_ExtractFileIcon.SetBitmap(m_bmpfile);
		}else if(rs->m_filetype == DIRECTORY_TYPE){
			m_ExtractFileIcon.SetBitmap(m_bmpdir);
		}
		m_ExtractFileIcon.ShowWindow(SW_SHOW);
		m_ExtractFileNameEdit.SetWindowText(m_fname);
		GetDlgItem(IDC_SAVE_FILE)->EnableWindow(TRUE);
		UpdateData();
	}else if(rs->m_statuscode == EXTRACT_ERROR)
	{//��ȡ�ļ�������
		AfxMessageBox(_T("��ȡ�ļ�������"));
	}

	return true;
}

//�����ļ�
void CMyExtractFileDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}//dCbLXC

LRESULT CMyExtractFileDlg::OnSaveFileResponse(WPARAM w, LPARAM l)
{
	SaveShareFileRs* rs = (SaveShareFileRs*)w;
	if(rs->m_statuscode == SAVE_SUCCESS)
	{//�����ļ��ɹ�
		AfxMessageBox(_T("�����ļ��ɹ���"));
	}else if(rs->m_statuscode == SAVE_DIR_UNEXIST)
	{
		AfxMessageBox(_T("�����ļ��в����ڣ�"));
	}else if(rs->m_statuscode == SAVE_CAPACITY_LESS)
	{
		AfxMessageBox(_T("�����ļ�ʧ�ܣ��ռ䲻�㣡"));
	}
	return 0;
}

void CMyExtractFileDlg::OnBnClickedSaveFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(FALSE);
	CString mydir;
	m_SaveDirEdit.GetWindowText(mydir);
	if(mydir.IsEmpty()){
		AfxMessageBox(_T("�����ļ��в���Ϊ�գ�"));
		return;
	}
	string fname,fdir,getcode,other,fmydir;
	CMyTools::EnCodeWCHARToUtf(mydir,fmydir);
	if(CMyTools::MatchDirectoryPath(fmydir) == false){
		AfxMessageBox(_T("��������ȷ���ļ��и�ʽ�����磺/home/"));
		return;
	}
	CMyTools::EnCodeWCHARToUtf(m_fname,fname);
	CMyTools::EnCodeWCHARToUtf(m_fdir,fdir);
	CMyTools::EnCodeWCHARToUtf(m_getcode,getcode);
	CMyTools::EnCodeWCHARToUtf(m_name,other);
	if(fmydir.size() > 2)
		fmydir = fmydir.substr(0,fmydir.size() - 1);
	SaveShareFileRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = SAVE_SHARE_FILE_REQUEST_MSG;
	memcpy_s(rq.m_dir,DIR_SIZE,fdir.c_str(),fdir.size());
	memcpy_s(rq.m_name,FILENAME_LENGTH,fname.c_str(),fname.size());
	memcpy_s(rq.m_getcode,SHARE_FILE_GETCODE_LENGTH,getcode.c_str(),getcode.size());
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_otheremail,EMAIL_SIZE,other.c_str(),other.size());
	memcpy_s(rq.m_mydir,DIR_SIZE,fmydir.c_str(),fmydir.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}
