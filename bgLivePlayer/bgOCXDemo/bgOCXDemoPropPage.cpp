// bgOCXDemoPropPage.cpp : CbgOCXDemoPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "bgOCXDemo.h"
#include "bgOCXDemoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CbgOCXDemoPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CbgOCXDemoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CbgOCXDemoPropPage, "BGOCXDEMO.bgOCXDemoPropPage.1",
	0xac8f70b6, 0xfc4a, 0x444e, 0xbd, 0x5b, 0xec, 0xcf, 0xd5, 0x7b, 0x34, 0xa4)



// CbgOCXDemoPropPage::CbgOCXDemoPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CbgOCXDemoPropPage ��ϵͳע�����

BOOL CbgOCXDemoPropPage::CbgOCXDemoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_BGOCXDEMO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CbgOCXDemoPropPage::CbgOCXDemoPropPage - ���캯��

CbgOCXDemoPropPage::CbgOCXDemoPropPage() :
	COlePropertyPage(IDD, IDS_BGOCXDEMO_PPG_CAPTION)
{
}



// CbgOCXDemoPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CbgOCXDemoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CbgOCXDemoPropPage ��Ϣ�������
