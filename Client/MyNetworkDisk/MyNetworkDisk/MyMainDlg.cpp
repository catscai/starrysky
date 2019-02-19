// MyMainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyMainDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "TcpMediator.h"

// CMyMainDlg �Ի���

IMPLEMENT_DYNAMIC(CMyMainDlg, CDialogEx)

CMyMainDlg::CMyMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyMainDlg::IDD, pParent)
{

}

CMyMainDlg::~CMyMainDlg()
{
}

void CMyMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAIN_HEAD, m_XingKong);
	DDX_Control(pDX, IDC_MAIN_RECENT, m_RecentAccess);
	DDX_Control(pDX, IDC_MAIN_SHARE, m_MyShare);
	DDX_Control(pDX, IDC_MAIN_RECYCLE, m_Recycle);
	DDX_Control(pDX, IDC_MAIN_SET, m_SetUp);
	DDX_Control(pDX, IDC_MAIN_NICKNAME, m_NickName);
	DDX_Control(pDX, IDC_MAIN_VIP, m_VipClass);
	DDX_Control(pDX, IDC_MAIN_UPDATE_VIP, m_UpdateVip);
	DDX_Control(pDX, IDC_MAIN_CAPACITY, m_Capacity);
	DDX_Control(pDX, IDC_PROGRESS1, m_CapacityProgress);
	DDX_Control(pDX, IDC_MAIN_MYWANGPAN, m_MyWangPan);
	DDX_Control(pDX, IDC_MAIN_FRIEND_SHARE, m_FriendShare);
	DDX_Control(pDX, IDC_MAIN_TRANFERLIST, m_TransferList);
	DDX_Control(pDX, IDC_MAIN_GETSOURCE, m_GetSource);
	DDX_Control(pDX, IDC_MAIN_CLOSE, m_CloseWindow);
	DDX_Control(pDX, IDC_MAIN_MIN, m_MinWindow);
	DDX_Control(pDX, IDC_MAIN_CHILD_REGION, m_PictureChild);
}


BEGIN_MESSAGE_MAP(CMyMainDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_STN_CLICKED(IDC_MAIN_MYWANGPAN, &CMyMainDlg::OnStnClickedMainMywangpan)
	ON_STN_CLICKED(IDC_MAIN_FRIEND_SHARE, &CMyMainDlg::OnStnClickedMainFriendShare)
	ON_STN_CLICKED(IDC_MAIN_TRANFERLIST, &CMyMainDlg::OnStnClickedMainTranferlist)
	ON_STN_CLICKED(IDC_MAIN_GETSOURCE, &CMyMainDlg::OnStnClickedMainGetsource)
	ON_STN_CLICKED(IDC_MAIN_RECENT, &CMyMainDlg::OnStnClickedMainRecent)
	ON_STN_CLICKED(IDC_MAIN_SHARE, &CMyMainDlg::OnStnClickedMainShare)
	ON_STN_CLICKED(IDC_MAIN_RECYCLE, &CMyMainDlg::OnStnClickedMainRecycle)
	ON_STN_CLICKED(IDC_MAIN_SET, &CMyMainDlg::OnStnClickedMainSet)
	ON_STN_CLICKED(IDC_MAIN_CLOSE, &CMyMainDlg::OnStnClickedMainClose)
	ON_STN_CLICKED(IDC_MAIN_MIN, &CMyMainDlg::OnStnClickedMainMin)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CMyMainDlg ��Ϣ�������


BOOL CMyMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //���ｫλͼ���ؽ���Դ����Load
    m_brush.CreatePatternBrush(&m_bmp);
	this->SetControlFont();

	m_PictureChild.GetWindowRect(&m_PictureRect);
	ScreenToClient(m_PictureRect);
	m_MyWangPanDlg.Create(IDD_MYWANGPAN_DLG,this);
	m_MyWangPanDlg.MoveWindow(m_PictureRect);
	
	m_PictureChild.ShowWindow(SW_HIDE);

	m_MyTransferDlg.Create(IDD_TRANSTER_DLG,this);
	m_MyTransferDlg.MoveWindow(m_PictureRect);
	m_MyTransferDlg.ShowWindow(SW_HIDE);
	m_MyWangPanDlg.ShowWindow(SW_SHOW);

	m_MyFriendsDlg.Create(IDD_FRIENDS_CHAT_DLG,this);
	m_MyFriendsDlg.MoveWindow(m_PictureRect);
	m_MyFriendsDlg.ShowWindow(SW_HIDE);

	m_MyShareDlg.Create(IDD_MYSHARE_DLG,this);
	m_MyShareDlg.MoveWindow(m_PictureRect);
	m_MyShareDlg.ShowWindow(SW_HIDE);

	m_MyExtractDlg.Create(IDD_EXTRACT_FILE_DLG,this);
	m_MyExtractDlg.MoveWindow(m_PictureRect);
	m_MyExtractDlg.ShowWindow(SW_HIDE);

	m_MySetDlg.Create(IDD_MYSET_DLG,this);
	m_MySetDlg.MoveWindow(m_PictureRect);
	m_MySetDlg.ShowWindow(SW_HIDE);

	this->FirstRequestFile();
	CString cap;
	cap.Format(_T("����:%lld/%lld"),theApp.m_currcapacity / (1000 * 1024),theApp.m_capacity / (1000 * 1024));
	this->m_Capacity.SetWindowText(cap);
	this->m_CapacityProgress.SetRange32(0,static_cast<int>(theApp.m_capacity / (1000 * 1024)));
	this->m_CapacityProgress.SetPos(static_cast<int>(theApp.m_currcapacity / (1000 * 1024)));

	this->m_NickName.SetWindowText(theApp.m_nicknameCStr);
	this->m_VipClass.SetWindowText(theApp.m_vip);
	UpdateData();
	CString home,email;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_home.c_str(),home);
	CMyTools::EnCodeUtfToWCHAR(theApp.m_email.c_str(),email);
	if(!PathIsDirectory(home + email + _T("/set/"))){
		CreateDirectory(home + email + _T("/set/"),NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMyMainDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	m_bmp.DeleteObject();   //λͼ
	m_brush.DeleteObject();  //��ˢ
	fon1.DeleteObject();
	fon2.DeleteObject();
	fon3.DeleteObject();
	fon4.DeleteObject();
	fon5.DeleteObject();
	fon6.DeleteObject();
	/*m_MyWangPanDlg.DestroyWindow();
	m_MyTransferDlg.DestroyWindow();
	m_MyFriendsDlg.DestroyWindow();
	m_MyShareDlg.DestroyWindow();
	m_MyExtractDlg.DestroyWindow();
	m_MySetDlg.DestroyWindow();*/
}


HBRUSH CMyMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_DLG){
		return m_brush;
	}
	HWND h = pWnd->GetSafeHwnd();
	if(nCtlColor ==  CTLCOLOR_STATIC) //��̬�ı�����͸��             
	{   
		pDC->SetBkMode(TRANSPARENT);
		if(m_XingKong.GetSafeHwnd() == h){
			pDC->SetTextColor(RGB(0,200,200));
		}else{
			if(m_MyWangPan.GetSafeHwnd() == h || 
				m_FriendShare.GetSafeHwnd() == h ||
				m_TransferList.GetSafeHwnd() == h ||
				m_GetSource.GetSafeHwnd() == h ||
				m_NickName.GetSafeHwnd() == h ||
				m_VipClass.GetSafeHwnd() == h ||
				m_UpdateVip.GetSafeHwnd() == h ||
				m_Capacity.GetSafeHwnd() == h ||
				m_RecentAccess.GetSafeHwnd() == h ||
				m_Recycle.GetSafeHwnd() == h ||
				m_SetUp.GetSafeHwnd() == h ||
				m_MyShare.GetSafeHwnd() == h){
				//pDC->SetTextColor(RGB(38,58,208));
				pDC->SetTextColor(RGB(128,128,255));
			}else if(GetDlgItem(IDC_1)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_2)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_3)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_4)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_5)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_6)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_7)->GetSafeHwnd() == h
				|| GetDlgItem(IDC_8)->GetSafeHwnd() == h){
				pDC->SetTextColor(RGB(238,98,154));
			}else if(m_CloseWindow.GetSafeHwnd() == h
				|| m_MinWindow.GetSafeHwnd() == h){
				pDC->SetTextColor(RGB(255,255,255));
			}
			else if(GetDlgItem(IDC_FRIEND_MESSAGE)->GetSafeHwnd() == h){
				pDC->SetTextColor(RGB(255,10,10));
			}else{
				pDC->SetTextColor(RGB(255,255,255));
			}
			
		}
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);   
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void	CMyMainDlg::SetControlFont()
{
	CFont* ftmp1 = m_XingKong.GetFont();
	LOGFONT lf,lf2,lf3,lf4;
	ftmp1->GetLogFont(&lf);//��ȡLOGFONT�ṹ��
	lf.lfHeight=50;    //�޸������С
	lf.lfItalic=TRUE;        //��б
	lf.lfWeight=FW_BOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf.lfFaceName,_T("����"));
	fon1.CreateFontIndirectW(&lf);
	m_XingKong.SetFont(&fon1);
	ftmp1 = m_RecentAccess.GetFont();
	ftmp1->GetLogFont(&lf2);
	lf2.lfHeight=25;    //�޸������С
	lf2.lfItalic=FALSE;        //��б
	lf2.lfWeight=500;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf2.lfFaceName,_T("����"));
	fon2.CreateFontIndirectW(&lf2);
	m_RecentAccess.SetFont(&fon2);
	m_MyShare.SetFont(&fon2);
	m_Recycle.SetFont(&fon2);
	m_SetUp.SetFont(&fon2);
	fon3.CreatePointFont(120,_T("����"));
	m_NickName.SetFont(&fon3);
	m_VipClass.SetFont(&fon3);
	ftmp1 = m_UpdateVip.GetFont();
	ftmp1->GetLogFont(&lf3);
	lf3.lfHeight=25;    //�޸������С
	lf3.lfItalic=FALSE;        //��б
	lf3.lfWeight=FW_EXTRABOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf3.lfFaceName,_T("����"));
	fon4.CreateFontIndirectW(&lf3);
	m_UpdateVip.SetFont(&fon4);

	ftmp1 = m_MyWangPan.GetFont();
	ftmp1->GetLogFont(&lf4);//��ȡLOGFONT�ṹ��
	lf4.lfHeight=30;    //�޸������С
	lf4.lfItalic=TRUE;        //��б
	lf4.lfWeight=FW_BOLD;   //�޸�����Ĵ�ϸ
	lstrcpyW(lf4.lfFaceName,_T("����"));
	fon5.CreateFontIndirectW(&lf4);
	m_MyWangPan.SetFont(&fon5);
	m_FriendShare.SetFont(&fon5);
	m_TransferList.SetFont(&fon5);
	m_GetSource.SetFont(&fon5);

	fon6.CreatePointFont(150,_T("����"));
	m_CloseWindow.SetFont(&fon6);
	m_MinWindow.SetFont(&fon6);
}	

void    CMyMainDlg::HideAllStatic()
{
	GetDlgItem(IDC_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_8)->ShowWindow(SW_HIDE);
}


void CMyMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDialogEx::OnMouseMove(nFlags, point);
}

//�ҵ�����
void CMyMainDlg::OnStnClickedMainMywangpan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_1)->ShowWindow(SW_SHOW);
	if(m_MyWangPanDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		m_MyWangPanDlg.ShowWindow(SW_SHOW);
	}
}

//�ҵĺ���
void CMyMainDlg::OnStnClickedMainFriendShare()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_2)->ShowWindow(SW_SHOW);
	if(m_MyFriendsDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		this->m_MyFriendsDlg.ShowWindow(SW_SHOW);
		this->GetDlgItem(IDC_FRIEND_MESSAGE)->ShowWindow(SW_HIDE);
	}
}

//�����б�
void CMyMainDlg::OnStnClickedMainTranferlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_3)->ShowWindow(SW_SHOW);
	if(m_MyTransferDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		this->m_MyTransferDlg.ShowWindow(SW_SHOW);
	}
}

//��ȡ��Դ
void CMyMainDlg::OnStnClickedMainGetsource()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_4)->ShowWindow(SW_SHOW);
	if(m_MyExtractDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		m_MyExtractDlg.ShowWindow(SW_SHOW);
	}
}

//�������
void CMyMainDlg::OnStnClickedMainRecent()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_5)->ShowWindow(SW_SHOW);
}

//�ҵķ���
void CMyMainDlg::OnStnClickedMainShare()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_6)->ShowWindow(SW_SHOW);
	if(m_MyShareDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		this->m_MyShareDlg.ShowWindow(SW_SHOW);
	}
}

//����վ
void CMyMainDlg::OnStnClickedMainRecycle()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_7)->ShowWindow(SW_SHOW);
}

//����
void CMyMainDlg::OnStnClickedMainSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->HideAllStatic();
	GetDlgItem(IDC_8)->ShowWindow(SW_SHOW);
	if(m_MySetDlg.IsWindowVisible() == FALSE)
	{
		this->HideAllDialogs();
		this->m_MySetDlg.ShowWindow(SW_SHOW);
	}
}

//�رմ���
void CMyMainDlg::OnStnClickedMainClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->SendMessage(WM_CLOSE);
}

//��С������
void CMyMainDlg::OnStnClickedMainMin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->ShowWindow(SW_MINIMIZE);
}


void CMyMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����������µ�λ��ת�Ƶ���������
	PostMessage(WM_NCLBUTTONDOWN,
    HTCAPTION,
    MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

//��ʼ�� ��һ�������ļ�
void CMyMainDlg::FirstRequestFile()
{
	FileListRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = FILELIST_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_dir,DIR_SIZE,"/",1);
	theApp.net()->send((char*)&rq,rq.m_len);
}




BOOL CMyMainDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

void	CMyMainDlg::HideAllDialogs()
{
	this->m_MyTransferDlg.ShowWindow(SW_HIDE);
	this->m_MyWangPanDlg.ShowWindow(SW_HIDE);
	this->m_MyFriendsDlg.ShowWindow(SW_HIDE);
	this->m_MyShareDlg.ShowWindow(SW_HIDE);
	this->m_MyExtractDlg.ShowWindow(SW_HIDE);
	this->m_MySetDlg.ShowWindow(SW_HIDE);
	UpdateData();
}