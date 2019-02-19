// MyNetworkDiskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyNetworkDiskDlg.h"
#include "afxdialogex.h"
#include "IOFileStream.h"
#include "MyTools.h"
#include "MyRegisterDlg.h"
#include "MyMainDlg.h"
#include "packdef.h"
#include "TcpMediator.h"
#include <regex>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyNetworkDiskDlg �Ի���



CMyNetworkDiskDlg::CMyNetworkDiskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyNetworkDiskDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyNetworkDiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_UidEdit);
	DDX_Control(pDX, IDC_EDIT3, m_PasswdEdit);
	DDX_Control(pDX, IDC_CHECK1, m_RememberPassBtn);
	DDX_Control(pDX, IDC_CHECK2, m_AutoLogBtn);
}

BEGIN_MESSAGE_MAP(CMyNetworkDiskDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CANCEL_BTN, &CMyNetworkDiskDlg::OnBnClickedCancelBtn)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_LOGIN_BTN, &CMyNetworkDiskDlg::OnBnClickedLoginBtn)
	ON_BN_CLICKED(IDC_REGISTER_BTN, &CMyNetworkDiskDlg::OnBnClickedRegisterBtn)
	ON_MESSAGE(LOGIN_SUCESS_DLG_MSG,&CMyNetworkDiskDlg::OnLoginSucessDlg)
END_MESSAGE_MAP()


// CMyNetworkDiskDlg ��Ϣ�������

BOOL CMyNetworkDiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_IsAutoLoginFlag = false;	//�Ƿ����Զ���¼
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_LOGIN_BMP));    //���ｫλͼ���ؽ���Դ����Load
    m_brush.CreatePatternBrush(&m_bmp); 
	CStatic* pass = (CStatic*)GetDlgItem(IDC_PASSWD);
	CStatic* uid = (CStatic*)GetDlgItem(IDC_UID);
	CStatic* wang = (CStatic*)GetDlgItem(IDC_WANG);
	CStatic* pan = (CStatic*)GetDlgItem(IDC_PAN);
	fon.CreatePointFont(150,_T("����"),NULL);
	pass->SetFont(&fon);
	uid->SetFont(&fon);
	m_brushEdit.CreateSolidBrush(RGB(255,255,255));
	CFont* ftmp1 = wang->GetFont();
	LOGFONT lf;
	ftmp1->GetLogFont(&lf);//��ȡLOGFONT�ṹ��
	lf.lfHeight=120;    //�޸������С
	lf.lfItalic=FALSE;        //��б
	lf.lfWeight=FW_BOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf.lfFaceName,_T("����"));
	//fon2->CreatePointFont(180,_T("����"));
	fon2.CreateFontIndirectW(&lf);
	wang->SetFont(&fon2);
	pan->SetFont(&fon2);
	wang->SetWindowText(_T("��"));
	pan->SetWindowText(_T("��"));
	//"caiyanqing1266@126.com","woaini123"
	CIOFileStream input("D:/StarrySky/login",IO_READ);
	if(input.IsExist() == true)
	{
		CString str;
		string u,p,c2;
		input.readline(u);
		if(!u.empty()){
			CMyTools::EnCodeUtfToWCHAR(u.c_str(),str);
			m_UidEdit.SetWindowText(str);
			input.readline(p);
			if(p != "FALSE")
			{
				for(unsigned i = 0; i < p.size(); i++)
					p[i] = ~p[i];
				CMyTools::EnCodeUtfToWCHAR(p.c_str(),str);
				m_PasswdEdit.SetWindowText(str);
				m_RememberPassBtn.SetCheck(1);
				input.readline(c2);
				if(c2 == "TRUE"){
					m_AutoLogBtn.SetCheck(1);
					m_IsAutoLoginFlag = true;
				}
				else
					m_AutoLogBtn.SetCheck(0);
			}
			UpdateData();
		}
		input.close();
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMyNetworkDiskDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyNetworkDiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyNetworkDiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



HBRUSH CMyNetworkDiskDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}

	/*
	if(nCtlColor == CTLCOLOR_DLG)   //�˴�����Ϊ����͸����CTLCOLOR_DLG��ʾ�Ի���  
	{  
		CBrush *brush;  
		brush = new CBrush(RGB(255,255,255));  
		return (HBRUSH)(brush->m_hObject);  
	}  */
	if(nCtlColor ==  CTLCOLOR_STATIC) //��̬�ı�����͸��             
	{              
		pDC->SetBkMode(TRANSPARENT);   
		if(GetDlgItem(IDC_WANG)->GetSafeHwnd() == pWnd->GetSafeHwnd() ||
			pWnd->GetSafeHwnd() == GetDlgItem(IDC_PAN)->GetSafeHwnd())
		{
			pDC->SetTextColor(RGB(38,58,208));
		}
		else if(GetDlgItem(IDC_LABEL)->GetSafeHwnd() == pWnd->GetSafeHwnd())
			pDC->SetTextColor(RGB(0,200,200));
		else
			pDC->SetTextColor(RGB(150,200,60));
		//pDC->SetTextColor(RGB(0,0,0));   
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);               
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CMyNetworkDiskDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_bmp.DeleteObject();   //λͼ
	m_brush.DeleteObject();  //��ˢ
	fon.DeleteObject();
	fon2.DeleteObject();
}


void CMyNetworkDiskDlg::OnBnClickedCancelBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void CMyNetworkDiskDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����������µ�λ��ת�Ƶ���������
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

//��¼��ť
void CMyNetworkDiskDlg::OnBnClickedLoginBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������Դ���
	/*CMyMainDlg     mainDlg;
	theApp.m_pMainWnd = &mainDlg;
	this->OnOK();
	mainDlg.DoModal();
	return;*/
	//����������
	m_UidEdit.GetWindowText(m_uidCStr);
	m_PasswdEdit.GetWindowText(m_passwdCStr);
	CMyTools::EnCodeWCHARToUtf(m_uidCStr.GetBuffer(),m_uidStr);
	CMyTools::EnCodeWCHARToUtf(m_passwdCStr.GetBuffer(),m_passStr);

	bool IsMatch = CMyTools::MatchUid(m_uidStr);
	if(IsMatch == false)
		return ;
	IsMatch = CMyTools::MatchPassword(m_passStr);
	if(IsMatch == false)
		return ;

	LoginRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = LOGIN_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,m_uidStr.c_str(),m_uidStr.size());
	memcpy_s(rq.m_pass,PASSWD_SIZE,m_passStr.c_str(),m_passStr.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}

//ע��
void CMyNetworkDiskDlg::OnBnClickedRegisterBtn()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMyRegisterDlg registerdlg;
	registerdlg.DoModal();
}

//�ɹ���½
LRESULT CMyNetworkDiskDlg::OnLoginSucessDlg(WPARAM w, LPARAM l)
{
	LoginRs* rs = (LoginRs*)w;
	if(rs->m_statuscode == LOGIN_PASSERROR){
		AfxMessageBox(_T("�������,�����ԣ�"));
		return 0;
	}else if(rs->m_statuscode == LOGIN_UEXIST){
		AfxMessageBox(_T("�û�������,����֤�������ԣ�"));
		return 0;
	}

	CString home;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_downloadPath.c_str(),home);
	if(!PathIsDirectory(home + m_uidCStr)){
		CreateDirectory(home + m_uidCStr,NULL);
	}
	CIOFileStream output(theApp.m_home + "login",IO_WRITE);
	output.writeline(m_uidStr);
	int check1,check2;
	check1 = m_RememberPassBtn.GetCheck();
	check2 = m_AutoLogBtn.GetCheck();
	if(check1){
		for(unsigned i = 0; i < m_passStr.size(); i++){
			output.write(~m_passStr[i]);
		}
		output.writeline("");
		if(check2){
			output.writeline("TRUE");
		}else{
			output.writeline("FALSE");
		}
	}else{
		output.writeline("FALSE");
		output.writeline("FALSE");
	}
	output.close();

	CString nick,vip;
	CMyTools::EnCodeUtfToWCHAR(rs->m_nickname,nick);
	vip.Format(_T("vip:%d"),rs->m_vipclass);

	CMyMainDlg mainDlg;
	theApp.SetParams(nick,vip,m_uidStr,rs->m_currcapacity,rs->m_capacity);
	theApp.m_pMainWnd = &mainDlg;
	this->OnOK();
	mainDlg.DoModal();
	return 0;
}


BOOL CMyNetworkDiskDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
