
// bgTCConnecterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bgTCConnecter.h"
#include "bgTCConnecterDlg.h"

#include <vector>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CbgTCConnecterDlg 对话框




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


// CbgTCConnecterDlg 消息处理程序

BOOL CbgTCConnecterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cFiles.SetExtendedStyle(m_cFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cFiles.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 1000);

	m_cState.SetWindowText(_T("准备就绪..."));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbgTCConnecterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbgTCConnecterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CbgTCConnecterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// 检查文件列表中是否有文件
	int items = m_cFiles.GetItemCount();
	if (items == 0)
	{
		return;
	}

	CFileDialog file_dlg(FALSE, _T("ts"), _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, this);

	INT_PTR ret = file_dlg.DoModal();
	if (ret == IDOK)
	{
		// 保存文件路径，开启工作线程，将此按钮禁用，直到线程退出
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

	// 首先，手机所有的ts文件的大小信息，得到总大小
	std::vector<std::wstring> ts_files;
	int items = dlg->m_cFiles.GetItemCount();

	__int64 total_file_size = 0;

	for (int index = 0; index < items; ++index)
	{
		CString path = dlg->m_cFiles.GetItemText(index, 0);
		
		// 打开文件，获取文件大小
		HANDLE hFile = CreateFile(path, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			continue;
		
		LARGE_INTEGER file_size;
		GetFileSizeEx(hFile, &file_size);

		total_file_size += file_size.QuadPart;

		CloseHandle(hFile);
	}

	// 创建输出文件
	HANDLE hOutputFile = CreateFile(dlg->m_strSaveFilePath, GENERIC_ALL, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hOutputFile == INVALID_HANDLE_VALUE)
	{
		dlg->MessageBox(_T("创建输出文件失败！"), _T("错误"), MB_OK|MB_ICONERROR);
		pCwnd->EnableWindow(TRUE);
		dlg->is_disable_drag = FALSE;
		return 0;
	}

	// 然后开始逐个文件读取，缓冲区我们默认为4K吧
	__int64 already_saved = 0;
	for (int index = 0; index < items; ++index)
	{
		CString path = dlg->m_cFiles.GetItemText(index, 0);

		// 打开文件，获取文件大小
		HANDLE hFile = CreateFile(path, GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			continue;

		_stprintf_s(msg, 4096, _T("正在合并文件：%s"), path.GetString());
		dlg->m_cState.SetWindowText(msg);

		unsigned char buffer[4096] = {0};
		DWORD readed = 0, written = 0;

		while (true)
		{
			BOOL b = ReadFile(hFile, buffer, 4096, &readed, NULL);
			if (!b)
			{
				// 出现错误，停止
				dlg->MessageBox(_T("读取输入文件失败！"), _T("错误"), MB_OK|MB_ICONERROR);

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
				// 出现错误，停止
				dlg->MessageBox(_T("写入输出文件失败！"), _T("错误"), MB_OK|MB_ICONERROR);

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

	dlg->m_cState.SetWindowText(_T("连接完成！"));
	dlg->m_cFiles.DeleteAllItems();

	CloseHandle(hOutputFile);
	pCwnd->EnableWindow(TRUE);
	dlg->is_disable_drag = FALSE;
	return 0;
}