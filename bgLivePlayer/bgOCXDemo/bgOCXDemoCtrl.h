#pragma once

// bgOCXDemoCtrl.h : CbgOCXDemoCtrl ActiveX �ؼ����������


// CbgOCXDemoCtrl : �й�ʵ�ֵ���Ϣ������� bgOCXDemoCtrl.cpp��

class CbgOCXDemoCtrl : public COleControl
{
	DECLARE_DYNCREATE(CbgOCXDemoCtrl)

// ���캯��
public:
	CbgOCXDemoCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CbgOCXDemoCtrl();

	DECLARE_OLECREATE_EX(CbgOCXDemoCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CbgOCXDemoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CbgOCXDemoCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CbgOCXDemoCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
	};
};

