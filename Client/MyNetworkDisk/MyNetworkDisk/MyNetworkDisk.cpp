
// MyNetworkDisk.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MyNetworkDisk.h"
#include "MyNetworkDiskDlg.h"
#include "TcpMediator.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyNetworkDiskApp

BEGIN_MESSAGE_MAP(CMyNetworkDiskApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMyNetworkDiskApp ����

CMyNetworkDiskApp::CMyNetworkDiskApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴����ӹ�����룬
	m_mediator = NULL;
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMyNetworkDiskApp ����

CMyNetworkDiskApp theApp;


// CMyNetworkDiskApp ��ʼ��

BOOL CMyNetworkDiskApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	m_mediator = new TcpMediator();
	
	if(false == m_mediator->connect())
	{
		AfxMessageBox(_T("���ӷ�����ʧ��"));
	}
	
	//Starry Sky
	if(!PathIsDirectory(_T("D:/StarrySky"))){
		CreateDirectory(_T("D:/StarrySky"),NULL);
	}
	m_home = "D:/StarrySky/";
	m_currdir = "/";
	fstream fin(m_home + "setconfig",ios::in);
	if(fin.is_open())
	{
		fin>>m_downloadPath;
		fin>>m_downloadNum;
		fin>>m_uploadNum;
		fin.close();
		if(m_downloadNum > 3 && m_downloadNum < 1)
			m_downloadNum = 1;
		if(m_uploadNum > 3 && m_uploadNum < 1)
			m_uploadNum = 1;
	}else{
		m_downloadPath = m_home;
		m_downloadNum = 1;
		m_uploadNum = 1;
	}
	CMyNetworkDiskDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


int CMyNetworkDiskApp::ExitInstance()
{
	// TODO: �ڴ�����ר�ô����/����û���
	if(m_mediator)
	{
		m_mediator->close();
		delete m_mediator;
		m_mediator = NULL;
	}

	fstream fout(m_home + "setconfig",ios::out);
	fout<<m_downloadPath<<endl;
	fout<<m_downloadNum<<endl;
	fout<<m_uploadNum<<endl;
	fout.close();
	return CWinApp::ExitInstance();
}