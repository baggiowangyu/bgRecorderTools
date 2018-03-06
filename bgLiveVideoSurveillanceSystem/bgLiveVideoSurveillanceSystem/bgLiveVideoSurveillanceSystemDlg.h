
// bgLiveVideoSurveillanceSystemDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "bgNetworkProtocolStruct.h"
#include "bgSniffer.h"
#include "..\bgStreamManager\bgStreamManager.h"
#include "..\bgStreamHandler\bgStreamHandler.h"
#include "..\bgStreamRecorder\bgStreamRecorder.h"
#include "..\bgMediaPlayer\bgMediaPlayer.h"

// CbgLiveVideoSurveillanceSystemDlg 对话框
class CbgLiveVideoSurveillanceSystemDlg : public CDialog, bgSnifferNotifer, bgStreamNotifer, bgStreamRecorderNotifer, bgMediaPlayerNotifer, bgStreamHandlerNotifer
{
// 构造
public:
	CbgLiveVideoSurveillanceSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CbgLiveVideoSurveillanceSystemDlg();

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
	bgSniffer *sniffer_;
	virtual int SnifferResultReport(const char *protocol, const char *value);

public:
	bgStreamManager *stream_mgr_;
	virtual int StreamNotifer(enum STREAM_NOTIFY_TYPE msg_type, const char *url);

public:
	bgStreamRecorder *recoder_;
	virtual int RecoderNotifer(const char *url, enum StreamRecordEvent event, unsigned char *info);

public:
	bgMediaPlayer *player_;
	virtual int PlayNotifer(const char *url, enum bgMediaPlayerEvent event, const unsigned char *data, int data_len);

public:
	virtual int StreamCodecInfoNotifer(AVCodecContext *video_codec_ctx, AVCodecContext *audio_codec_ctx);
	virtual int PlayingStreamNotifer(const char *url, AVFrame *frame);
	virtual int SaveStreamNotifer(const char *url, AVPacket *pkt);

public:
	afx_msg void OnBnClickedBtnStartMonitor();
	afx_msg void OnBnClickedBtnStopMonitor();
	CEdit m_cSavePath;

public:

	afx_msg void OnNMRClickListSnifferUrls(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListRecords(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSnifferMenuDel();
	afx_msg void OnSnifferMenuCopy();
	afx_msg void OnSnifferMenuPlay();
	afx_msg void OnSnifferMenuRecord();
	afx_msg void OnSnifferMenuDelAll();
	afx_msg void OnSnifferMenuCopyAll();
};
