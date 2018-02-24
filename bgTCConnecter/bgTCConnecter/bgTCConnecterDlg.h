
// bgTCConnecterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CbgTCConnecterDlg �Ի���
class CbgTCConnecterDlg : public CDialog
{
// ����
public:
	CbgTCConnecterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BGTCCONNECTER_DIALOG };

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
	CListCtrl m_cFiles;
	CEdit m_cState;
	CProgressCtrl m_cProgress;
	CString m_strSaveFilePath;
	BOOL is_disable_drag;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedBtnStart();

	static DWORD WINAPI WorkingThread(LPVOID lpParam);
};
