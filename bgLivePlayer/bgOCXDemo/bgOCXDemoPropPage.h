#pragma once

// bgOCXDemoPropPage.h : CbgOCXDemoPropPage ����ҳ���������


// CbgOCXDemoPropPage : �й�ʵ�ֵ���Ϣ������� bgOCXDemoPropPage.cpp��

class CbgOCXDemoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CbgOCXDemoPropPage)
	DECLARE_OLECREATE_EX(CbgOCXDemoPropPage)

// ���캯��
public:
	CbgOCXDemoPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_BGOCXDEMO };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

