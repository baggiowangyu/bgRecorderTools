
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
	, sniffer_(new bgSniffer(this))
	, stream_mgr_(new bgStreamManager(this))
	, recoder_(new bgStreamRecorder(this))
	, player_(new bgMediaPlayer(this))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CbgLiveVideoSurveillanceSystemDlg::~CbgLiveVideoSurveillanceSystemDlg()
{
	delete sniffer_;
	sniffer_ = NULL;

	delete stream_mgr_;
	stream_mgr_ = NULL;

	delete recoder_;
	recoder_ = NULL;

	delete player_;
	player_ = NULL;
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
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SNIFFER_URLS, &CbgLiveVideoSurveillanceSystemDlg::OnNMRClickListSnifferUrls)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_RECORDS, &CbgLiveVideoSurveillanceSystemDlg::OnNMRClickListRecords)
	ON_COMMAND(ID_SNIFFER_MENU_DEL, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuDel)
	ON_COMMAND(ID_SNIFFER_MENU_COPY, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuCopy)
	ON_COMMAND(ID_SNIFFER_MENU_PLAY, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuPlay)
	ON_COMMAND(ID_SNIFFER_MENU_RECORD, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuRecord)
	ON_COMMAND(ID_SNIFFER_MENU_DEL_ALL, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuDelAll)
	ON_COMMAND(ID_SNIFFER_MENU_COPY_ALL, &CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuCopyAll)
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
	_stprintf_s(version, 4096, _T("网络直播视频监控平台 V%s"), v.GetBuffer(0));
	SetWindowText(version);

	// 初始化三个列表
	m_cNetworkDevices.SetExtendedStyle(m_cNetworkDevices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cNetworkDevices.InsertColumn(0, _T("网卡名称"), 0, 210);
	m_cNetworkDevices.InsertColumn(1, _T("网卡描述"), 0, 210);
	m_cNetworkDevices.InsertColumn(2, _T("网段"), 0, 100);
	m_cNetworkDevices.InsertColumn(3, _T("网卡路径"), 0, 200);
	m_cNetworkDevices.InsertColumn(4, _T("掩码"), 0, 200);

	m_cSnifferURL.SetExtendedStyle(m_cSnifferURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cSnifferURL.InsertColumn(0, _T("抓取的视频源"), 0, 620);

	m_cSnifferURL.InsertItem(0, _T("rtmp://live.hkstv.hk.lxdns.com/live/hks"));

	m_cRecordURL.SetExtendedStyle(m_cRecordURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cRecordURL.InsertColumn(0, _T("录制文件名"), 0, 250);
	m_cRecordURL.InsertColumn(1, _T("已录制大小"), 0, 100);
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

	// 初始化播放器
	RECT r;
	m_cScreen.GetRect(&r);
	int errCode = player_->Initialize(r.right - r.left, r.bottom - r.top, 1, m_cScreen.GetSafeHwnd());
	if (errCode != 0)
	{
		m_cState.SetWindowText(_T("初始化播放组件失败！"));
		return TRUE;
	}

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
	int errCode = sniffer_->EnumAllNetworkDevices(dev_count, dev_desc);

	dev_desc = new NETWORK_DEVICE_DESC[dev_count];
	errCode = sniffer_->EnumAllNetworkDevices(dev_count, dev_desc);

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

	delete [] dev_desc;
	dev_desc = NULL;
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
	int errCode = sniffer_->OpenNetworkDevice(T2A(dev_path.GetBuffer(0)), _ttoi(mask_ip.GetBuffer(0)));
	if (errCode != 0)
		m_cState.SetWindowText(_T("打开网卡出错！"));
	
}

void CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStopMonitor()
{
	// 点击了停止监控
	sniffer_->CloseNetworkDevice();

	// 重新遍历一遍网络设备
	//EnumNetworkDevices();
}

int CbgLiveVideoSurveillanceSystemDlg::SnifferResultReport(const char *protocol, const char *value)
{
	USES_CONVERSION;

	// 首先检查这个url是否在两个列表中有出现，如果有，则不处理
	int sniffer_item_count = m_cSnifferURL.GetItemCount();
	for (int index = 0; index < sniffer_item_count; ++index)
	{
		CString url = m_cSnifferURL.GetItemText(index, 0);
		if (_stricmp(T2A(url.GetBuffer(0)), value) == 0)
			return -1;
	}

	int record_item_count = m_cRecordURL.GetItemCount();
	for (int index = 0; index < record_item_count; ++index)
	{
		CString url = m_cRecordURL.GetItemText(index, 2);
		if (_stricmp(T2A(url.GetBuffer(0)), value) == 0)
			return -1;
	}

	int item_count = m_cSnifferURL.GetItemCount();

	m_cSnifferURL.InsertItem(item_count, A2T(value));

	return 0;
}
void CbgLiveVideoSurveillanceSystemDlg::OnNMRClickListSnifferUrls(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU_SNIFFER));           //这里是我们在1中定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//下面的两行代码主要是为了后面的操作为准备的
		//获取列表视图控件中第一个被选择项的位置
		POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值
		int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);

	}
}

void CbgLiveVideoSurveillanceSystemDlg::OnNMRClickListRecords(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU_RECORD));           //这里是我们定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//下面的两行代码主要是为了后面的操作为准备的
		//获取列表视图控件中第一个被选择项的位置
		POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
		//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值
		int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);

	}
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuDel()
{
	// 
	POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);
	m_cSnifferURL.DeleteItem(m_nIndex);
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuDelAll()
{
	m_cSnifferURL.DeleteAllItems();
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuCopy()
{
	// 
	POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);
	CString url = m_cSnifferURL.GetItemText(m_nIndex, 0);

	// 写入系统剪贴板
	BOOL bret = OpenClipboard();
	if (!bret)
		m_cState.SetWindowText(_T("复制视频源地址到剪贴板失败！"));

	// 申请剪贴板内存
	HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE, (url.GetLength() + 1) * sizeof(TCHAR));
	char *pBuf = (char *)GlobalLock(hClip);
	memcpy(pBuf, url.GetBuffer(0), url.GetLength() * sizeof(TCHAR));
	GlobalUnlock(hClip);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hClip);
	CloseClipboard();
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuCopyAll()
{
	// TODO: 在此添加命令处理程序代码
	CString all_urls;
	int item_count = m_cSnifferURL.GetItemCount();
	for (int index = 0; index < item_count; ++index)
	{
		CString url = m_cSnifferURL.GetItemText(index, 0);
		all_urls += url;
		all_urls += _T("\r\n");
	}

	// 写入系统剪贴板
	BOOL bret = OpenClipboard();
	if (!bret)
		m_cState.SetWindowText(_T("复制视频源地址到剪贴板失败！"));

	// 申请剪贴板内存
	HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE, (all_urls.GetLength() + 1) * sizeof(TCHAR));
	char *pBuf = (char *)GlobalLock(hClip);
	memcpy(pBuf, all_urls.GetBuffer(0), all_urls.GetLength() * sizeof(TCHAR));
	GlobalUnlock(hClip);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hClip);
	CloseClipboard();
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuPlay()
{
	// 
	POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);
	CString url = m_cSnifferURL.GetItemText(m_nIndex, 0);

	USES_CONVERSION;
	stream_mgr_->HandleURL(T2A(url.GetString()), true, false);
}

void CbgLiveVideoSurveillanceSystemDlg::OnSnifferMenuRecord()
{
	// 
	POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);
	CString url = m_cSnifferURL.GetItemText(m_nIndex, 0);
	m_cSnifferURL.DeleteItem(m_nIndex);

	// 将此url交给录制模块
	USES_CONVERSION;
	stream_mgr_->HandleURL(T2A(url.GetString()), false, true);
}


int CbgLiveVideoSurveillanceSystemDlg::StreamNotifer(enum STREAM_NOTIFY_TYPE msg_type, const char *url)
{
	int errCode = 0;

	switch (msg_type)
	{
	case StreamSave:
		// 需要保存这个流，将流地址从Sniffer列表中移除，加入到录像列表
		// 然后将URL扔给录像模块
		errCode = recoder_->StartRecord(url);
		break;
	case StreamPlay:
		errCode = player_->Play(url);
		break;
	default:
		break;
	}

	return errCode;
}

int CbgLiveVideoSurveillanceSystemDlg::PlayNotifer(const char *url, enum bgMediaPlayerEvent event, const unsigned char *data, int data_len)
{
	int errCode = 0;

	switch (event)
	{
	case 0:
		break;
	default:
		break;
	}

	return errCode;
}

int CbgLiveVideoSurveillanceSystemDlg::RecoderNotifer(const char *url, enum StreamRecordEvent event, unsigned char *info)
{
	int errCode = 0;

	switch (event)
	{
	case StreamRecordUpdateRootpath:
		// 更新根路径
		break;
	case StreamRecordUpdateRecordInfo:
		// 更新录制信息
		break;
	case StreamRecordStart:
		// 开始录制
		break;
	case StreamRecordPause:
		// 暂停录制
		break;
	case StreamRecordStop:
		// 停止录制
		break;
	default:
		break;
	}

	return errCode;
}

int CbgLiveVideoSurveillanceSystemDlg::StreamCodecInfoNotifer(AVCodecContext *video_codec_ctx, AVCodecContext *audio_codec_ctx)
{
	return 0;
}

int CbgLiveVideoSurveillanceSystemDlg::PlayingStreamNotifer(const char *url, AVFrame *frame)
{
	return 0;
}

int CbgLiveVideoSurveillanceSystemDlg::SaveStreamNotifer(const char *url, AVPacket *pkt)
{
	return 0;
}