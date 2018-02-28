
// bgLiveVideoSurveillanceSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bgLiveVideoSurveillanceSystem.h"
#include "bgLiveVideoSurveillanceSystemDlg.h"

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


// CbgLiveVideoSurveillanceSystemDlg 对话框




CbgLiveVideoSurveillanceSystemDlg::CbgLiveVideoSurveillanceSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbgLiveVideoSurveillanceSystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbgLiveVideoSurveillanceSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SCREEN, m_cScreen);
	DDX_Control(pDX, IDC_EDIT2, m_cState);
	DDX_Control(pDX, IDC_LIST_NETWORK_DEVICES, m_cNetworkDevices);
	DDX_Control(pDX, IDC_LIST_SNIFFER_URLS, m_cSnifferURL);
	DDX_Control(pDX, IDC_LIST_RECORDS, m_cRecordURL);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_cSavePath);
}

BEGIN_MESSAGE_MAP(CbgLiveVideoSurveillanceSystemDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START_MONITOR, &CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStartMonitor)
	ON_BN_CLICKED(IDC_BTN_STOP_MONITOR, &CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStopMonitor)
END_MESSAGE_MAP()


// CbgLiveVideoSurveillanceSystemDlg 消息处理程序

BOOL CbgLiveVideoSurveillanceSystemDlg::OnInitDialog()
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
	CString v = GetMyVersion();
	TCHAR version[4096] = {0};
	_stprintf_s(version, 4096, _T("WLR视频监控系统 %s"), v.GetBuffer(0));
	SetWindowText(version);

	// 初始化三个列表
	m_cNetworkDevices.SetExtendedStyle(m_cNetworkDevices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cNetworkDevices.InsertColumn(0, _T("网卡名称"), 0, 210);
	m_cNetworkDevices.InsertColumn(1, _T("网卡描述"), 0, 210);
	m_cNetworkDevices.InsertColumn(2, _T("网段"), 0, 100);
	m_cNetworkDevices.InsertColumn(3, _T("网卡路径"), 0, 200);
	m_cNetworkDevices.InsertColumn(4, _T("掩码"), 0, 200);

	m_cSnifferURL.SetExtendedStyle(m_cSnifferURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cSnifferURL.InsertColumn(0, _T("视频源"), 0, 620);

	m_cRecordURL.SetExtendedStyle(m_cRecordURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cRecordURL.InsertColumn(0, _T("文件名"), 0, 250);
	m_cRecordURL.InsertColumn(1, _T("录制大小"), 0, 100);
	m_cRecordURL.InsertColumn(2, _T("视频源"), 0, 270);

	// 默认使用程序所在目录作为录像存储根路径
#ifdef UNICODE
	std::wstring cpp;
	std::wstring current_dir;
#else
	std::string cpp;
	std::string current_dir;
#endif

	TCHAR current_prog_path[MAX_PATH] = {0};
	GetModuleFileName(NULL, current_prog_path, MAX_PATH);
	cpp = current_prog_path;
	int pos = cpp.find_last_of(_T("\\"));
	current_dir = cpp.substr(0, pos);
	m_cSavePath.SetWindowText(current_dir.c_str());

	// 枚举当前网卡
	EnumNetworkDevices();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbgLiveVideoSurveillanceSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CbgLiveVideoSurveillanceSystemDlg::OnPaint()
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
HCURSOR CbgLiveVideoSurveillanceSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CbgLiveVideoSurveillanceSystemDlg::GetMyVersion()
{
	TCHAR   szFullPath[MAX_PATH];
	DWORD   dwVerInfoSize = 0;
	DWORD   dwVerHnd;
	VS_FIXEDFILEINFO *pFileInfo;

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));			//这一步很关键，可以自动获取.exe文件路径
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);	// 这一步更关键，直接就是获取到数据
	if (dwVerInfoSize > 0)
	{
		HANDLE     hMem;
		LPVOID     lpvMem;
		unsigned   int   uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);//用来获得指定文件的版本信息：

		if (!::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (LPVOID*)&pFileInfo, &uInfoSize))//这边的话主要是用于从指定的版本信息源获取版本信息
		{
			if (hMem)
			{
				GlobalUnlock(hMem);
				GlobalFree(hMem);
			}

			return  _T("");
		}

		WORD   m_nProdVersion[4];
		m_nProdVersion[0] = HIWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[1] = LOWORD(pFileInfo->dwProductVersionMS);
		m_nProdVersion[2] = HIWORD(pFileInfo->dwProductVersionLS);
		m_nProdVersion[3] = LOWORD(pFileInfo->dwProductVersionLS);

		TCHAR vVersion[256] = { 0 };
		_stprintf_s(vVersion, 256, _T("%d.%d.%d.%d"), m_nProdVersion[0], m_nProdVersion[1], m_nProdVersion[2], m_nProdVersion[3]);
		CString vVerStr = vVersion;

		GlobalUnlock(hMem);
		GlobalFree(hMem);
		return vVerStr;
	}

	return _T("");
}

void CbgLiveVideoSurveillanceSystemDlg::EnumNetworkDevices()
{
	m_cNetworkDevices.DeleteAllItems();

	int dev_count = 0;
	PNETWORK_DEVICE_DESC dev_desc = NULL;
	int errCode = sniffer_.EnumAllNetworkDevices(dev_count, dev_desc);

	dev_desc = new NETWORK_DEVICE_DESC[dev_count];
	errCode = sniffer_.EnumAllNetworkDevices(dev_count, dev_desc);

	PNETWORK_DEVICE_DESC dev = dev_desc;
	for (int index = 0; index < dev_count; ++index)
	{
		int item_count = m_cNetworkDevices.GetItemCount();

		USES_CONVERSION;
		m_cNetworkDevices.InsertItem(item_count, A2T(dev_desc[index].sys_name_.c_str()));
		m_cNetworkDevices.SetItemText(item_count, 1, A2T(dev_desc[index].descript_.c_str()));
		m_cNetworkDevices.SetItemText(item_count, 2, A2T(dev_desc[index].net_ip_.c_str()));
		m_cNetworkDevices.SetItemText(item_count, 3, A2T(dev_desc[index].name_.c_str()));
		TCHAR buffer[32] = {0};
		m_cNetworkDevices.SetItemText(item_count, 4, _itot(dev_desc[index].mask_ip_, buffer, 10));
	}
}


void CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStartMonitor()
{
	// 点击了开始监控，打开网卡，然后启动工作线程
	POSITION pos = m_cNetworkDevices.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return ;

	int selected_item = m_cNetworkDevices.GetNextSelectedItem(pos);
	CString dev_path = m_cNetworkDevices.GetItemText(selected_item, 3);
	CString mask_ip = m_cNetworkDevices.GetItemText(selected_item, 4);

	USES_CONVERSION;
	int errCode = sniffer_.OpenNetworkDevice(T2A(dev_path.GetBuffer(0)), _ttoi(mask_ip.GetBuffer(0)));
	if (errCode != 0)
		m_cState.SetWindowText(_T("打开网卡出错！"));
	
}

void CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStopMonitor()
{
	// 点击了停止监控
	sniffer_.CloseNetworkDevice();

	// 重新遍历一遍网络设备
	//EnumNetworkDevices();
}
