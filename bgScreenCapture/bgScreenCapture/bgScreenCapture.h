
// bgScreenCapture.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CbgScreenCaptureApp:
// �йش����ʵ�֣������ bgScreenCapture.cpp
//

class CbgScreenCaptureApp : public CWinAppEx
{
public:
	CbgScreenCaptureApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CbgScreenCaptureApp theApp;