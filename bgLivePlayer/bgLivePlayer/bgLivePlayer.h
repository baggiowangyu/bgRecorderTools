
// bgLivePlayer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CbgLivePlayerApp:
// �йش����ʵ�֣������ bgLivePlayer.cpp
//

class CbgLivePlayerApp : public CWinAppEx
{
public:
	CbgLivePlayerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CbgLivePlayerApp theApp;