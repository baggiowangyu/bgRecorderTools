// bgOCXDemoPropPage.cpp : CbgOCXDemoPropPage 属性页类的实现。

#include "stdafx.h"
#include "bgOCXDemo.h"
#include "bgOCXDemoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CbgOCXDemoPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CbgOCXDemoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CbgOCXDemoPropPage, "BGOCXDEMO.bgOCXDemoPropPage.1",
	0xac8f70b6, 0xfc4a, 0x444e, 0xbd, 0x5b, 0xec, 0xcf, 0xd5, 0x7b, 0x34, 0xa4)



// CbgOCXDemoPropPage::CbgOCXDemoPropPageFactory::UpdateRegistry -
// 添加或移除 CbgOCXDemoPropPage 的系统注册表项

BOOL CbgOCXDemoPropPage::CbgOCXDemoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_BGOCXDEMO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CbgOCXDemoPropPage::CbgOCXDemoPropPage - 构造函数

CbgOCXDemoPropPage::CbgOCXDemoPropPage() :
	COlePropertyPage(IDD, IDS_BGOCXDEMO_PPG_CAPTION)
{
}



// CbgOCXDemoPropPage::DoDataExchange - 在页和属性间移动数据

void CbgOCXDemoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CbgOCXDemoPropPage 消息处理程序
