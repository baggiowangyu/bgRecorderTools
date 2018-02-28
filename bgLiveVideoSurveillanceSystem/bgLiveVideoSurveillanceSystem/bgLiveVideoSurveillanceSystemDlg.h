
// bgLiveVideoSurveillanceSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "bgSniffer.h"


// CbgLiveVideoSurveillanceSystemDlg �Ի���
class CbgLiveVideoSurveillanceSystemDlg : public CDialog
{
// ����
public:
	CbgLiveVideoSurveillanceSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BGLIVEVIDEOSURVEILLANCESYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// ��ȡ����汾��
	CString GetMyVersion();

	// ö�������豸
	void EnumNetworkDevices();
	// ���ذ��������
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
