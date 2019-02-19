#pragma once
#include <string>
#include <fstream>
using namespace std;
#define IO_READ 1
#define IO_WRITE 2
class CIOFileStream
{
private:
	fstream*		m_io;
	bool			m_isExist;
public:
	CIOFileStream(const string& path,int mothed);
	~CIOFileStream(void);
public:
	bool	eof();
	bool	IsExist();
	bool	readline(string& text);
	bool	writeline(const string& text);
	char    read();
	bool    write(char c);
	bool	close();
};

