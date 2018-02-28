#pragma once

// bgOCXDemoCtrl.h : CbgOCXDemoCtrl ActiveX 控件类的声明。


// CbgOCXDemoCtrl : 有关实现的信息，请参阅 bgOCXDemoCtrl.cpp。

class CbgOCXDemoCtrl : public COleControl
{
	DECLARE_DYNCREATE(CbgOCXDemoCtrl)

// 构造函数
public:
	CbgOCXDemoCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CbgOCXDemoCtrl();

	DECLARE_OLECREATE_EX(CbgOCXDemoCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CbgOCXDemoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CbgOCXDemoCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CbgOCXDemoCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
	};
};

