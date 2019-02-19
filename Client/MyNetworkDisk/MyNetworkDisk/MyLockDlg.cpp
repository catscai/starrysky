// MyLockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyLockDlg.h"
#include "afxdialogex.h"
#include "MyTools.h"
#include "packdef.h"
#include "TcpMediator.h"
#include "MyMainDlg.h"
// CMyLockDlg 对话框

IMPLEMENT_DYNAMIC(CMyLockDlg, CDialogEx)

CMyLockDlg::CMyLockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyLockDlg::IDD, pParent)
{

}

CMyLockDlg::~CMyLockDlg()
{
}

void CMyLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UNLOCK_PASS, m_pass);
}


BEGIN_MESSAGE_MAP(CMyLockDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyLockDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(UNLOCK_MAINDLG_RESPONSE_DLG_MSG,&CMyLockDlg::OnUnlockResponse)
	ON_STN_CLICKED(IDC_LOCK_CLOSE, &CMyLockDlg::OnStnClickedLockClose)
	ON_STN_CLICKED(IDC_LOCK_MIN, &CMyLockDlg::OnStnClickedLockMin)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyLockDlg 消息处理程序

//解锁
void CMyLockDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	CString pass;
	m_pass.GetWindowText(pass);
	string p;
	CMyTools::EnCodeWCHARToUtf(pass,p);
	if(CMyTools::MatchPassword(p) == false)
	{
		AfxMessageBox(_T("密码错误"));
		return;
	}
	UnlockRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = UNLOCK_MAINDLG_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_pass,PASSWD_SIZE,p.c_str(),p.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}


BOOL CMyLockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_LOCKDLG_BMP));    //这里将位图加载进资源后，再Load
	m_brush.CreatePatternBrush(&m_bmp);
	fon1,fon2;
	CWnd* wang = GetDlgItem(IDC_WANGPANSUODING);
	CFont* ftmp1 = wang->GetFont();
	LOGFONT lf;
	ftmp1->GetLogFont(&lf);//获取LOGFONT结构体
	lf.lfHeight=50;    //修改字体大小
	lf.lfItalic=TRUE;        //倾斜
	lf.lfWeight=FW_BOLD;   //修改字体的粗细
	lstrcpyW(lf.lfFaceName,_T("宋体"));
	fon1.CreateFontIndirectW(&lf);
	wang->SetFont(&fon1);
	fon2.CreatePointFont(200,_T("宋体"));
	GetDlgItem(IDC_LOCK_CLOSE)->SetFont(&fon2);
	GetDlgItem(IDC_LOCK_MIN)->SetFont(&fon2);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void CMyLockDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_bmp.DeleteObject();   //位图
	m_brush.DeleteObject();  //画刷
}


HBRUSH CMyLockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}else if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{   
		pDC->SetBkMode(TRANSPARENT);
		HWND h = pWnd->GetSafeHwnd();
		if(h == GetDlgItem(IDC_WANGPANSUODING)->GetSafeHwnd()
			|| h == GetDlgItem(IDC_INPUT_PASS)->GetSafeHwnd())
		{
			pDC->SetTextColor(RGB(250,250,0));  
		}else{
			pDC->SetTextColor(RGB(255,255,255));  
		}
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);  
	}
	return hbr;
}

LRESULT CMyLockDlg::OnUnlockResponse(WPARAM w, LPARAM l)
{
	UnlockRs* rs = (UnlockRs*)w;
	if(rs->m_statuscode == UNLOCK_FAILED){
		AfxMessageBox(_T("密码错误"));
	}else{
		CMyMainDlg maindlg;
		theApp.m_pMainWnd = &maindlg;
		this->OnOK();
		maindlg.DoModal();
	}
	return 0;
}

void CMyLockDlg::OnStnClickedLockClose()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SendMessage(WM_CLOSE);
}


void CMyLockDlg::OnStnClickedLockMin()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_MINIMIZE);
}


void CMyLockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}
