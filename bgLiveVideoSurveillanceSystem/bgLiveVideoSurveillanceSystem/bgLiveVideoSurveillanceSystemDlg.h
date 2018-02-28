
// bgLiveVideoSurveillanceSystemDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "bgSniffer.h"


// CbgLiveVideoSurveillanceSystemDlg 对话框
class CbgLiveVideoSurveillanceSystemDlg : public CDialog
{
// 构造
public:
	CbgLiveVideoSurveillanceSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BGLIVEVIDEOSURVEILLANCESYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// 获取程序版本号
	CString GetMyVersion();

	// 枚举网卡设备
	void EnumNetworkDevices();
	// 加载包解析插件
	void LoadPacketParsePlugins();

public:
	CEdit m_cScreen;
	CEdit m_cState;
	CListCtrl m_cNetworkDevices;
	CListCtrl m_cSnifferURL;
	CListCtrl m_cRecordURL;

public:
	bgSniffer sniffer_;

public:
	afx_msg void OnBnClickedBtnStartMonitor();
	afx_msg void OnBnClickedBtnStopMonitor();
	CEdit m_cSavePath;
};
