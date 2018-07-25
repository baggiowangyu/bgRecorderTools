
// bgLiveBoxDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "bgLiveBoxBusiness.h"
#include "vlc/vlc.h"


// CbgLiveBoxDlg 对话框
class CbgLiveBoxDlg : public CDialog, public bgLiveBoxBusinessObserver
{
// 构造
public:
	CbgLiveBoxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BGLIVEBOX_DIALOG };

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
	CListCtrl m_cApps;
	CListCtrl m_cRooms;
	CComboBox m_cShowTypes;
	CEdit m_cCurrentInfo;

	std::string current_app_id;
	std::string current_app_name;
	HANDLE update_room_event;

public:
	bgLiveBoxBusiness *business_;
	virtual int AppUpdated();
	virtual int RoomUpdate(const char *app_id, const char *app_name);
	virtual void StateNotify(const char *state);

public:
	static DWORD WINAPI AutoRefresh(LPVOID lpParam);
	static DWORD WINAPI RoomRefresh(LPVOID lpParam);
	
public:
	afx_msg void OnNMDblclkListApps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCopyLivingUrl();
	afx_msg void OnRecord();
	afx_msg void OnPlay();
	afx_msg void OnNMRClickListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult);

public:
	libvlc_instance_t		*m_vlcInst;
	libvlc_media_player_t	*m_vlcMplay;
	libvlc_media_t			*m_vlcMedia;
	afx_msg void OnNMDblclkListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStopPlay();
	afx_msg void OnClose();
	afx_msg void OnStnClickedStaticPlayer2();
	CEdit m_cCurrentRoomListInfo;
};
