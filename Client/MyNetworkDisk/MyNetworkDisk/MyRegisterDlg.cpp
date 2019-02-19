// MyRegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyRegisterDlg.h"
#include "afxdialogex.h"
#include "MyTools.h"
#include <regex>
#include "packdef.h"
#include "TcpMediator.h"
using namespace std;

// CMyRegisterDlg �Ի���

IMPLEMENT_DYNAMIC(CMyRegisterDlg, CDialogEx)

CMyRegisterDlg::CMyRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyRegisterDlg::IDD, pParent)
	, m_securityCodeStr(_T(""))
{

}

CMyRegisterDlg::~CMyRegisterDlg()
{
}

void CMyRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_nickNameEdit);
	DDX_Control(pDX, IDC_EDIT4, m_passwdEdit1);
	DDX_Control(pDX, IDC_EDIT5, m_passwdEdit2);
	DDX_Control(pDX, IDC_EDIT6, m_emailEdit);
	DDX_Text(pDX, IDC_EDIT7, m_securityCodeStr);
	DDX_Control(pDX, IDC_CAPTCHA_BTN, m_securityBtn);
}


BEGIN_MESSAGE_MAP(CMyRegisterDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REGISTER_CANCEL, &CMyRegisterDlg::OnBnClickedRegisterCancel)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CAPTCHA_BTN, &CMyRegisterDlg::OnBnClickedCaptchaBtn)
	ON_BN_CLICKED(IDC_REGISTER_OK, &CMyRegisterDlg::OnBnClickedRegisterOk)
END_MESSAGE_MAP()


// CMyRegisterDlg ��Ϣ�������


BOOL CMyRegisterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_REGISTER_BMP));    //���ｫλͼ���ؽ���Դ����Load
    m_brush.CreatePatternBrush(&m_bmp); 
	m_fon1 = new CFont;
	m_fon2 = new CFont;
	m_fon1->CreatePointFont(150,_T("����"),NULL);
	GetDlgItem(IDC_NAME)->SetFont(m_fon1);
	GetDlgItem(IDC_PASSWORD1)->SetFont(m_fon1);
	GetDlgItem(IDC_PASSWORD2)->SetFont(m_fon1);
	GetDlgItem(IDC_INPUT_EMAIL)->SetFont(m_fon1);
	GetDlgItem(IDC_CAPTCHA)->SetFont(m_fon1);
	//m_nickNameEdit.SetWindowText(_T("����Ϊ2-20���ַ�"));
	//m_passwdEdit1.SetWindowText(_T("6-16λ���ֻ���ĸ"));
	CFont* ftmp1 = GetDlgItem(IDC_ZHU)->GetFont();
	LOGFONT lf;
	ftmp1->GetLogFont(&lf);//��ȡLOGFONT�ṹ��
	lf.lfHeight=100;    //�޸������С
	lf.lfItalic=FALSE;        //��б
	lf.lfWeight=FW_BOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf.lfFaceName,_T("����"));
	m_fon2->CreateFontIndirectW(&lf);
	GetDlgItem(IDC_ZHU)->SetFont(m_fon2);
	GetDlgItem(IDC_CE)->SetFont(m_fon2);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH CMyRegisterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	if(nCtlColor ==  CTLCOLOR_STATIC) //��̬�ı�����͸��             
	{              
		pDC->SetBkMode(TRANSPARENT);   
		if(GetDlgItem(IDC_ZHU)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_CE)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
			pDC->SetTextColor(RGB(34,175,175));
		}
		else if(GetDlgItem(IDC_NAME)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PASSWORD1)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PASSWORD2)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_INPUT_EMAIL)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_CAPTCHA)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
			pDC->SetTextColor(RGB(150,200,60));
		}
		else if(GetDlgItem(IDC_PROMPT1)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PROMPT2)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PROMPT3)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PROMPT4)->GetSafeHwnd() == pWnd->GetSafeHwnd()
			|| GetDlgItem(IDC_PROMPT5)->GetSafeHwnd() == pWnd->GetSafeHwnd())
		{
			pDC->SetTextColor(RGB(200,10,10));
		}
		else
			pDC->SetTextColor(RGB(255,255,255));
		
		//pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);               
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CMyRegisterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_fon1->DeleteObject();
	m_fon2->DeleteObject();
	delete m_fon1;
	delete m_fon2;
}


void CMyRegisterDlg::OnBnClickedRegisterCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->SendMessage(WM_CLOSE);
}


void CMyRegisterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����������µ�λ��ת�Ƶ���������
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

//��ȡ��֤��
void CMyRegisterDlg::OnBnClickedCaptchaBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

//ȷ��ע��
void CMyRegisterDlg::OnBnClickedRegisterOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ʾ����ȫ������
	this->HidePromptWindow();

	CString str;
	string  text;
	RegisterRq rq;
	ZeroMemory(&rq,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = REGISTER_REQUEST_MSG;
	m_nickNameEdit.GetWindowText(str);
	if(str.IsEmpty()){
		AfxMessageBox(_T("�������ǳ�.."));
		return;
	}
	CMyTools::EnCodeWCHARToUtf(str.GetBuffer(),text);
	if(text.size() < 2 && text.size() > 20){
		GetDlgItem(IDC_PROMPT1)->ShowWindow(SW_SHOW);
		return;
	}
	//ע���ǳ�
	memcpy_s(rq.m_nickname,NICKNAME_SIZE,text.c_str(),text.size());
	m_passwdEdit1.GetWindowText(str);
	CMyTools::EnCodeWCHARToUtf(str.GetBuffer(),text);
	if(false == CMyTools::MatchPassword(text)){
		GetDlgItem(IDC_PROMPT2)->ShowWindow(SW_SHOW);
	}
	CString tmp;
	m_passwdEdit2.GetWindowText(tmp);
	if(tmp != str){
		GetDlgItem(IDC_PROMPT3)->ShowWindow(SW_SHOW);
	}
	//ע������
	memcpy_s(rq.m_pass,PASSWD_SIZE,text.c_str(),text.size());
	m_emailEdit.GetWindowText(str);
	CMyTools::EnCodeWCHARToUtf(str.GetBuffer(),text);
	if(false == CMyTools::MatchUid(text)){
		GetDlgItem(IDC_PROMPT4)->ShowWindow(SW_SHOW);
	}
	//ע������
	memcpy_s(rq.m_email,EMAIL_SIZE,text.c_str(),text.size());
	//��֤�����:��ʱ���Թ�

	ZeroMemory(rq.m_securitycode,SECURITY_CODE_SIZE);
	//ע������:��Ϊ�ڷ������������

	theApp.net()->send((char*)&rq,sizeof(rq));
}


BOOL CMyRegisterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void	CMyRegisterDlg::HidePromptWindow()
{
	GetDlgItem(IDC_PROMPT1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROMPT2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROMPT3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROMPT4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROMPT5)->ShowWindow(SW_HIDE);
}
