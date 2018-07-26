#include "stdafx.h"
#include "Monitor.h"
#include "MakePNG.h"

#include <Windows.h>

Monitor::Monitor()
{

}

Monitor::~Monitor()
{

}

int Monitor::StartScreenCaptureMonitor()
{
	CreateThread(NULL, 0, ScreenCaptureMonitor, this, 0, NULL);
	Sleep(1);

	return 0;
}

DWORD WINAPI Monitor::ScreenCaptureMonitor(LPVOID lpParam)
{
	Monitor *monitor = (Monitor *)lpParam;

	WSADATA wsaData;  
	WSAStartup(MAKEWORD(2,2),&wsaData);  

	while (true)
	{
		// ȡ�ü�����Ļ�����Ϣ��
		// ���������IP��ַ����ǰʱ��
		char computer_name[4096] = {0};
		DWORD computer_name_len = 4096;
		GetComputerNameA(computer_name, &computer_name_len);

		//��ȡ����IP
		char hostname[256] = {0};
		gethostname(hostname,sizeof(hostname));

		HOSTENT* host=gethostbyname(hostname);
		char ip[64] = {0};
		strcpy(ip, inet_ntoa(*(in_addr*)*host->h_addr_list));

		// ��ǰʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);
		char time_stamp[4096] = {0};
		sprintf_s(time_stamp, 4096, "%d-%02d-%02d_%02d-%02d-%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		// ������ʱ�ļ�·�������ڴ洢����
		char tmpdir[4096] = {0};
		GetTempPathA(4096, tmpdir);

		char tmpfile[4096] = {0};
		GetTempFileNameA(tmpdir, "blb", 0, tmpfile);

		HWND hWnd = ::GetDesktopWindow();//�����Ļ��HWND.
		HDC hScreenDC = ::GetDC(hWnd);   //�����Ļ��HDC.
		HDC MemDC = ::CreateCompatibleDC(hScreenDC);
		RECT rect; 
		::GetWindowRect(hWnd,&rect);

		USES_CONVERSION;
		CMakePNG MakePNG;
		MakePNG.MakePNG(hScreenDC, rect, A2T(tmpfile));
		ReleaseDC(hWnd, MemDC);

		// ���ʼ�

		DeleteFileA(tmpfile);

		Sleep(15 * 60 * 1000);
	}

	return 0;
}