#pragma once
#include <string>
#include <atlstr.h>
using namespace std;
class CMyTools
{
public:
	CMyTools(void);
	~CMyTools(void);
public:
	static void	EnCodeUtfToWCHAR(const char*utf,CString& text);
	static void	EnCodeWCHARToUtf(const WCHAR*uncode,string& text);
	static bool MatchUid(const string& uid);
	static bool MatchPassword(const string& pass);
	static void CalFileSize(long long length,CString& out);
	static string FileDigest(const string &file) ;
	static long long GetFileSize(const string& path);
	static bool	createdir(const string& dir);
	static void NowTime(CString& out);
	static bool MatchDirectoryPath(const string& path);
	static CString SerachFolder(CString DlgTitle=_T("请选择一个文件夹："));
};

