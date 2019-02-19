// MyTransferDownloadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyTransferDownloadDlg.h"
#include "afxdialogex.h"
#include "MyTools.h"
#include "packdef.h"
#include "TcpMediator.h"
#include "MyMainDlg.h"
#include <fstream>
using namespace std;
// CMyTransferDownloadDlg �Ի���

IMPLEMENT_DYNAMIC(CMyTransferDownloadDlg, CDialogEx)

CMyTransferDownloadDlg::CMyTransferDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTransferDownloadDlg::IDD, pParent)
{

}

CMyTransferDownloadDlg::~CMyTransferDownloadDlg()
{
}

void CMyTransferDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DOWNLOAD_LIST, m_DownloadList);
}


BEGIN_MESSAGE_MAP(CMyTransferDownloadDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_DOWNLOAD_LIST, &CMyTransferDownloadDlg::OnNMRClickDownloadList)
	ON_COMMAND(ID_DOWNLOAD_PAUSE, &CMyTransferDownloadDlg::OnDownloadPause)
	ON_COMMAND(ID_DOWNLOAD_CONTINUE, &CMyTransferDownloadDlg::OnDownloadContinue)
	ON_COMMAND(ID_DOWNLOAD_DELETE, &CMyTransferDownloadDlg::OnDownloadDelete)
	ON_COMMAND(ID_DOWNLOAD_PAUSE_ALL, &CMyTransferDownloadDlg::OnDownloadPauseAll)
	ON_COMMAND(ID_DOWNLOAD_CONTINUE_ALL, &CMyTransferDownloadDlg::OnDownloadContinueAll)
	ON_COMMAND(ID_DOWNLOAD_DELETE_ALL, &CMyTransferDownloadDlg::OnDownloadDeleteAll)
	ON_NOTIFY(NM_CLICK, IDC_DOWNLOAD_LIST, &CMyTransferDownloadDlg::OnNMClickDownloadList)
END_MESSAGE_MAP()


// CMyTransferDownloadDlg ��Ϣ�������


BOOL CMyTransferDownloadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_menu.LoadMenu(IDR_DOWN_MENU);
	DWORD dwStyle = m_DownloadList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl�� 
	dwStyle |=  LVS_EX_CHECKBOXES | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;//itemǰ����checkbox�ؼ�
	m_DownloadList.SetExtendedStyle(dwStyle);

	m_DownloadList.InsertColumn(0,_T("�ļ���"),LVCFMT_LEFT,300 );
	m_DownloadList.InsertColumn(1,_T("����"),LVCFMT_LEFT,350 );
	m_DownloadList.InsertColumn(2,_T("��С"),LVCFMT_LEFT,100 );
	m_DownloadList.InsertColumn(3,_T("״̬"),LVCFMT_LEFT,100 );
	m_info.flags=LVFI_PARTIAL;

	//���ļ��ж�ȡ������Ϣ
	fstream fin(theApp.m_downloadPath + theApp.m_email + "/set/" + "downloadinfo",ios::in);
	if(fin.is_open())
	{
		string count;
		int time = 0;
		auto& dlg = ((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg;
		if(!fin.eof())
		{
			fin>>count;
			time = atoi(count.c_str());
		}
		for(int i = 0; i < time; i++)
		{
			if(fin.eof())
				break;
			DownloadFileInfo* info = NULL;
			info = new DownloadFileInfo;
			fin>>info->m_name;
			fin>>info->m_dir;
			fin>>info->m_curdir;
			fin>>info->m_seek;
			fin>>info->m_filesize;
			fin>>info->m_nIteam;
			fin>>info->m_statuscode;
			info->m_output.open(info->m_dir + info->m_name,ios::binary | ios::out);

			dlg.AddDownloadTask(info);
		}
		fin.close();
	}
	return TRUE;  // return TRUE unless you set the focus to a control

}


void CMyTransferDownloadDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_menu.DestroyMenu();
}


HBRUSH CMyTransferDownloadDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void	CMyTransferDownloadDlg::AddDownTask(DownloadFileInfo* info)
{
	CString fname,fsize,fdir;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	if(!m_DownLoadMap.count(fdir + fname))
	{
		int nRow = m_DownloadList.GetItemCount();
		m_DownloadList.InsertItem(nRow,NULL);
		info->m_nIteam = nRow;
		CMyTools::CalFileSize(info->m_filesize,fsize);
		m_DownloadList.SetItemText(nRow,0,fdir + fname);
		m_DownloadList.SetItemData(nRow,0);
		m_DownloadList.SetItemText(nRow,2,fsize);
		m_DownloadList.SetItemText(nRow,3,_T("�ȴ�����"));
		m_DownLoadMap[fdir + fname] = info;
	}
}

void	CMyTransferDownloadDlg::FinishTask(DownloadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_DownloadList.FindItem(&m_info);
	m_DownloadList.SetItemText(currItem,3,_T("�������"));
	m_DownloadList.DeleteItem(currItem);
	m_DownLoadMap.erase(fdir + fname);
	UpdateData();
}

void	CMyTransferDownloadDlg::PauseTask(DownloadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_DownloadList.FindItem(&m_info);
	info->m_nIteam = currItem;
	m_DownloadList.SetItemText(currItem,3,_T("��ͣ��"));
	UpdateData();
}


void	CMyTransferDownloadDlg::UpdateRate(DownloadFileInfo* info)
{
	CString fname,fdir;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	CString path = (fdir + fname);
	m_info.psz = path;
	int currItem = m_DownloadList.FindItem(&m_info);
	m_DownloadList.SetItemText(currItem,3,_T("��������"));
	double cur = static_cast<double>(info->m_seek);
	double end = static_cast<double>(info->m_filesize);
	double tmp = (cur/end)*100;
	int rate = static_cast<int>(tmp);
	m_DownloadList.SetItemData(currItem,rate);
	UpdateData();
}

void	CMyTransferDownloadDlg::DeleteTask(DownloadFileInfo* info)
{
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(info->m_dir.c_str(),fdir);
	CMyTools::EnCodeUtfToWCHAR(info->m_name.c_str(),fname);
	path = fdir + fname;
	m_info.psz = path;
	int currItem = m_DownloadList.FindItem(&m_info);
	m_DownloadList.DeleteItem(currItem);
	m_DownLoadMap.erase(path);
}

void CMyTransferDownloadDlg::OnNMRClickDownloadList(NMHDR *pNMHDR, LRESULT *pResult)
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

//��ͣ
void CMyTransferDownloadDlg::OnDownloadPause()
{
	// TODO: �ڴ���������������
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_DownloadList.GetCheck(i))
		{
			path = m_DownloadList.GetItemText(i,0);
			if(m_DownLoadMap.count(path)){
				auto info = m_DownLoadMap[path];
				info->m_statuscode = FILE_PAUSE;
			}
		}
	}
}

//����
void CMyTransferDownloadDlg::OnDownloadContinue()
{
	// TODO: �ڴ���������������
	DownloadFileHeadRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = DOWNLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_DownloadList.GetCheck(i))
		{
			path = m_DownloadList.GetItemText(i,0);
			if(m_DownLoadMap.count(path)){
				auto info = m_DownLoadMap[path];
				memcpy_s(rq.m_dir,DIR_SIZE,info->m_curdir.c_str(),info->m_curdir.size());
				memcpy_s(rq.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
				rq.m_filetype = FILE_TYPE;
				rq.m_seek = info->m_seek;
				info->m_statuscode = FILE_TRANSFER;
				theApp.net()->send((char*)&rq,rq.m_len);
				memset(rq.m_dir,0,DIR_SIZE);
				memset(rq.m_name,0,FILENAME_LENGTH);
			}
		}
	}
}

//ɾ��
void CMyTransferDownloadDlg::OnDownloadDelete()
{
	// TODO: �ڴ���������������
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_DownloadList.GetCheck(i))
		{
			path = m_DownloadList.GetItemText(i,0);
			if(m_DownLoadMap.count(path)){
				auto info = m_DownLoadMap[path];
				if(info->m_statuscode == FILE_TRANSFER)
					info->m_statuscode = FILE_DELETE;
				else{
					this->DeleteTask(info);
					((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg.DeleteDownloadTask(info);
				}
			}
		}
	}
}

//��ͣȫ��
void CMyTransferDownloadDlg::OnDownloadPauseAll()
{
	// TODO: �ڴ���������������
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_DownloadList.GetItemText(i,0);
		if(m_DownLoadMap.count(path)){
			auto info = m_DownLoadMap[path];
			info->m_statuscode = FILE_PAUSE;
		}
	}
}

//����ȫ��
void CMyTransferDownloadDlg::OnDownloadContinueAll()
{
	// TODO: �ڴ���������������
	DownloadFileHeadRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = DOWNLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_DownloadList.GetItemText(i,0);
		if(m_DownLoadMap.count(path)){
			auto info = m_DownLoadMap[path];
			memcpy_s(rq.m_dir,DIR_SIZE,info->m_curdir.c_str(),info->m_curdir.size());
			memcpy_s(rq.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
			rq.m_filetype = FILE_TYPE;
			rq.m_seek = info->m_seek;
			info->m_statuscode = FILE_TRANSFER;
			theApp.net()->send((char*)&rq,rq.m_len);
			memset(rq.m_dir,0,DIR_SIZE);
			memset(rq.m_name,0,FILENAME_LENGTH);
		}
		
	}
}

//ɾ��ȫ��
void CMyTransferDownloadDlg::OnDownloadDeleteAll()
{
	// TODO: �ڴ���������������
	CString path;
	int nRow = m_DownloadList.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		path = m_DownloadList.GetItemText(i,0);
		if(m_DownLoadMap.count(path)){
			auto info = m_DownLoadMap[path];
			if(info->m_statuscode == FILE_TRANSFER)
				info->m_statuscode = FILE_DELETE;
			else{
				this->DeleteTask(info);
				((CMyMainDlg*)theApp.m_pMainWnd)->m_MyWangPanDlg.DeleteDownloadTask(info);
			}
		}
		
	}
}


void CMyTransferDownloadDlg::OnNMClickDownloadList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		if(this->m_DownloadList.GetCheck(pNMListView->iItem))
			this->m_DownloadList.SetCheck(pNMListView->iItem,FALSE);
		else
			this->m_DownloadList.SetCheck(pNMListView->iItem,TRUE);
	} 
	*pResult = 0;
}
