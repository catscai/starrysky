// MyTransferUploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyTransferUploadDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
#include "MyMainDlg.h"
#include <fstream>
using namespace std;
// CMyTransferUploadDlg 对话框

IMPLEMENT_DYNAMIC(CMyTransferUploadDlg, CDialogEx)

CMyTransferUploadDlg::CMyTransferUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTransferUploadDlg::IDD, pParent)
{

}

CMyTransferUploadDlg::~CMyTransferUploadDlg()
{
}

void CMyTransferUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UPLOAD_LIST, m_UploadList);
}


BEGIN_MESSAGE_MAP(CMyTransferUploadDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_COMMAND(ID_UPLOAD_PAUSE, &CMyTransferUploadDlg::OnUploadPause)
	ON_COMMAND(ID_UPLOAD_CONTINUE, &CMyTransferUploadDlg::OnUploadContinue)
	ON_COMMAND(ID_UPLOAD_DELETE, &CMyTransferUploadDlg::OnUploadDelete)
	ON_COMMAND(ID_UPLOAD_PAUSE_ALL, &CMyTransferUploadDlg::OnUploadPauseAll)
	ON_COMMAND(ID_UPLOAD_CONTINUE_ALL, &CMyTransferUploadDlg::OnUploadContinueAll)
	ON_COMMAND(ID_UPLOAD_DELETE_ALL, &CMyTransferUploadDlg::OnUploadDeleteAll)
	ON_NOTIFY(NM_RCLICK, IDC_UPLOAD_LIST, &CMyTransferUploadDlg::OnNMRClickUploadList)
	ON_NOTIFY(NM_CLICK, IDC_UPLOAD_LIST, &CMyTransferUploadDlg::OnNMClickUploadList)
END_MESSAGE_MAP()


// CMyTransferUploadDlg 消息处理程序


BOOL CMyTransferUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_menu.LoadMenu(IDR_UP_MENU);
	DWORD dwStyle = m_UploadList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |=  LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;//item前生成checkbox控件
	m_UploadList.SetExtendedStyle(dwStyle);

	m_UploadList.InsertColumn(0,_T("文件名"),LVCFMT_LEFT,300 );
	m_UploadList.InsertColumn(1,_T("进度"),LVCFMT_LEFT,350 );
	m_UploadList.InsertColumn(2,_T("大小"),LVCFMT_LEFT,100 );
	m_UploadList.InsertColumn(3,_T("状态"),LVCFMT_LEFT,100 );

	m_info.flags=LVFI_PARTIAL;

	fstream  fin(theApp.m_downloadPath + theApp.m_email + "/set/" + "uploadinfo",ios::in);
	if(fin.is_open())
	{
		string count;
		int time = 0;
		if(!fin.eof()){
			fin>>count;
			time = atoi(count.c_str());
		}
		auto& dlg = ((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg;
		for(int i = 0; i < time; i++)
		{
			UploadFileInfo* info = new UploadFileInfo;
			fin>>info->m_name;
			fin>>info->m_dir;
			fin>>info->m_curdir;
			fin>>info->m_p;
			fin>>info->m_md5code;
			fin>>info->m_filesize;
			fin>>info->m_seek;
			fin>>info->m_nIteam;
			fin>>info->m_statuscode;
			info->m_input.open(info->m_p,ios::binary | ios::in);

			dlg.AddUploadTask(info);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control

}


void CMyTransferUploadDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_menu.DestroyMenu();
}

void	CMyTransferUploadDlg::AddUploadTask(UploadFileInfo* info)
{
	CString fname,fsize,fdir;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	if(!m_UploadMap.count(fdir + fname))
	{
		int nRow = m_UploadList.GetItemCount();
		m_UploadList.InsertItem(nRow,NULL);
		info->m_nIteam = nRow;
		CMyTools::CalFileSize(info->m_filesize,fsize);
		m_UploadList.SetItemText(nRow,0,fdir + fname);
		m_UploadList.SetItemData(nRow,0);
		m_UploadList.SetItemText(nRow,2,fsize);
		m_UploadList.SetItemText(nRow,3,_T("等待上传"));
		m_UploadMap[fdir + fname] = info;
	}
}

void	CMyTransferUploadDlg::FinishTask(UploadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_UploadList.FindItem(&m_info);
	m_UploadList.SetItemText(currItem,3,_T("上传文件完成"));
	m_UploadList.DeleteItem(currItem);
	m_UploadMap.erase(fdir + fname);
	UpdateData();
}

void	CMyTransferUploadDlg::PauseTask(UploadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_UploadList.FindItem(&m_info);
	info->m_nIteam = currItem;
	m_UploadList.SetItemText(currItem,3,_T("暂停中"));
	UpdateData();
}

void	CMyTransferUploadDlg::UpdateRate(UploadFileInfo* info)
{
	CString fname,fdir;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	CString path = (fdir + fname);
	m_info.psz = path;
	int currItem = m_UploadList.FindItem(&m_info);
	m_UploadList.SetItemText(currItem,3,_T("正在上传"));
	double cur = static_cast<double>(info->m_seek);
	double end = static_cast<double>(info->m_filesize);
	double tmp = (cur/end)*100;
	int rate = static_cast<int>(tmp);
	m_UploadList.SetItemData(currItem,rate);
	UpdateData();
}

void	CMyTransferUploadDlg::DeleteTask(UploadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_UploadList.FindItem(&m_info);
	m_UploadList.DeleteItem(currItem);
	m_UploadMap.erase(path);
}

void CMyTransferUploadDlg::OnUploadPause()
{
	// TODO: 在此添加命令处理程序代码
	UploadPause rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = UPLOAD_PAUSE_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_UploadList.GetCheck(i))
		{
			path = m_UploadList.GetItemText(i,0);
			auto info = m_UploadMap[path];
			memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
			theApp.net()->send((char*)&rq,rq.m_len);
			info->m_statuscode = FILE_PAUSE;
			//this->PauseTask(info);
			memset(rq.m_md5code,0,FILE_MD5CODE_SIZE);
		}
	}
}


void CMyTransferUploadDlg::OnUploadContinue()
{
	// TODO: 在此添加命令处理程序代码
	UploadFileHeadRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = UPLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_UploadList.GetCheck(i))
		{
			path = m_UploadList.GetItemText(i,0);
			auto info = m_UploadMap[path];
			memcpy_s(rq.m_dir,DIR_SIZE,info->m_dir.c_str(),info->m_dir.size());
			memcpy_s(rq.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
			memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
			rq.m_filesize = info->m_filesize;
			rq.m_seek = info->m_seek;
			info->m_statuscode = FILE_TRANSFER;
			theApp.net()->send((char*)&rq,rq.m_len);
			
			memset(rq.m_md5code,0,FILE_MD5CODE_SIZE);
			memset(rq.m_name,0,FILENAME_LENGTH);
			memset(rq.m_dir,0,DIR_SIZE);
		}
	}
}


void CMyTransferUploadDlg::OnUploadDelete()
{
	// TODO: 在此添加命令处理程序代码
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_UploadList.GetCheck(i))
		{
			path = m_UploadList.GetItemText(i,0);
			if(m_UploadMap.count(path)){
				auto info = m_UploadMap[path];
				if(info->m_statuscode == FILE_TRANSFER)
					info->m_statuscode = FILE_DELETE;
				else{
					this->DeleteTask(info);
					((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg.DeleteUploadTask(info);
				}
			}
		}
	}
}


void CMyTransferUploadDlg::OnUploadPauseAll()
{
	// TODO: 在此添加命令处理程序代码
	UploadPause rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = UPLOAD_PAUSE_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_UploadList.GetItemText(i,0);
		auto info = m_UploadMap[path];
		memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
		theApp.net()->send((char*)&rq,rq.m_len);
		info->m_statuscode = FILE_PAUSE;
		//this->PauseTask(info);
		memset(rq.m_md5code,0,FILE_MD5CODE_SIZE);
	}
}


void CMyTransferUploadDlg::OnUploadContinueAll()
{
	// TODO: 在此添加命令处理程序代码
	UploadFileHeadRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = UPLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_UploadList.GetItemText(i,0);
		auto info = m_UploadMap[path];
		memcpy_s(rq.m_dir,DIR_SIZE,info->m_dir.c_str(),info->m_dir.size());
		memcpy_s(rq.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
		memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
		rq.m_filesize = info->m_filesize;
		rq.m_seek = info->m_seek;
		info->m_statuscode = FILE_TRANSFER;
		theApp.net()->send((char*)&rq,rq.m_len);
			
		memset(rq.m_md5code,0,FILE_MD5CODE_SIZE);
		memset(rq.m_name,0,FILENAME_LENGTH);
		memset(rq.m_dir,0,DIR_SIZE);
		
	}
}


void CMyTransferUploadDlg::OnUploadDeleteAll()
{
	// TODO: 在此添加命令处理程序代码
	CString path;
	int nRow = m_UploadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_UploadList.GetItemText(i,0);
		if(m_UploadMap.count(path)){
			auto info = m_UploadMap[path];
			if(info->m_statuscode == FILE_TRANSFER)
				info->m_statuscode = FILE_DELETE;
			else{
				this->DeleteTask(info);
				((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg.DeleteUploadTask(info);
			}
		}
		
	}
}


void CMyTransferUploadDlg::OnNMRClickUploadList(NMHDR *pNMHDR, LRESULT *pResult)
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


void CMyTransferUploadDlg::OnNMClickUploadList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		if(this->m_UploadList.GetCheck(pNMListView->iItem))
			this->m_UploadList.SetCheck(pNMListView->iItem,FALSE);
		else
			this->m_UploadList.SetCheck(pNMListView->iItem,TRUE);
	} 
	*pResult = 0;
}
