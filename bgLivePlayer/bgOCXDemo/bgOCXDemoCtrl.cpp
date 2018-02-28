// bgOCXDemoCtrl.cpp : CbgOCXDemoCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "bgOCXDemo.h"
#include "bgOCXDemoCtrl.h"
#include "bgOCXDemoPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CbgOCXDemoCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CbgOCXDemoCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CbgOCXDemoCtrl, COleControl)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CbgOCXDemoCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CbgOCXDemoCtrl, 1)
	PROPPAGEID(CbgOCXDemoPropPage::guid)
END_PROPPAGEIDS(CbgOCXDemoCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CbgOCXDemoCtrl, "BGOCXDEMO.bgOCXDemoCtrl.1",
	0x74d9f2, 0xf690, 0x4f1f, 0xbe, 0xd6, 0xd4, 0x42, 0x88, 0x37, 0x41, 0x96)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CbgOCXDemoCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_DbgOCXDemo =
		{ 0x2A4A9DFA, 0xDEE6, 0x438B, { 0xB2, 0xC6, 0xD2, 0xD9, 0x57, 0x43, 0x91, 0xC2 } };
const IID BASED_CODE IID_DbgOCXDemoEvents =
		{ 0xCEE45F56, 0x68A6, 0x40CB, { 0x86, 0x5, 0xB3, 0xFB, 0x43, 0x92, 0x39, 0x33 } };



// 控件类型信息

static const DWORD BASED_CODE _dwbgOCXDemoOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CbgOCXDemoCtrl, IDS_BGOCXDEMO, _dwbgOCXDemoOleMisc)



// CbgOCXDemoCtrl::CbgOCXDemoCtrlFactory::UpdateRegistry -
// 添加或移除 CbgOCXDemoCtrl 的系统注册表项

BOOL CbgOCXDemoCtrl::CbgOCXDemoCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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



// CbgOCXDemoCtrl::CbgOCXDemoCtrl - 构造函数

CbgOCXDemoCtrl::CbgOCXDemoCtrl()
{
	InitializeIIDs(&IID_DbgOCXDemo, &IID_DbgOCXDemoEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CbgOCXDemoCtrl::~CbgOCXDemoCtrl - 析构函数

CbgOCXDemoCtrl::~CbgOCXDemoCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CbgOCXDemoCtrl::OnDraw - 绘图函数

void CbgOCXDemoCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CbgOCXDemoCtrl::DoPropExchange - 持久性支持

void CbgOCXDemoCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CbgOCXDemoCtrl::OnResetState - 将控件重置为默认状态

void CbgOCXDemoCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CbgOCXDemoCtrl 消息处理程序
