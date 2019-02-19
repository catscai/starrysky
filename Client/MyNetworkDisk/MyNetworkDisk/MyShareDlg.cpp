// MyShareDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyShareDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"

// CMyShareDlg 对话框

IMPLEMENT_DYNAMIC(CMyShareDlg, CDialogEx)

CMyShareDlg::CMyShareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyShareDlg::IDD, pParent)
{

}

CMyShareDlg::~CMyShareDlg()
{
}

void CMyShareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHARE_FILE_LIST, m_ShareFileListControl);
}


BEGIN_MESSAGE_MAP(CMyShareDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyShareDlg::OnBnClickedButton1)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(SHARE_FILE_LIST_RESPONSE_DLG_MSG,&CMyShareDlg::OnShareFileListResponse)
	ON_NOTIFY(NM_CLICK, IDC_SHARE_FILE_LIST, &CMyShareDlg::OnNMClickShareFileList)
END_MESSAGE_MAP()


// CMyShareDlg 消息处理程序

//取消分享
void CMyShareDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	ShareFileDelete sfd;
	memset(&sfd,0,sizeof(sfd));
	sfd.m_len = sizeof(sfd);
	sfd.m_type = SHARE_FILE_CANCEL_MSG;
	memcpy_s(sfd.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	int nRow = m_ShareFileListControl.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_ShareFileListControl.GetCheck(i))
		{
			path = m_ShareFileListControl.GetItemText(i,0);
			if(m_sharefileinfoMap.count(path))
			{
				auto info = m_sharefileinfoMap[path];
				memcpy_s(sfd.m_dir,DIR_SIZE,info->m_dir,strlen(info->m_dir));
				memcpy_s(sfd.m_name,FILENAME_LENGTH,info->m_name,strlen(info->m_name));

				theApp.net()->send((char*)&sfd,sfd.m_len);
				m_ShareFileListControl.DeleteItem(i);
				memset(sfd.m_dir,0,DIR_SIZE);
				memset(sfd.m_name,0,FILENAME_LENGTH);
			}
		}
	}
}


BOOL CMyShareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bmp.LoadBitmap(MAKEINTRESOURCE(IDB_MAIN_BMP));    //这里将位图加载进资源后，再Load
	m_brush.CreatePatternBrush(&m_bmp);
	DWORD dwStyle = m_ShareFileListControl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |=  LVS_EX_CHECKBOXES | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;//item前生成checkbox控件
	m_ShareFileListControl.SetExtendedStyle(dwStyle);

	m_ShareFileListControl.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,300 );
	m_ShareFileListControl.InsertColumn(1,_T("分享时间"),LVCFMT_LEFT,150 );
	m_ShareFileListControl.InsertColumn(2,_T("剩余时间"),LVCFMT_LEFT,100 );
	m_ShareFileListControl.InsertColumn(3,_T("保存次数"),LVCFMT_LEFT,100 );
	m_ShareFileListControl.InsertColumn(4,_T("提取码"),LVCFMT_LEFT,100 );
	m_sharedlg.Create(IDD_SHARE_FILE_DLG,this);
	m_sharedlg.ShowWindow(SW_HIDE);
	this->RequestShareFileList();
	//UpdateData();
	return TRUE;  // return TRUE unless you set the focus to a control

}


void CMyShareDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_brush.DeleteObject();
	m_bmp.DeleteObject();

	auto ite = m_sharefileinfoMap.begin();
	while(ite != m_sharefileinfoMap.end())
	{
		delete ite->second;
		ite->second = NULL;
		++ite;
	}
	m_sharefileinfoMap.clear();
	m_sharedlg.DestroyWindow();
}


HBRUSH CMyShareDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG)
	{
		return m_brush;
	}
	if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{              
		pDC->SetBkMode(TRANSPARENT);   
		//pDC->SetTextColor(RGB(150,200,60));
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);               
	}
	return hbr;
}

void	CMyShareDlg::ShowShareFileDlg()
{
	if(m_sharedlg.IsWindowVisible())
		m_sharedlg.ShowWindow(SW_HIDE);
	else
		m_sharedlg.ShowWindow(SW_SHOW);
}

void	CMyShareDlg::SetShareFileDlgParams(const CString& fdir,const CString& fname)
{
	m_sharedlg.SetDirName(fdir,fname);
}

void	CMyShareDlg::SetShareFileDlgFName(const CString& path)
{
	m_sharedlg.SetInitParams(path);
}

bool	CMyShareDlg::IsShareShowing()
{
	if(m_sharedlg.IsWindowVisible())
		return true;
	return false;
}

void	CMyShareDlg::AddShareFile(const CString& fname,const CString& sharetime,const 
		CString& days,const CString& savetime,const CString& getcode)
{
	int nRow = m_ShareFileListControl.GetItemCount();
	m_ShareFileListControl.InsertItem(nRow,NULL);
	m_ShareFileListControl.SetItemText(nRow,0,fname);
	m_ShareFileListControl.SetItemText(nRow,1,sharetime);
	m_ShareFileListControl.SetItemText(nRow,2,days);
	m_ShareFileListControl.SetItemText(nRow,3,savetime);
	m_ShareFileListControl.SetItemText(nRow,4,getcode);
	UpdateData();
}

void	CMyShareDlg::RequestShareFileList()
{
	ShareFileListRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = SHARE_FILE_LIST_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());

	theApp.net()->send((char*)&rq,rq.m_len);
}

LRESULT CMyShareDlg::OnShareFileListResponse(WPARAM w, LPARAM l)
{
	ShareFileListRs* rs = (ShareFileListRs*)w;
	CString fname,fdir,path,savetime,starttime,getcode,days;
	for(int i = 0; i < rs->m_sharelistcount; i++)
	{
		auto& info = rs->m_sharelist[i];
		CMyTools::EnCodeUtfToWCHAR(info.m_dir,fdir);
		CMyTools::EnCodeUtfToWCHAR(info.m_name,fname);
		CMyTools::EnCodeUtfToWCHAR(info.m_getcode,getcode);
		path = fdir + fname;
		savetime.Format(_T("%d"),info.m_savetime);
		CMyTools::EnCodeUtfToWCHAR(info.m_starttime,starttime);
		days.Format(_T("%d 天"),info.m_days);
		this->AddShareFile(path,starttime,days,savetime,getcode);
		ShareFileInfo* ff = new ShareFileInfo;
		memset(ff,0,sizeof(ShareFileInfo));
		ff->m_days = info.m_days;
		ff->m_savetime = info.m_savetime;
		memcpy_s(ff->m_dir,DIR_SIZE,info.m_dir,strlen(info.m_dir));
		memcpy_s(ff->m_name,FILENAME_LENGTH,info.m_name,strlen(info.m_name));
		memcpy_s(ff->m_getcode,SHARE_FILE_GETCODE_LENGTH,info.m_getcode,strlen(info.m_getcode));
		memcpy_s(ff->m_starttime,SHARE_START_TIME_LENGTH,info.m_starttime,strlen(info.m_starttime));
		m_sharefileinfoMap[path] = ff;
	}
	return 0;
}

void CMyShareDlg::OnNMClickShareFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		if(this->m_ShareFileListControl.GetCheck(pNMListView->iItem))
			this->m_ShareFileListControl.SetCheck(pNMListView->iItem,FALSE);
		else
			this->m_ShareFileListControl.SetCheck(pNMListView->iItem,TRUE);
	} 
	*pResult = 0;
}
