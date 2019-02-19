// MyNetworkDiskDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMyNetworkDiskDlg 对话框



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


// CMyNetworkDiskDlg 消息处理程序

BOOL CMyNetworkDiskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_IsAutoLoginFlag = false;	//是否是自动登录
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_LOGIN_BMP));    //这里将位图加载进资源后，再Load
    m_brush.CreatePatternBrush(&m_bmp); 
	CStatic* pass = (CStatic*)GetDlgItem(IDC_PASSWD);
	CStatic* uid = (CStatic*)GetDlgItem(IDC_UID);
	CStatic* wang = (CStatic*)GetDlgItem(IDC_WANG);
	CStatic* pan = (CStatic*)GetDlgItem(IDC_PAN);
	fon.CreatePointFont(150,_T("宋体"),NULL);
	pass->SetFont(&fon);
	uid->SetFont(&fon);
	m_brushEdit.CreateSolidBrush(RGB(255,255,255));
	CFont* ftmp1 = wang->GetFont();
	LOGFONT lf;
	ftmp1->GetLogFont(&lf);//获取LOGFONT结构体
	lf.lfHeight=120;    //修改字体大小
	lf.lfItalic=FALSE;        //倾斜
	lf.lfWeight=FW_BOLD;   //修改字体的粗细
	lstrcpyW(lf.lfFaceName,_T("宋体"));
	//fon2->CreatePointFont(180,_T("楷体"));
	fon2.CreateFontIndirectW(&lf);
	wang->SetFont(&fon2);
	pan->SetFont(&fon2);
	wang->SetWindowText(_T("星"));
	pan->SetWindowText(_T("空"));
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
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyNetworkDiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if(nCtlColor == CTLCOLOR_DLG)   //此处设置为窗体透明，CTLCOLOR_DLG表示对话框  
	{  
		CBrush *brush;  
		brush = new CBrush(RGB(255,255,255));  
		return (HBRUSH)(brush->m_hObject);  
	}  */
	if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
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
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CMyNetworkDiskDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_bmp.DeleteObject();   //位图
	m_brush.DeleteObject();  //画刷
	fon.DeleteObject();
	fon2.DeleteObject();
}


void CMyNetworkDiskDlg::OnBnClickedCancelBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}


void CMyNetworkDiskDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//将鼠标左键按下的位置转移到标题条上
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

//登录按钮
void CMyNetworkDiskDlg::OnBnClickedLoginBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	//界面测试代码
	/*CMyMainDlg     mainDlg;
	theApp.m_pMainWnd = &mainDlg;
	this->OnOK();
	mainDlg.DoModal();
	return;*/
	//测试主界面
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

//注册
void CMyNetworkDiskDlg::OnBnClickedRegisterBtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyRegisterDlg registerdlg;
	registerdlg.DoModal();
}

//成功登陆
LRESULT CMyNetworkDiskDlg::OnLoginSucessDlg(WPARAM w, LPARAM l)
{
	LoginRs* rs = (LoginRs*)w;
	if(rs->m_statuscode == LOGIN_PASSERROR){
		AfxMessageBox(_T("密码错误,请重试！"));
		return 0;
	}else if(rs->m_statuscode == LOGIN_UEXIST){
		AfxMessageBox(_T("用户不存在,请验证好再重试！"));
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
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
