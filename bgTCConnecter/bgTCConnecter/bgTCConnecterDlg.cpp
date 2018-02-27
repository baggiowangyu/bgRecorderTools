
// bgTCConnecterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "bgTCConnecter.h"
#include "bgTCConnecterDlg.h"

#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CbgTCConnecterDlg �Ի���




CbgTCConnecterDlg::CbgTCConnecterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbgTCConnecterDlg::IDD, pParent)
	, is_disable_drag(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbgTCConnecterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_cFiles);
	DDX_Control(pDX, IDC_EDIT_STATE, m_cState);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgress);
}

BEGIN_MESSAGE_MAP(CbgTCConnecterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_START, &CbgTCConnecterDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CbgTCConnecterDlg ��Ϣ�������

BOOL CbgTCConnecterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_cFiles.SetExtendedStyle(m_cFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cFiles.InsertColumn(0, _T("�ļ���"), LVCFMT_LEFT, 1000);

	m_cState.SetWindowText(_T("׼������..."));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CbgTCConnecterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CbgTCConnecterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CbgTCConnecterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbgTCConnecterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (!is_disable_drag)
	{
		TCHAR filePath[MAX_PATH] = {0};
		UINT count = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
		if(count)
		{        
			for(UINT i = 0; i < count; ++i)
			{
				int pathLen = DragQueryFile(hDropInfo, i, filePath, MAX_PATH);
				int items = m_cFiles.GetItemCount();
				m_cFiles.InsertItem(items, filePath);
			}
		}

		DragFinish(hDropInfo);
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CbgTCConnecterDlg::OnBnClickedBtnStart()
{
	// ����ļ��б����Ƿ����ļ�
	int items = m_cFiles.GetItemCount();
	if (items == 0)
	{
		return;
	}

	CFileDialog file_dlg(FALSE, _T("ts"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	INT_PTR ret = file_dlg.DoModal();
	if (ret == IDOK)
	{
		// �����ļ�·�������������̣߳����˰�ť���ã�ֱ���߳��˳�
		m_strSaveFilePath = file_dlg.GetPathName();

		HANDLE hThread = CreateThread(NULL, 0, WorkingThread, this, 0, NULL);
		if (hThread != NULL)
		{
			CWnd *pCwnd = GetDlgItem(IDC_BTN_START);
			pCwnd->EnableWindow(FALSE);

			is_disable_drag = TRUE;
		}
	}
}

DWORD WINAPI CbgTCConnecterDlg::WorkingThread(LPVOID lpParam)
{
	TCHAR msg[4096] = {0};
	CbgTCConnecterDlg *dlg = (CbgTCConnecterDlg *)lpParam;
	CWnd *pCwnd = dlg->GetDlgItem(IDC_BTN_START);

	// ���ȣ��ֻ����е�ts�ļ��Ĵ�С��Ϣ���õ��ܴ�С
	std::vector<std::wstring> ts_files;
	int items = dlg->m_cFiles.GetItemCount();

	__int64 total_file_size = 0;

	for (int index = 0; index < items; ++index)
	{
		CString path = dlg->m_cFiles.GetItemText(index, 0);
		
		// ���ļ�����ȡ�ļ���С
		HANDLE hFile = CreateFile(path, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			continue;
		
		LARGE_INTEGER file_size;
		GetFileSizeEx(hFile, &file_size);

		total_file_size += file_size.QuadPart;

		CloseHandle(hFile);
	}

	// ��������ļ�
	HANDLE hOutputFile = CreateFile(dlg->m_strSaveFilePath, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOutputFile == INVALID_HANDLE_VALUE)
	{
		dlg->MessageBox(_T("��������ļ�ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
		pCwnd->EnableWindow(TRUE);
		dlg->is_disable_drag = FALSE;
		return 0;
	}

	// Ȼ��ʼ����ļ���ȡ������������Ĭ��Ϊ4K��
	__int64 already_saved = 0;
	for (int index = 0; index < items; ++index)
	{
		CString path = dlg->m_cFiles.GetItemText(index, 0);

		// ���ļ�����ȡ�ļ���С
		HANDLE hFile = CreateFile(path, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			continue;

		_stprintf_s(msg, 4096, _T("���ںϲ��ļ���%s"), path.GetString());
		dlg->m_cState.SetWindowText(msg);

		unsigned char buffer[4096] = {0};
		DWORD readed = 0, written = 0;

		while (true)
		{
			BOOL b = ReadFile(hFile, buffer, 4096, &readed, NULL);
			if (!b)
			{
				// ���ִ���ֹͣ
				dlg->MessageBox(_T("��ȡ�����ļ�ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);

				CloseHandle(hFile);
				CloseHandle(hOutputFile);
				pCwnd->EnableWindow(TRUE);
				dlg->is_disable_drag = FALSE;
				return 0;
			}

			if (readed == 0)
				break;

			b = WriteFile(hOutputFile, buffer, readed, &written, NULL);
			if (!b)
			{
				// ���ִ���ֹͣ
				dlg->MessageBox(_T("д������ļ�ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);

				CloseHandle(hFile);
				CloseHandle(hOutputFile);
				pCwnd->EnableWindow(TRUE);
				dlg->is_disable_drag = FALSE;
				return 0;
			}

			already_saved += written;
			int pos = ((double)already_saved / (double)total_file_size) * 100;
			dlg->m_cProgress.SetPos(pos);
		}

		CloseHandle(hFile);
	}

	dlg->m_cState.SetWindowText(_T("������ɣ�"));
	dlg->m_cFiles.DeleteAllItems();

	CloseHandle(hOutputFile);
	pCwnd->EnableWindow(TRUE);
	dlg->is_disable_drag = FALSE;
	return 0;
}