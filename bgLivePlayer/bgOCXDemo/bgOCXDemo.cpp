// bgOCXDemo.cpp : CbgOCXDemoApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "bgOCXDemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CbgOCXDemoApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x854CA030, 0xC862, 0x456F, { 0xBC, 0xD7, 0x37, 0xE1, 0x45, 0x74, 0xFB, 0x5E } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CbgOCXDemoApp::InitInstance - DLL 初始化

BOOL CbgOCXDemoApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CbgOCXDemoApp::ExitInstance - DLL 终止

int CbgOCXDemoApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
