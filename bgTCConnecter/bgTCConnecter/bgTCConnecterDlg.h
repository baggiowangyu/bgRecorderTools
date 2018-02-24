
// bgTCConnecterDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CbgTCConnecterDlg 对话框
class CbgTCConnecterDlg : public CDialog
{
// 构造
public:
	CbgTCConnecterDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BGTCCONNECTER_DIALOG };

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
	CListCtrl m_cFiles;
	CEdit m_cState;
	CProgressCtrl m_cProgress;
	CString m_strSaveFilePath;
	BOOL is_disable_drag;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnStart();

	static DWORD WINAPI WorkingThread(LPVOID lpParam);
};
