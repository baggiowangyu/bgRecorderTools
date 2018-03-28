
// bgLiveVideoSurveillanceSystem.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "bgLiveVideoSurveillanceSystem.h"
#include "bgLiveVideoSurveillanceSystemDlg.h"
#include "..\bgBase\output.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CbgLiveVideoSurveillanceSystemApp

BEGIN_MESSAGE_MAP(CbgLiveVideoSurveillanceSystemApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CbgLiveVideoSurveillanceSystemApp ����

CbgLiveVideoSurveillanceSystemApp::CbgLiveVideoSurveillanceSystemApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CbgLiveVideoSurveillanceSystemApp ����

CbgLiveVideoSurveillanceSystemApp theApp;


// CbgLiveVideoSurveillanceSystemApp ��ʼ��

BOOL CbgLiveVideoSurveillanceSystemApp::InitInstance()
{
	// ׼������ת����Ϣ
	TCHAR current_dir[MAX_PATH] = {0};
	TCHAR current_exe_path[MAX_PATH] = {0};
	GetModuleFileName(NULL, current_exe_path, MAX_PATH);
#ifdef UNICODE
	std::wstring tmp = current_exe_path;
	int pos = tmp.find_last_of(_T('\\'));
	if (pos < 0)
		return FALSE;

	std::wstring current_dir_stl = tmp.substr(0, pos);
	_tcscpy_s(current_dir, MAX_PATH, current_dir_stl.c_str());
#else
	std::string tmp = current_exe_path;
	int pos = tmp.find_last_of(_T('\\'));
	if (pos < 0)
		return FALSE;

	std::string current_dir_stl = tmp.substr(0, pos);
	_tcscpy_s(current_dir, MAX_PATH, current_dir_stl.c_str());
#endif

	TCHAR dump_path[MAX_PATH] = {0};
	_tcscat_s(dump_path, MAX_PATH, current_dir);
	_tcscat_s(dump_path, MAX_PATH, _T("\\dump"));
	CreateDirectory(dump_path, NULL);

	// ����ʱ�䴴���ļ�
	SYSTEMTIME st;
	GetLocalTime(&st);

	TCHAR dmp_name[MAX_PATH] = {0};
	_stprintf_s(dmp_name, MAX_PATH, _T("Dump-%d%02d%02d-%02d%02d%02d.dmp"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	_tcscat_s(dump_path, MAX_PATH, _T("\\"));
	_tcscat_s(dump_path, MAX_PATH, dmp_name);

	SetupMiniDumpMonitor(dump_path);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("����ֱ����Ƶ���ƽ̨"));

	CbgLiveVideoSurveillanceSystemDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}