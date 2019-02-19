// MyWangPanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyWangPanDlg.h"
#include "afxdialogex.h"
#include "packdef.h"
#include "MyTools.h"
#include "TcpMediator.h"
#include "ThreadPool.h"
#include <fstream>
#include "MyNewDirectoryDlg.h"
#include "MyMainDlg.h"
using namespace std;

// CMyWangPanDlg 对话框

IMPLEMENT_DYNAMIC(CMyWangPanDlg, CDialogEx)

CMyWangPanDlg::CMyWangPanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyWangPanDlg::IDD, pParent)
{

}

CMyWangPanDlg::~CMyWangPanDlg()
{
}

void CMyWangPanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PATH_EDIT, m_PathEdit);
	DDX_Control(pDX, IDC_WANGPAN_FILE_LIST, m_FileListControl);
}


BEGIN_MESSAGE_MAP(CMyWangPanDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_WANGPAN_FILE_LIST, &CMyWangPanDlg::OnNMDblclkFileList)
	ON_MESSAGE(FILELIST_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnFileListResponse)
	ON_STN_CLICKED(IDC_REFRESH, &CMyWangPanDlg::OnStnClickedRefresh)
	ON_STN_CLICKED(IDC_LAST_PATH, &CMyWangPanDlg::OnStnClickedLastPath)
	ON_BN_CLICKED(IDC_UPLOAD, &CMyWangPanDlg::OnBnClickedUpload)
	ON_MESSAGE(UPLOAD_FILEHEAD_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnUploadFileHeadResponse)
	ON_BN_CLICKED(IDC_NEW_DIRECTORY, &CMyWangPanDlg::OnBnClickedNewDirectory)
	ON_MESSAGE(ADD_NEW_DIRECTORY_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnAddNewDirectoryResponse)
	ON_BN_CLICKED(IDC_DELETE_SOURCE, &CMyWangPanDlg::OnBnClickedDeleteSource)
	ON_MESSAGE(DELETE_FILE_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnDeleteFileResponse)
	ON_BN_CLICKED(IDC_DOWNLOAD, &CMyWangPanDlg::OnBnClickedDownload)
	ON_MESSAGE(DOWNLOAD_FILEHEAD_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnDownloadFileHeadResponse)
	ON_MESSAGE(DOWNLOAD_FILEDATA_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnDownloadFileDataResponse)
	ON_MESSAGE(DOWNLOAD_FILE_FINISH_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnDownloadFileFinishResponse)
	ON_MESSAGE(UPLOAD_FILEDATA_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnUploadFileDataResponse)
	ON_NOTIFY(NM_CLICK, IDC_WANGPAN_FILE_LIST, &CMyWangPanDlg::OnNMClickWangpanFileList)
	ON_MESSAGE(SHARE_FILE_RESPONSE_DLG_MSG,&CMyWangPanDlg::OnShareFileResponse)
	ON_BN_CLICKED(IDC_SHARE_SOURCE, &CMyWangPanDlg::OnBnClickedShareSource)
END_MESSAGE_MAP()


// CMyWangPanDlg 消息处理程序
BOOL CMyWangPanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//::SetWindowLong(m_hWnd , GWL_EXSTYLE , GetWindowLong(m_hWnd , GWL_EXSTYLE)| WS_EX_LAYERED);
	//::SetLayeredWindowAttributes(m_hWnd , 0 , 128*0.7 , LWA_ALPHA);
	this->SetStaticFont();
	DWORD dwStyle = m_FileListControl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl） 
	dwStyle |= LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES|LVS_EX_SUBITEMIMAGES ;//item前生成checkbox控件
	m_FileListControl.SetExtendedStyle(dwStyle);
	m_ImageList.Create(30,30,ILC_COLOR24,30,30);
	this->AddHeaderImage();
	m_FileListControl.SetImageList(&m_ImageList,LVSIL_SMALL);
	m_FileListControl.InsertColumn(0,_T(""),LVCFMT_LEFT,55);
	m_FileListControl.InsertColumn(1,_T("文件名"),LVCFMT_LEFT,400 );
	//m_FileListControl.InsertColumn(2,_T("文件数量"),LVCFMT_LEFT,60 );
	m_FileListControl.InsertColumn(2,_T("大小"),LVCFMT_LEFT,100 );
	m_FileListControl.InsertColumn(3,_T("类型"),LVCFMT_LEFT,50 );
	
	m_PathEdit.SetWindowText(_T("我的网盘"));
	m_pool = new ThreadPool;
	m_parent = (CMyMainDlg*)this->GetParent();
	//m_pool->CreateThreadPool(2,5,10);
	//this->InsertPicAndWord(1,_T("我的资源"),_T("-"));
	m_downtime = 0;
	m_uptime = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH CMyWangPanDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if(nCtlColor == CTLCOLOR_DLG){
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	HWND h = pWnd->GetSafeHwnd();
	if(nCtlColor ==  CTLCOLOR_STATIC) //静态文本背景透明             
	{              
		pDC->SetBkMode(TRANSPARENT);
		if(GetDlgItem(IDC_LAST_PATH)->GetSafeHwnd() == h ||
			GetDlgItem(IDC_REFRESH)->GetSafeHwnd() == h)
		{
			pDC->SetTextColor(RGB(0,128,64));
		}else{
			pDC->SetTextColor(RGB(255,255,255));
		}
		return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CMyWangPanDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if(m_pool)
	{
		//m_pool->DestroyThreadPool();
		delete m_pool;
		m_pool = NULL;
	}

	UploadFileInfo* upload = NULL;
	fstream fout(theApp.m_home + theApp.m_email + "/set/" + "uploadinfo",ios::out);
	fout<<m_uploadFileMap.size()<<endl;
	auto ite = m_uploadFileMap.begin();
	while(ite != m_uploadFileMap.end())
	{
		upload = ite->second;
		fout<<upload->m_name<<endl;
		fout<<upload->m_dir<<endl;
		fout<<upload->m_curdir<<endl;
		fout<<upload->m_p<<endl;
		fout<<upload->m_md5code<<endl;
		fout<<upload->m_filesize<<endl;
		fout<<upload->m_seek<<endl;
		fout<<upload->m_nIteam<<endl;
		fout<<upload->m_statuscode<<endl;
		delete ite->second;
		ite->second = NULL;
		++ite;
	}
	fout.close();
	m_uploadFileMap.clear();

	DownloadFileInfo* downinfo = NULL;
	fstream	fout2(theApp.m_home + theApp.m_email + "/set/" + "downloadinfo",ios::out);
	fout2<<m_downloadFileMap.size()<<endl;
	auto ite2 = m_downloadFileMap.begin();
	while(ite2 != m_downloadFileMap.end())
	{
		downinfo = ite2->second;
		fout2<<downinfo->m_name<<endl;
		fout2<<downinfo->m_dir<<endl;
		fout2<<downinfo->m_curdir<<endl;
		fout2<<downinfo->m_seek<<endl;
		fout2<<downinfo->m_filesize<<endl;
		fout2<<downinfo->m_nIteam<<endl;
		fout2<<downinfo->m_statuscode<<endl;
		delete ite2->second;
		ite2->second = NULL;
		++ite2;
	}
	fout2.close();
	m_downloadFileMap.clear();
}

void	CMyWangPanDlg::SetStaticFont()
{
	CFont* ftmp1 = GetDlgItem(IDC_LAST_PATH)->GetFont();
	LOGFONT lf;
	ftmp1->GetLogFont(&lf);//获取LOGFONT结构体
	lf.lfHeight=15;    //修改字体大小
	lf.lfItalic=FALSE;        //倾斜
	lf.lfWeight=FW_EXTRABOLD;   //修改字体的粗细
	lstrcpyW(lf.lfFaceName,_T("宋体"));
	fon1.CreateFontIndirectW(&lf);
	GetDlgItem(IDC_LAST_PATH)->SetFont(&fon1);
	GetDlgItem(IDC_REFRESH)->SetFont(&fon1);
}

void	CMyWangPanDlg::AddHeaderImage()
{
	CBitmap*	m_HeaderBmp;
	m_HeaderBmp = new CBitmap;
	m_HeaderBmp->LoadBitmap(IDB_FILE_BMP);
	m_ImageList.Add(m_HeaderBmp,RGB(0,0,0));
	delete m_HeaderBmp;
	m_HeaderBmp = new CBitmap;
	m_HeaderBmp->LoadBitmap(IDB_DIRECTORY_BMP);
	m_ImageList.Add(m_HeaderBmp,RGB(0,0,0));
	delete m_HeaderBmp;
}

void	CMyWangPanDlg::InsertPicAndWord(int PicIndex,const CString& fname,
	const CString& size)
{
	// 添加数据 InsertItem向列表中插入主项数据 SetItemText向列表中的子项写入数据
	int nRow;                                        // 记录行号 
	LVITEM lvItem={0};                               // 列表视图控 LVITEM用于定义"项"的结构
	//第二行数据
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // 文字、图片、状态
	lvItem.iItem = m_FileListControl.GetItemCount();                                // 行号(第三行)
	lvItem.iImage = PicIndex;                               // 图片索引号(第三幅图片 IDB_BITMAP3)
	lvItem.iSubItem = 0;                             // 子列号
	nRow = m_FileListControl.InsertItem(&lvItem);      
	m_FileListControl.SetItemText(nRow,1,fname);   
	//m_FileListControl.SetItemText(nRow,2,fnumber); 
	m_FileListControl.SetItemText(nRow,2,size); 
	if(PicIndex == 0)
		m_FileListControl.SetItemText(nRow,3,_T("文件")); 
	else
		m_FileListControl.SetItemText(nRow,3,_T("文件夹")); 
	UpdateData();
}

//鼠标双击响应
void CMyWangPanDlg::OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	//AfxMessageBox(_T("鼠标双击。。"));
	//鼠标选中一项、若是文件夹就删除当前所有项、并向服务器请求该文件夹下的列表、
	POSITION pos = NULL;
	int nIndex = -1;
	// CListCtrl& lc = GetListCtrl();
	pos = m_FileListControl.GetFirstSelectedItemPosition();
	nIndex = m_FileListControl.GetNextSelectedItem(pos);
	CString ftype;
	ftype = m_FileListControl.GetItemText(nIndex,3);
	if(ftype == _T("文件夹"))
	{
		CString fname = m_FileListControl.GetItemText(nIndex,1);
		//删除当前列表所有项
		m_FileListControl.DeleteAllItems();
		//设置当前文件路径
		string s;
		CMyTools::EnCodeWCHARToUtf(fname.GetBuffer(),s);
		theApp.m_currdir += s + '/';
		string dir = theApp.m_currdir;
		//向服务器请求文件列表
		FileListRq rq;
		memset(&rq,0,sizeof(rq));
		rq.m_len = sizeof(rq);
		rq.m_type = FILELIST_REQUEST_MSG;
		memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
		memcpy_s(rq.m_dir,DIR_SIZE,dir.c_str(),dir.size());
		//设置路径显示
		CString path;
		CMyTools::EnCodeUtfToWCHAR(dir.c_str(),path);
		this->RefreshPath(path);

		theApp.net()->send((char*)&rq,rq.m_len);
	}
}

//收到文件列表
LRESULT CMyWangPanDlg::OnFileListResponse(WPARAM w, LPARAM l)
{
	FileListRs* rs = (FileListRs*)w;
	CString fname;
	CString fsize;
	CString ftype;
	int		index = 0;
	m_FileListControl.DeleteAllItems();
	//只管添加、删除所有项会在鼠标点击时完成（并更新当前目录）
	for(unsigned i = 0; i < rs->m_filecount; i++)
	{
		if(rs->m_filelist[i].m_filesize == 0){
			ftype = _T("文件夹");
			fsize = _T(" - ");
			index = 1;
		}else{
			ftype = _T("文件");
			CMyTools::CalFileSize(rs->m_filelist[i].m_filesize,fsize);
			index = 0;
		}
		CMyTools::EnCodeUtfToWCHAR(rs->m_filelist[i].m_name,fname);
		this->InsertPicAndWord(index,fname,fsize);
	}
	
	return 0;
}

//刷新当前路径
void CMyWangPanDlg::OnStnClickedRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_FileListControl.DeleteAllItems();
	FileListRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = FILELIST_REQUEST_MSG;
	memcpy_s(rq.m_dir,DIR_SIZE,theApp.m_currdir.c_str(),theApp.m_currdir.size());
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	theApp.net()->send((char*)&rq,rq.m_len);
}

//上一级目录
void CMyWangPanDlg::OnStnClickedLastPath()
{
	// TODO: 在此添加控件通知处理程序代码
	if(theApp.m_currdir == "/")
		return;
	string dir(theApp.m_currdir.begin(),theApp.m_currdir.begin() + theApp.m_currdir.size() - 1);
	auto pos = dir.find_last_of('/');
	theApp.m_currdir = dir.substr(0,pos + 1);
	this->OnStnClickedRefresh();
	CString dd;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_currdir.c_str(),dd);
	this->RefreshPath(dd);
}

void	CMyWangPanDlg::RefreshPath(CString path)
{
	this->m_PathEdit.SetWindowText(_T("我的网盘:") + path);
	UpdateData();
}

//upload 上传文件
void CMyWangPanDlg::OnBnClickedUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	CString outfile;
	this->SelectUploadFile(outfile);
	if(outfile.IsEmpty())
		return;
	string email,fname,path,p;
	CMyTools::EnCodeWCHARToUtf(outfile.GetBuffer(),p);
	auto pos = p.find_last_of('\\');
	fname = p.substr(pos + 1);
	email = theApp.m_email;
	path = theApp.m_currdir;
	//向服务器发送请求上传文件头、等待服务器验证
	string md5code = CMyTools::FileDigest(p);
	UploadFileHeadRq rq ;
	memset(&rq,0,sizeof(UploadFileHeadRq));
	rq.m_len = sizeof(UploadFileHeadRq);
	rq.m_type = UPLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,email.c_str(),email.size());
	memcpy_s(rq.m_dir,DIR_SIZE,path.c_str(),path.size());
	memcpy_s(rq.m_name,FILENAME_LENGTH,fname.c_str(),fname.size());
	memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,md5code.c_str(),md5code.size());
	rq.m_filesize = CMyTools::GetFileSize(p);
	
	if(!m_uploadFileMap.count(md5code))
	{//如果已经在上传了、就不能再次发出上传请求
		UploadFileInfo* info = new UploadFileInfo;
		info->m_filesize = rq.m_filesize;
		info->m_name = rq.m_name;
		info->m_dir = rq.m_dir;
		info->m_curdir = rq.m_dir;
		info->m_p = p;
		info->m_md5code = md5code;
		info->m_input.open(info->m_p,ios::binary | ios::in);
		m_uploadFileMap[md5code] = info;
		theApp.net()->send((char*)&rq,rq.m_len);
	}
}

void	CMyWangPanDlg::SelectUploadFile(CString& outfile)
{
	TCHAR BASED_CODE szFilter[] = _T("ALL Type (*.*)|*.*||");
	CFileDialog fileDlg (TRUE, _T("Picture file"), _T(""), OFN_FILEMUSTEXIST, szFilter, this);
	if(fileDlg.DoModal() == IDOK)
	{
		outfile = fileDlg.GetPathName();
	}
}

LRESULT CMyWangPanDlg::OnUploadFileHeadResponse(WPARAM w, LPARAM l)
{
	UploadFileHeadRs* rs = (UploadFileHeadRs*)w;
	if(rs->m_statuscode == UPLOAD_RES_EXIST){
		//秒传成功
		if(m_uploadFileMap.count(rs->m_md5code)){
			delete m_uploadFileMap[rs->m_md5code];
			m_uploadFileMap[rs->m_md5code] = NULL;
			m_uploadFileMap.erase(rs->m_md5code);
		}
	}else if(rs->m_statuscode == UPLOAD_RES_SPACE_LESS){
		//网盘空间不足
		AfxMessageBox(_T("对不起您的存储空间不足，可通过开通VIP扩充容量!"));
		if(m_uploadFileMap.count(rs->m_md5code)){
			delete m_uploadFileMap[rs->m_md5code];
			m_uploadFileMap[rs->m_md5code] = NULL;
			m_uploadFileMap.erase(rs->m_md5code);
		}
	}else if(rs->m_statuscode == UPLOAD_RES_NAMECONFLICT){
		//文件名重复（同一路径下）
		AfxMessageBox(_T("该目录下已包含此文件！"));
	}else if(rs->m_statuscode == UPLOAD_RES_START){
		//可开始传输、发送请求
		UploadFileInfo* info = m_uploadFileMap[rs->m_md5code];
		UploadFileDataRq rq;
		memset(&rq,0,sizeof(rq));
		rq.m_len = sizeof(rq);
		rq.m_type = UPLOAD_FILEREQUEST_MSG;
		memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
		memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,rs->m_md5code,strlen(rs->m_md5code));
		rq.m_seek = info->m_seek;
		info->m_input.seekg(rq.m_seek,ios::beg);
		info->m_input.read(rq.m_filedata,UPLOAD_FILEDATA_SIZE);
		rq.m_datalen = static_cast<int>(info->m_input.gcount());

		m_parent->m_MyTransferDlg.m_uploaddlg.AddUploadTask(info);
		if(m_uptime < theApp.m_uploadNum){
			theApp.net()->send((char*)&rq,rq.m_len);
			++m_uptime;
		}else{
			m_upLst.push_back(rq);
		}
	}
	return 0;
}

LRESULT CMyWangPanDlg::OnUploadFileDataResponse(WPARAM w, LPARAM l)
{
	UploadFileDataRs* rs = (UploadFileDataRs*)w;
	
	UploadFileInfo* info = m_uploadFileMap[rs->m_md5code];
	if(info->m_statuscode == FILE_WAIT)
		info->m_statuscode = FILE_TRANSFER;
	if(info->m_input.eof())
	{//读到文件尾、上传完成、向服务器发送完成消息
		UploadFileEnd end;
		memset(&end,0,sizeof(end));
		end.m_len = sizeof(end);
		end.m_type = UPLOAD_FILEFINISH_MSG;
		end.m_filesize = info->m_filesize;
		memcpy_s(end.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
		memcpy_s(end.m_dir,DIR_SIZE,info->m_curdir.c_str(),info->m_curdir.size());
		memcpy_s(end.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
		memcpy_s(end.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());

		theApp.net()->send((char*)&end,end.m_len);
		m_parent->m_MyTransferDlg.m_uploaddlg.FinishTask(info);
		m_parent->m_MyTransferDlg.m_finishdlg.AddUploadFinishItemToList(info);
		//从map中删除
		delete info;
		info = NULL;
		m_uploadFileMap.erase(end.m_md5code);
		--m_uptime;
		if(m_uptime < 0) m_uptime = 0;
		if(m_upLst.size() > 0)
		{
			auto& uprq = m_upLst.front();
			theApp.net()->send((char*)&uprq,uprq.m_len);
			m_upLst.pop_front();
		}
	}else{
		if(info->m_statuscode == FILE_PAUSE){
			m_parent->m_MyTransferDlg.m_uploaddlg.PauseTask(info);
			--m_uptime;
			if(m_uptime < 0) m_uptime = 0;
			if(m_upLst.size() > 0)
			{
				auto& uprq = m_upLst.front();
				theApp.net()->send((char*)&uprq,uprq.m_len);
				m_upLst.pop_front();
				++m_uptime;
			}
		}else if(info->m_statuscode == FILE_DELETE){
			m_parent->m_MyTransferDlg.m_uploaddlg.DeleteTask(info);
			--m_uptime;
			if(m_uptime < 0) m_uptime = 0;
			if(m_upLst.size() > 0)
			{
				auto& uprq = m_upLst.front();
				theApp.net()->send((char*)&uprq,uprq.m_len);
				m_upLst.pop_front();
				++m_uptime;
			}
			//向服务器发送删除上传文件的消息请求
			UploadDelete up;
			memset(&up,0,sizeof(up));
			up.m_len = sizeof(up);
			up.m_type = UPLOAD_DELETE_REQUEST_MSG;
			memcpy_s(up.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
			memcpy_s(up.m_dir,DIR_SIZE,info->m_curdir.c_str(),info->m_curdir.size());
			memcpy_s(up.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
			memcpy_s(up.m_md5code,FILE_MD5CODE_SIZE,info->m_md5code.c_str(),info->m_md5code.size());
			theApp.net()->send((char*)&up,up.m_len);

			if(m_uploadFileMap.count(info->m_md5code))
			{
				string md5 = info->m_md5code;
				delete info;
				info = NULL;
				m_uploadFileMap.erase(md5);
			}
		}else{
			UploadFileDataRq rq;
			memset(&rq,0,sizeof(rq));
			rq.m_len = sizeof(rq);
			rq.m_type = UPLOAD_FILEREQUEST_MSG;
			memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
			memcpy_s(rq.m_md5code,FILE_MD5CODE_SIZE,rs->m_md5code,strlen(rs->m_md5code));
			rq.m_seek = rs->m_seek;
			info->m_input.read(rq.m_filedata,UPLOAD_FILEDATA_SIZE);
			rq.m_datalen = static_cast<int>(info->m_input.gcount());
			info->m_seek = rq.m_seek;
			m_parent->m_MyTransferDlg.m_uploaddlg.UpdateRate(info);
			theApp.net()->send((char*)&rq,rq.m_len);
		}
	}
	return 0;
}

LRESULT CMyWangPanDlg::OnAddNewDirectoryResponse(WPARAM w, LPARAM l)
{
	AddNewDirectoryRs* rs = (AddNewDirectoryRs*)w;
	if(rs->m_statuscode == ADD_DIRECTORY_RES_OK){
		OnStnClickedRefresh();
		MessageBox(_T("添加文件夹成功"));
	}else if(rs->m_statuscode == ADD_DIRECTORY_RES_EXIST){
		AfxMessageBox(_T("添加文件夹失败：文件夹已存在"));
	}
	return 0;
}

LRESULT CMyWangPanDlg::OnDownloadFileHeadResponse(WPARAM w, LPARAM l)
{
	DownloadFileHeadRs* rs = (DownloadFileHeadRs*)w;
	if(rs->m_statuscode == DOWNLOAD_HEAD_RES_ERROR){
		CString name;
		CMyTools::EnCodeUtfToWCHAR(rs->m_name,name);
		AfxMessageBox(_T("下载文件发生错误:")+ name);
	}else if(rs->m_statuscode == DOWNLOAD_HEAD_RES_OK){
		DownloadFileDataRq rq;
		memset(&rq,0,sizeof(rq));
		rq.m_len = sizeof(rq);
		rq.m_type = DOWNLOAD_FILE_REQUEST_MSG;
		rq.m_seek = rs->m_seek;
		memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
		memcpy_s(rq.m_name,FILENAME_LENGTH,rs->m_name,strlen(rs->m_name));
		memcpy_s(rq.m_dir,DIR_SIZE,rs->m_dir,strlen(rs->m_dir));
		
		string mark = rs->m_dir;
		mark += rs->m_name;
		DownloadFileInfo* info = NULL;
		if(m_downloadFileMap.count(mark))
		{
			info = m_downloadFileMap[mark];
		}else{
			info = new DownloadFileInfo;
			info->m_dir = theApp.m_downloadPath + theApp.m_email + rs->m_dir;
			info->m_curdir = rs->m_dir;
			info->m_name = rs->m_name;
			info->m_filesize = rs->m_filesize;
			//创建文件夹
			CMyTools::createdir(info->m_dir);
			info->m_output.open(info->m_dir + info->m_name,ios::binary | ios::out);
			m_downloadFileMap[mark] = info;
			m_parent->m_MyTransferDlg.m_downloaddlg.AddDownTask(info);
		}
		info->m_statuscode = FILE_WAIT;
		info->m_seek = rs->m_seek;
		info->m_output.seekg(rs->m_seek,ios::beg);
		if(m_downtime < theApp.m_downloadNum){
			theApp.net()->send((char*)&rq,rq.m_len);
			++m_downtime;
		}else{
			m_downLst.push_back(rq);
		}
	}
	return 0;
}

LRESULT CMyWangPanDlg::OnDownloadFileDataResponse(WPARAM w, LPARAM l)
{
	DownloadFileDataRs* rs = (DownloadFileDataRs*)w;
	string s = rs->m_dir;
	s += rs->m_name;
	DownloadFileInfo* info = NULL;
	if(m_downloadFileMap.count(s))
	{
		info = m_downloadFileMap[s];
	}else
	{
		//
	}
	if(info->m_statuscode == FILE_WAIT)
		info->m_statuscode = FILE_TRANSFER;

	info->m_output.seekg(rs->m_seek,ios::beg);
	info->m_output.write(rs->m_filedata,rs->m_datalen);
	info->m_seek += rs->m_datalen;
	m_parent->m_MyTransferDlg.m_downloaddlg.UpdateRate(info);

	
	if(rs->m_datalen > 0)
	{
		if(info->m_statuscode == FILE_PAUSE){
			m_parent->m_MyTransferDlg.m_downloaddlg.PauseTask(info);
			--m_downtime;
			if(m_downtime < 0 ) m_downtime = 0;
			if(m_downLst.size() > 0)
			{
				auto& downrq = m_downLst.front();
				theApp.net()->send((char*)&downrq,downrq.m_len);
				m_downLst.pop_front();
				++m_downtime;
			}
		}else if(info->m_statuscode == FILE_DELETE){
			m_parent->m_MyTransferDlg.m_downloaddlg.DeleteTask(info);
			--m_downtime;
			if(m_downtime < 0 ) m_downtime = 0;
			if(m_downLst.size() > 0)
			{
				auto& downrq = m_downLst.front();
				theApp.net()->send((char*)&downrq,downrq.m_len);
				m_downLst.pop_front();
				++m_downtime;
			}
			DownloadDelete rq;
			memset(&rq,0,sizeof(rq));
			rq.m_len = sizeof(rq);
			rq.m_type = DOWNLOAD_DELETE_REQUEST_MSG;
			memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
			memcpy_s(rq.m_dir,DIR_SIZE,info->m_curdir.c_str(),info->m_curdir.size());
			memcpy_s(rq.m_name,FILENAME_LENGTH,info->m_name.c_str(),info->m_name.size());
			theApp.net()->send((char*)&rq,rq.m_len);

			string ss(info->m_curdir);
			ss += info->m_name;
			if(m_downloadFileMap.count(ss))
			{
				delete info;
				info = NULL;
				m_downloadFileMap.erase(ss);
			}
		}else{
			DownloadFileDataRq rq;
			memset(&rq,0,sizeof(rq));
			rq.m_len = sizeof(rq);
			rq.m_type = DOWNLOAD_FILE_REQUEST_MSG;
			memcpy_s(rq.m_dir,DIR_SIZE,rs->m_dir,strlen(rs->m_dir));
			memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
			memcpy_s(rq.m_name,FILENAME_LENGTH,rs->m_name,strlen(rs->m_name));
			rq.m_seek = info->m_seek;
			theApp.net()->send((char*)&rq,rq.m_len);
		}
	}
	return 0;
}

//下载文件完成
LRESULT CMyWangPanDlg::OnDownloadFileFinishResponse(WPARAM w, LPARAM l)
{
	DownloadFileFinish* finish = (DownloadFileFinish*)w;
	string s = finish->m_dir;
	s += finish->m_name;
	if(m_downloadFileMap.count(s))
	{
		m_parent->m_MyTransferDlg.m_downloaddlg.FinishTask(m_downloadFileMap[s]);
		m_parent->m_MyTransferDlg.m_finishdlg.AddDownloadFinishItemToList(m_downloadFileMap[s]);
		m_downloadFileMap[s]->m_output.close();
		delete m_downloadFileMap[s];
		m_downloadFileMap[s] = NULL;
		m_downloadFileMap.erase(s);
		--m_downtime;
		if(m_downtime < 0 ) m_downtime = 0;
		if(m_downLst.size() > 0)
		{
			auto& downrq = m_downLst.front();
			theApp.net()->send((char*)&downrq,downrq.m_len);
			m_downLst.pop_front();
		}
	}else
	{//发生错误

	}
	return 0;
}

//删除文件回复
LRESULT CMyWangPanDlg::OnDeleteFileResponse(WPARAM w, LPARAM l)
{
	DeleteFileRs* rs = (DeleteFileRs*)w;
	if(rs->m_statuscode == DELETE_FILE_RES_OK){
		this->OnStnClickedRefresh();
	}
	return 0;
}

//新建文件夹
void CMyWangPanDlg::OnBnClickedNewDirectory()
{
	// TODO: 在此添加控件通知处理程序代码
	CMyNewDirectoryDlg d;
	d.DoModal();
}

//删除文件或文件夹
void CMyWangPanDlg::OnBnClickedDeleteSource()
{
	// TODO: 在此添加控件通知处理程序代码
	int		index = -1;
	int		length = m_FileListControl.GetItemCount();
	CString nameCStr,type;
	string name;
	DeleteFileRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = DELETE_FILE_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_dir,DIR_SIZE,theApp.m_currdir.c_str(),theApp.m_currdir.size());
	for(index = 0; index < length; index++)
	{
		if(m_FileListControl.GetCheck(index))
		{
			nameCStr = m_FileListControl.GetItemText(index,1);
			type = m_FileListControl.GetItemText(index,3);
			if(type == _T("文件"))
				rq.m_filetype = FILE_TYPE;
			else if(type == _T("文件夹"))
				rq.m_filetype = DIRECTORY_TYPE;
			CMyTools::EnCodeWCHARToUtf(nameCStr.GetBuffer(),name);
			memcpy_s(rq.m_name,FILENAME_LENGTH,name.c_str(),name.size());
			theApp.net()->send((char*)&rq,rq.m_len);

			memset(rq.m_name,0,FILENAME_LENGTH);
		}
	}
}


BOOL CMyWangPanDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->wParam==VK_ESCAPE || pMsg->wParam == VK_RETURN) 
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

//下载请求
void CMyWangPanDlg::OnBnClickedDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	int		index = -1;
	int		length = m_FileListControl.GetItemCount();
	CString nameCStr,type;
	string	name;
	DownloadFileHeadRq rq;
	memset(&rq,0,sizeof(rq));
	rq.m_len = sizeof(rq);
	rq.m_type = DOWNLOAD_FILEHEAD_REQUEST_MSG;
	memcpy_s(rq.m_email,EMAIL_SIZE,theApp.m_email.c_str(),theApp.m_email.size());
	memcpy_s(rq.m_dir,DIR_SIZE,theApp.m_currdir.c_str(),theApp.m_currdir.size());
	rq.m_seek = 0;
	for(index = 0; index < length; index++)
	{
		if(m_FileListControl.GetCheck(index))
		{//选择下载文件或文件夹
			nameCStr = m_FileListControl.GetItemText(index,1);
			type = m_FileListControl.GetItemText(index,3);
			CMyTools::EnCodeWCHARToUtf(nameCStr.GetBuffer(),name);
			if(type == _T("文件"))
			{//先处理文件类型
				rq.m_filetype = FILE_TYPE;
			}else{
				rq.m_filetype = DIRECTORY_TYPE;
			}
			memcpy_s(rq.m_name,FILENAME_LENGTH,name.c_str(),name.size());
			if(!m_downloadFileMap.count(rq.m_dir + name))//如果已经下载了就不能再发出下载请求
				theApp.net()->send((char*)&rq,rq.m_len);
			memset(rq.m_name,0,FILENAME_LENGTH);
			//Sleep(2000);
		}
	}
}


void CMyWangPanDlg::OnNMClickWangpanFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if(pNMListView->iItem != -1)
	{
		if(this->m_FileListControl.GetCheck(pNMListView->iItem))
			this->m_FileListControl.SetCheck(pNMListView->iItem,FALSE);
		else
			this->m_FileListControl.SetCheck(pNMListView->iItem,TRUE);
	} 
	*pResult = 0;
}

void	CMyWangPanDlg::DeleteDownloadTask(DownloadFileInfo* info)
{
	string path(info->m_curdir + info->m_name);
	string s;
	auto ite = m_downLst.begin();
	while(ite != m_downLst.end())
	{
		s = ite->m_dir;
		s += ite->m_name;
		if(s == path){
			m_downLst.erase(ite);
			break;
		}
		++ite;
	}
	
	if(m_downloadFileMap.count(path)){
		delete info;
		info = NULL;
		m_downloadFileMap.erase(path);
	}
}

void	CMyWangPanDlg::DeleteUploadTask(UploadFileInfo* info)
{
	string md5code = info->m_md5code;
	auto ite = m_upLst.begin();
	while(ite != m_upLst.end())
	{
		if(ite->m_md5code == md5code)
		{
			m_upLst.erase(ite);
			break;
		}
		++ite;
	}
	if(m_uploadFileMap.count(md5code))
	{
		delete m_uploadFileMap[md5code];
		m_uploadFileMap[md5code] = NULL;
		m_uploadFileMap.erase(md5code);
	}
}

void	CMyWangPanDlg::AddDownloadTask(DownloadFileInfo* info)
{
	m_downloadFileMap[info->m_curdir + info->m_name] = info;
	m_parent->m_MyTransferDlg.m_downloaddlg.AddDownTask(info);
	m_parent->m_MyTransferDlg.m_downloaddlg.UpdateRate(info);
	m_parent->m_MyTransferDlg.m_downloaddlg.PauseTask(info);
}

void	CMyWangPanDlg::AddUploadTask(UploadFileInfo* info)
{
	m_uploadFileMap[info->m_md5code] = info;
	m_parent->m_MyTransferDlg.m_uploaddlg.AddUploadTask(info);
	m_parent->m_MyTransferDlg.m_uploaddlg.UpdateRate(info);
	m_parent->m_MyTransferDlg.m_uploaddlg.PauseTask(info);
}

LRESULT CMyWangPanDlg::OnShareFileResponse(WPARAM w, LPARAM l)
{
	ShareFileRs* rs = (ShareFileRs*)w;
	CString path,fname,fdir;
	CMyTools::EnCodeUtfToWCHAR(rs->m_dir,fdir);
	CMyTools::EnCodeUtfToWCHAR(rs->m_name,fname);
	path = fdir + fname;
	if(rs->m_statuscode == SHARE_SUCCESS)
	{//分享成功

	}else if(rs->m_statuscode == SHARE_EXIST)
	{//已分享过了
		AfxMessageBox(_T("文件: ")+path + _T("已经分享过了！"));
	}else{

	}
	return 0;
}

//分享
void CMyWangPanDlg::OnBnClickedShareSource()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_parent->m_MyShareDlg.AddShareFile(_T("123"),_T(""),_T(""),_T(""),_T(""));
	//return;
	CString fname,fdir,path;
	CMyTools::EnCodeUtfToWCHAR(theApp.m_currdir.c_str(),fdir);
	int nRow = m_FileListControl.GetItemCount();
	for(int i = 0; i < nRow; i++)
	{
		if(m_FileListControl.GetCheck(i))
		{
			fname = m_FileListControl.GetItemText(i,1);
			path = fdir + fname;
			m_parent->m_MyShareDlg.SetShareFileDlgFName(path);
			m_parent->m_MyShareDlg.SetShareFileDlgParams(fdir,fname);
			m_parent->m_MyShareDlg.ShowShareFileDlg();
			break;
		}
	}
}
