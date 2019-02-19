#include "stdafx.h"
#include "IOFileStream.h"


CIOFileStream::CIOFileStream(const string& path,int method)
{
	m_io = new fstream;
	if(method == IO_READ){
		m_io->open(path,ios::in);
	}else if(method == IO_WRITE){
		m_io->open(path,ios::out);
	}else if(method == (IO_READ | IO_WRITE)){
		m_io->open(path,ios::in | ios::out);
	}
	if(m_io->is_open())
		m_isExist = true;
	else
		m_isExist = false;
}


CIOFileStream::~CIOFileStream(void)
{
	delete m_io;
	m_io = NULL;
}

bool	CIOFileStream::IsExist()
{
	return m_isExist;
}

bool	CIOFileStream::eof()
{
	return m_io->eof();
}

bool	CIOFileStream::readline(string& text)
{
	if(eof())
		return false;
	(*m_io)>>text;
	return true;
}

bool	CIOFileStream::writeline(const string& text)
{
	(*m_io)<<text<<endl;
	return true;
}

bool	CIOFileStream::close()
{
	m_io->close();
	return true;
}

char    CIOFileStream::read()
{
	if(eof())
		return 0;
	return m_io->get();
}

bool   CIOFileStream::write(char c)
{
	m_io->put(c);
	return true;
}