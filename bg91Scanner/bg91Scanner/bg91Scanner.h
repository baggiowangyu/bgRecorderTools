
// bg91Scanner.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cbg91ScannerApp:
// �йش����ʵ�֣������ bg91Scanner.cpp
//

class Cbg91ScannerApp : public CWinAppEx
{
public:
	Cbg91ScannerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cbg91ScannerApp theApp;