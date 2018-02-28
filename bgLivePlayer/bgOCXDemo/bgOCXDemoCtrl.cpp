// bgOCXDemoCtrl.cpp : CbgOCXDemoCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "bgOCXDemo.h"
#include "bgOCXDemoCtrl.h"
#include "bgOCXDemoPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CbgOCXDemoCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CbgOCXDemoCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CbgOCXDemoCtrl, COleControl)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CbgOCXDemoCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CbgOCXDemoCtrl, 1)
	PROPPAGEID(CbgOCXDemoPropPage::guid)
END_PROPPAGEIDS(CbgOCXDemoCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CbgOCXDemoCtrl, "BGOCXDEMO.bgOCXDemoCtrl.1",
	0x74d9f2, 0xf690, 0x4f1f, 0xbe, 0xd6, 0xd4, 0x42, 0x88, 0x37, 0x41, 0x96)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CbgOCXDemoCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID BASED_CODE IID_DbgOCXDemo =
		{ 0x2A4A9DFA, 0xDEE6, 0x438B, { 0xB2, 0xC6, 0xD2, 0xD9, 0x57, 0x43, 0x91, 0xC2 } };
const IID BASED_CODE IID_DbgOCXDemoEvents =
		{ 0xCEE45F56, 0x68A6, 0x40CB, { 0x86, 0x5, 0xB3, 0xFB, 0x43, 0x92, 0x39, 0x33 } };



// �ؼ�������Ϣ

static const DWORD BASED_CODE _dwbgOCXDemoOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CbgOCXDemoCtrl, IDS_BGOCXDEMO, _dwbgOCXDemoOleMisc)



// CbgOCXDemoCtrl::CbgOCXDemoCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CbgOCXDemoCtrl ��ϵͳע�����

BOOL CbgOCXDemoCtrl::CbgOCXDemoCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_BGOCXDEMO,
			IDB_BGOCXDEMO,
			afxRegApartmentThreading,
			_dwbgOCXDemoOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CbgOCXDemoCtrl::CbgOCXDemoCtrl - ���캯��

CbgOCXDemoCtrl::CbgOCXDemoCtrl()
{
	InitializeIIDs(&IID_DbgOCXDemo, &IID_DbgOCXDemoEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CbgOCXDemoCtrl::~CbgOCXDemoCtrl - ��������

CbgOCXDemoCtrl::~CbgOCXDemoCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CbgOCXDemoCtrl::OnDraw - ��ͼ����

void CbgOCXDemoCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CbgOCXDemoCtrl::DoPropExchange - �־���֧��

void CbgOCXDemoCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CbgOCXDemoCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CbgOCXDemoCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CbgOCXDemoCtrl ��Ϣ�������
