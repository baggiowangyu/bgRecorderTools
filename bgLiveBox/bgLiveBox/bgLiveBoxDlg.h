
// bgLiveBoxDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "bgLiveBoxBusiness.h"


// CbgLiveBoxDlg �Ի���
class CbgLiveBoxDlg : public CDialog, public bgLiveBoxBusinessObserver
{
// ����
public:
	CbgLiveBoxDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BGLIVEBOX_DIALOG };

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
};
