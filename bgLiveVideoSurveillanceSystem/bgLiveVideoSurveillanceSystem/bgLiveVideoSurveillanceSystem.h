
// bgLiveVideoSurveillanceSystem.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CbgLiveVideoSurveillanceSystemApp:
// �йش����ʵ�֣������ bgLiveVideoSurveillanceSystem.cpp
//

class CbgLiveVideoSurveillanceSystemApp : public CWinAppEx
{
public:
	CbgLiveVideoSurveillanceSystemApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CbgLiveVideoSurveillanceSystemApp theApp;