#pragma once

// bgOCXDemoPropPage.h : CbgOCXDemoPropPage 属性页类的声明。


// CbgOCXDemoPropPage : 有关实现的信息，请参阅 bgOCXDemoPropPage.cpp。

class CbgOCXDemoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CbgOCXDemoPropPage)
	DECLARE_OLECREATE_EX(CbgOCXDemoPropPage)

// 构造函数
public:
	CbgOCXDemoPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_BGOCXDEMO };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

