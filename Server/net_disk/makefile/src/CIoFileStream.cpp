/*************************************************************************
	> File Name: CIoFileStream.cpp
	> Author: 
	> Mail: 
	> Created Time: ven. 21 déc. 2018 22:28:27 CST
 ************************************************************************/

#include "CIoFileStream.h"
#include <iostream>
using namespace std;
CIoFileStream::CIoFileStream()
{

}

CIoFileStream::CIoFileStream(const string& file,int mode)
{
    this->open(file,mode);
}

CIoFileStream::~CIoFileStream()
{
    this->close();    
}

bool    CIoFileStream::open(const string& file,int mode)
{
    if(mode == 0)
        m_stream.open(file,ios::binary|ios::out);
    else 
        m_stream.open(file,ios::binary|ios::in);
    if(m_stream.is_open()){
        m_IsOpenFlag = true;
    }
    else{
        m_IsOpenFlag = false;
    }
    return true;
}

bool    CIoFileStream::createdir(const string& dir)
{
    int pos = 0;
    string tmp = dir;
    string tmp2;
    string front;
    int     ret = 0;
    if(tmp[0] == '.'){
        tmp = tmp.substr(1);
        front = '.';
    }
    pos = tmp.find_first_not_of('/');
    front += tmp.substr(0,pos);
    tmp = tmp.substr(pos);
    while((pos = tmp.find('/')) != string::npos)
    {
        tmp2 = tmp.substr(0,pos + 1);
        front += tmp2;
        ret = access(front.c_str(),0);
        if(ret == -1)
        {
            mkdir(front.c_str(),0664);            
        }
        if(pos + 1 < tmp.size())
            tmp = tmp.substr(pos + 1);
        else 
            break;
    }
    return true;
}

bool    CIoFileStream::put(char c)
{
    if(m_IsOpenFlag)
    {
        m_stream.put(c);
    }else
        return false;
    return true;
}

bool    CIoFileStream::put(const char* buf,unsigned len)
{
    if(m_IsOpenFlag)
    {
        m_stream.write(buf,len);
    }else
        return false;
    return true;
}

char    CIoFileStream::get()
{
    if(!this->eof())
    {
        return m_stream.get();
    }
    return 0;
}

bool    CIoFileStream::get(char* buf,unsigned len)
{
    if(m_IsOpenFlag)
    {
        m_stream.read(buf,len);
        return true;
    }
    return false;
}

bool    CIoFileStream::eof()
{
    if(m_IsOpenFlag)
    {
        return m_stream.eof();
    }
    return false;
}

bool    CIoFileStream::close()
{
    if(m_IsOpenFlag)
    {
        m_stream.close();
    }
    return true;
}


bool    CIoFileStream::seek(int off,int num)
{
    if(m_IsOpenFlag)
    {
        if(IOSEEK_BEGIN == off)
            m_stream.seekg(num,ios::beg);
        else if(IOSEEK_END == off)
            m_stream.seekg(num,ios::end);
        return true;
    }
    return false;
}
