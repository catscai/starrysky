/*************************************************************************
	> File Name: IoFileStream.h
	> Author: 
	> Mail: 
	> Created Time: ven. 21 déc. 2018 22:17:23 CST
 ************************************************************************/

#ifndef _IOFILESTREAM_H
#define _IOFILESTREAM_H
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <utility>
#include <sys/io.h>
using namespace std;
#define IOSEEK_BEGIN 0
#define IOSEEK_END 1
class   CIoFileStream
{
private:
    fstream    m_stream;
    bool        m_IsOpenFlag;
public:
    CIoFileStream();
    CIoFileStream(const string& file,int mode);
    ~CIoFileStream();
public:
    bool    open(const string& file,int mode);
    bool    createdir(const string& dir);
    bool    put(const char* buf,unsigned len);
    char    get();
    bool    get(char* buf,unsigned len);
    bool    eof();
    bool    close();
    bool    seek(int off,int num);
    bool    put(char c);
};
#endif
