// MyTransferFinishDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyTransferFinishDlg.h"
#include "afxdialogex.h"
#include "MyTools.h"
#include "packdef.h"
#include <fstream>
using namespace std;
// CMyTransferFinishDlg �Ի���

IMPLEMENT_DYNAMIC(CMyTransferFinishDlg, CDialogEx)

CMyTransferFinishDlg::CMyTransferFinishDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTransferFinishDlg::IDD, pParent)
{

}

CMyTransferFinishDlg::~CMyTransferFinishDlg()
{
}

void CMyTransferFinishDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FINISH_LIST, m_FinishList);
}


BEGIN_MESSAGE_MAP(CMyTransferFinishDlg, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_FINISH_LIST, &CMyTransferFinishDlg::OnNMClickFinishList)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_FINISH_LIST, &CMyTransferFinishDlg::OnNMRClickFinishList)
	ON_COMMAND(ID_FINISG_CLEAR, &CMyTransferFinishDlg::OnFinisgClear)
END_MESSAGE_MAP()


// CMyTransferFinishDlg ��Ϣ�������


BOOL CMyTransferFinishDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DWORD dwStyle = m_FinishList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;//itemǰ����checkbox�ؼ�
	m_FinishList.SetExtendedStyle(dwStyle);

	m_FinishList.InsertColumn(0,_T("�ļ���"),LVCFMT_LEFT,400 );
	m_FinishList.InsertColumn(1,_T("��С"),LVCFMT_LEFT,100 );
	m_FinishList.InsertColumn(2,_T("״̬"),LVCFMT_LEFT,100 );
	m_FinishList.InsertColumn(3,_T("ʱ��"),LVCFMT_LEFT,130 );
	m_menu.LoadMenu(IDR_FINISH_MENU);

	fstream fin(theApp.m_downloadPath + theApp.m_email + "/set/" + "finishinfo",ios::in);
	if(fin.is_open())
	{
		string name,size,status,time;
		CString fname,fsize,fstatus,ftime;
		char str[500] = {0};
		while(!fin.eof())
		{
			fin.getline(str,500);
			name = str;
			if(name.empty()) break;
			fin.getline(str,500);
			size = str;
			if(size.empty()) break;
			fin.getline(str,500);
			status = str;
			if(status.empty()) break;
			fin.getline(str,500);
			time = str;
			if(time.empty()) break;
			CMyTools::EnCodeUtfToWCHAR(name.c_str(),fname);
			CMyTools::EnCodeUtfToWCHAR(size.c_str(),fsize);
			CMyTools::EnCodeUtfToWCHAR(status.c_str(),fstatus);
			CMyTools::EnCodeUtfToWCHAR(time.c_str(),ftime);
			this->InsertFinishListItem(fname,fsize,fstatus,ftime);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void	CMyTransferFinishDlg::AddUploadFinishItemToList(UploadFileInfo* up)
{
	CString fname,fdir,fsize,ftime;
	CMyTools::EnCodeUtfToWCHAR(up->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(up->m_name.c_str(),fname);
	CMyTools::CalFileSize(up->m_filesize,fsize);
	CMyTools::NowTime(ftime);

	m_FinishList.InsertItem(0,NULL);
	m_FinishList.SetItemText(0,0,fdir + fname);
	m_FinishList.SetItemText(0,1,fsize);
	m_FinishList.SetItemText(0,2,_T("�ϴ����"));
	m_FinishList.SetItemText(0,3,ftime);
	string t,s;
	CMyTools::EnCodeWCHARToUtf(ftime.GetBuffer(),t);
	CMyTools::EnCodeWCHARToUtf(fsize,s);
	FinishRecord* record = new FinishRecord(up->m_curdir + up->m_name,s,"�ϴ����",t);
	m_FinishStack.push(record);
}

void	CMyTransferFinishDlg::AddDownloadFinishItemToList(DownloadFileInfo* down)
{
	CString fname,fdir,fsize,ftime;
	CMyTools::EnCodeUtfToWCHAR(down->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(down->m_name.c_str(),fname);
	CMyTools::CalFileSize(down->m_filesize,fsize);
	CMyTools::NowTime(ftime);

	m_FinishList.InsertItem(0,NULL);
	m_FinishList.SetItemText(0,0,fdir + fname);
	m_FinishList.SetItemText(0,1,fsize);
	m_FinishList.SetItemText(0,2,_T("�������"));
	m_FinishList.SetItemText(0,3,ftime);
	string t,s;
	CMyTools::EnCodeWCHARToUtf(ftime.GetBuffer(),t);
	CMyTools::EnCodeWCHARToUtf(fsize,s);
	FinishRecord* record = new FinishRecord(down->m_dir + down->m_name,s,"�������",t);
	m_FinishStack.push(record);
}

void CMyTransferFinishDlg::OnNMClickFinishList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		if(this->m_FinishList.GetCheck(pNMListView->iItem))
			this->m_FinishList.SetCheck(pNMListView->iItem,FALSE);
		else
			this->m_FinishList.SetCheck(pNMListView->iItem,TRUE);
	} 
	*pResult = 0;
}


void CMyTransferFinishDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	fstream fout(theApp.m_downloadPath + theApp.m_email + "/set/" + "finishinfo",ios::out | ios::app);
	while(!m_FinishStack.empty())
	{
		FinishRecord* record = m_FinishStack.top();
		m_FinishStack.pop();
		fout<<record->name<<endl;
		fout<<record->size<<endl;
		fout<<record->status<<endl;
		fout<<record->time<<endl;
		delete record;
		record = NULL;
	}
	fout.close();

	m_menu.DestroyMenu();
}

void	CMyTransferFinishDlg::InsertFinishListItem(const CString& n,
		const CString& s,const CString& ss,const CString& t)
{
	m_FinishList.InsertItem(0,NULL);
	m_FinishList.SetItemText(0,0,n);
	m_FinishList.SetItemText(0,1,s);
	m_FinishList.SetItemText(0,2,ss);
	m_FinishList.SetItemText(0,3,t);
}

void CMyTransferFinishDlg::OnNMRClickFinishList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
    
        CMenu* popup = m_menu.GetSubMenu(0);
		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	} 
	*pResult = 0;
}


void CMyTransferFinishDlg::OnFinisgClear()
{
	// TODO: �ڴ���������������
	m_FinishList.DeleteAllItems();
	fstream fout(theApp.m_downloadPath + theApp.m_email + '/' + "finishinfo",ios::out);
	if(fout.is_open())
	{
		fout.clear();
		fout.close();
	}
}
