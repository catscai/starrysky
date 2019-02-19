// MyShareFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyShareFileDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
#include "MyShareDlg.h"
#include "MyMainDlg.h"
// CMyShareFileDlg �Ի���

IMPLEMENT_DYNAMIC(CMyShareFileDlg, CDialogEx)

CMyShareFileDlg::CMyShareFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyShareFileDlg::IDD, pParent)
	, m_RadioGroup(0)
{

}

CMyShareFileDlg::~CMyShareFileDlg()
{
}

void CMyShareFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioGroup);
	DDX_Control(pDX, IDC_EDIT1, m_ShareFileNameEdit);
	DDX_Control(pDX, IDC_EDIT2, m_ShareGetCodeEdit);
}


BEGIN_MESSAGE_MAP(CMyShareFileDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO1, &CMyShareFileDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMyShareFileDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CMyShareFileDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CMyShareFileDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyShareFileDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyShareFileDlg::OnBnClickedButton4)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(SHARE_FILE_RESPONSE_DLG_MSG,&CMyShareFileDlg::OnShareFileResponse)
END_MESSAGE_MAP()


// CMyShareFileDlg ��Ϣ�������


BOOL CMyShareFileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_days = 1;
	m_RadioGroup = 1;

	CStatic* success = (CStatic*)GetDlgItem(IDC_SHARE_SUCCESS_TEXT);
	CStatic* failed = (CStatic*)GetDlgItem(IDC_SHARE_FAILED_TEXT);
	CFont* ptmp = success->GetFont();
	LOGFONT lf;
	ptmp->GetLogFont(&lf);//��ȡLOGFONT�ṹ��
	lf.lfHeight=20;    //�޸������С
	lf.lfItalic=TRUE;        //��б
	lf.lfWeight=FW_BOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf.lfFaceName,_T("����"));
	m_font1.CreateFontIndirectW(&lf);

	success->SetFont(&m_font1);
	failed->SetFont(&m_font1);

	m_parent = &(((CMyMainDlg*)theApp.m_pMainWnd)->m_MyShareDlg);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMyShareFileDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	m_font1.DeleteObject();
}

void	CMyShareFileDlg::SetInitParams(const CString& name)
{
	m_ShareFileNameEdit.SetWindowText(name);
	UpdateData();
}

void CMyShareFileDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_days = 1;
}


void CMyShareFileDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_days = 3;
}


void CMyShareFileDlg::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_days = 30;
}


void CMyShareFileDlg::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_days = 999999;
}

//ȷ�Ϸ���
void CMyShareFileDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShareFileRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = SHARE_FILE_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	string fname,fdir;
	CMyTools::EnCodeWCHARToUtf(m_ShareFileDir,fdir);
	CMyTools::EnCodeWCHARToUtf(m_ShareFileName,fname);
	memcpy_s(rq.m_dir,DIR_SIZE,fdir.c_str(),fdir.size());
	memcpy_s(rq.m_name,FILENAME_LENGTH,fname.c_str(),fname.size());
	rq.m_days = m_days;
	
	theApp.net()->send((char*)&rq,rq.m_len);
}


void CMyShareFileDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_SHARE_SUCCESS_TEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SHARE_FAILED_TEXT)->ShowWindow(SW_HIDE);
	this->SendMessage(WM_CLOSE);
}


void	CMyShareFileDlg::SetDirName(const CString& fdir,const CString& fname)
{
	m_ShareFileDir = fdir;
	m_ShareFileName = fname;
}

//�����ļ��ظ�
LRESULT CMyShareFileDlg::OnShareFileResponse(WPARAM w, LPARAM l)
{
	ShareFileRs* rs = (ShareFileRs*)w;
	if(rs->m_statuscode == SHARE_SUCCESS)
	{//����ɹ�
		CMyTools::EnCodeUtfToWCHAR(rs->m_getcode,m_ShareGetCode);
		m_ShareGetCodeEdit.SetWindowText(m_ShareGetCode);
		GetDlgItem(IDC_SHARE_SUCCESS_TEXT)->ShowWindow(SW_SHOW);
		CString out,days;
		days.Format(_T("%d ��"),m_days);
		CMyTools::NowTime(out);
		m_parent->AddShareFile(m_ShareFileDir + m_ShareFileName,out,days,_T("0"),m_ShareGetCode);
		UpdateData();
	}else if(rs->m_statuscode == SHARE_EXIST)
	{
		GetDlgItem(IDC_SHARE_FAILED_TEXT)->SetWindowText(_T("�ļ��Ѿ�����"));
		GetDlgItem(IDC_SHARE_FAILED_TEXT)->ShowWindow(SW_SHOW);
		UpdateData();
	}else{
		GetDlgItem(IDC_SHARE_FAILED_TEXT)->SetWindowText(_T("����ʧ��"));
		GetDlgItem(IDC_SHARE_FAILED_TEXT)->ShowWindow(SW_SHOW);
		UpdateData();
	}

	return 0;
}

HBRUSH CMyShareFileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	HWND h = pWnd->GetSafeHwnd();
	if(GetDlgItem(IDC_SHARE_SUCCESS_TEXT)->GetSafeHwnd() == h){
		pDC->SetTextColor(RGB(36,255,91));
	}else if(GetDlgItem(IDC_SHARE_FAILED_TEXT)->GetSafeHwnd() == h)
	{
		pDC->SetTextColor(RGB(255,53,53));
	}
	return hbr;
}
