#pragma once

// bgOCXDemo.h : bgOCXDemo.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������


// CbgOCXDemoApp : �й�ʵ�ֵ���Ϣ������� bgOCXDemo.cpp��

class CbgOCXDemoApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

