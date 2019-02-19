#include "stdafx.h"
#include <regex>
#include "MyTools.h"
#include "md5.h"
#include <direct.h>
#include <io.h>
#include <ctime>
using namespace std;
CMyTools::CMyTools(void)
{
}


CMyTools::~CMyTools(void)
{
}

void CMyTools::EnCodeUtfToWCHAR(const char*utf,CString& text)
{
	int nLen = MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, utf, -1, NULL, 0 );
	if (nLen == 0)
	{
	   return ;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, utf, -1, pResult, nLen );
	text = pResult;
	delete[] pResult;
}

void CMyTools::EnCodeWCHARToUtf(const WCHAR*uncode,string& text)
{
	int nLen = WideCharToMultiByte( CP_ACP, 0, uncode, -1, NULL, 0, NULL, NULL );
	if (nLen == 0)
	{
	   return ;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte( CP_ACP, 0, uncode, -1, pResult, nLen, NULL, NULL );
	text = pResult;
	delete[] pResult;
}

bool CMyTools::MatchUid(const string& uid)
{
	regex reg("([\\w\\-_\\.]+)@([\\w]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
	return regex_match(uid,reg);
}

bool CMyTools::MatchPassword(const string& pass)
{
	regex reg2("[\\w]{6,16}");
	return regex_match(pass,reg2);
}

void CMyTools::CalFileSize(long long length,CString& out)
{
	CString unit = _T("byte");
	CString value;
	if(length > 1024 * 1024){
		unit = _T("MB");
		double p = static_cast<double>(length) / (1024*1024);
		value.Format(_T("%.2lf"),p);
	}else if(length > 1024){
		unit = _T("KB");
		double p = static_cast<double>(length) / 1024;
		value.Format(_T("%.2lf"),p);
	}else{
		value.Format(_T("%d"),length);
	}

	out = value + _T(" ") + unit;
}

string CMyTools::FileDigest(const string &file) 
{  
  
    ifstream in(file.c_str(), ios::binary);  
    if (!in)  
        return "";  
  
    MD5 md5;  
    std::streamsize length;  
    char buffer[1024];  
    while (!in.eof()) {  
        in.read(buffer, 1024);  
        length = in.gcount();  
        if (length > 0)  
            md5.update(buffer, static_cast<size_t>(length));  
    }  
    in.close();  
    return md5.toString();  
} 

long long CMyTools::GetFileSize(const string& path)
{
	ifstream input(path,ios::in);
	if(input.is_open()){
		input.seekg(0,ios::end);
		long long len = static_cast<long long>(input.tellg());
		input.close();
		return len;
	}
	return 0;
}

bool	CMyTools::createdir(const string& dir)
{
	string s = dir;
	int ret = -1;
	int pos = -1;
	string front = "";
	while((pos = s.find('/')) != string::npos)
	{
		front += s.substr(0,pos + 1);
		ret = _access(front.c_str(),0);
		if(ret == -1)
		{
			_mkdir(front.c_str());
		}
		if(pos + 1< s.size())
			s = s.substr(pos + 1);
		else
			break;
	}
	return true;
}

void	CMyTools::NowTime(CString& out)
{
	time_t now = time(0);
	tm t;
	localtime_s(&t,&now);
	out.Format(_T("%d-%d-%d %d:%d:%d"),1900 + t.tm_year,t.tm_mon + 1,
		t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
}

bool CMyTools::MatchDirectoryPath(const string& path)
{
	if(path == "/")
		return true;
	regex reg("(/[\\w]+)+/");
	return regex_match(path,reg);
}

CString CMyTools::SerachFolder(CString DlgTitle)
{
		TCHAR			szFolderPath[MAX_PATH] = {0};
		CString			strFolderPath = _T("");
		
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot   = 0;
		sInfo.lpszTitle   = DlgTitle;
		sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn     = NULL;
 
		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo); 
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
			if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))  
			{
				strFolderPath = szFolderPath;
			}
		}
		if(lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}
 
		return strFolderPath;

}