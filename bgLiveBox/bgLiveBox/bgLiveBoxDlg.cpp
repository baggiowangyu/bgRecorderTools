
// bgLiveBoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bgLiveBox.h"
#include "bgLiveBoxDlg.h"

#include <ctime>

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


// CbgLiveBoxDlg 对话框




CbgLiveBoxDlg::CbgLiveBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbgLiveBoxDlg::IDD, pParent)
	, business_(new bgLiveBoxBusiness(this))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbgLiveBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_APPS, m_cApps);
	DDX_Control(pDX, IDC_LIST_LIVING_ROOM, m_cRooms);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cShowTypes);
	DDX_Control(pDX, IDC_EDIT_CURRENT_INFO, m_cCurrentInfo);
	DDX_Control(pDX, IDC_EDIT_CURRENT_ROOM_LIST_INFO2, m_cCurrentRoomListInfo);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_cSearch);
	DDX_Control(pDX, IDC_LIST_24HOURS_KEEP, m_cKeepRecordList);
}

BEGIN_MESSAGE_MAP(CbgLiveBoxDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_APPS, &CbgLiveBoxDlg::OnNMDblclkListApps)
	ON_COMMAND(ID_COPY_LIVING_URL, &CbgLiveBoxDlg::OnCopyLivingUrl)
	ON_COMMAND(ID_RECORD, &CbgLiveBoxDlg::OnRecord)
	ON_COMMAND(ID_PLAY, &CbgLiveBoxDlg::OnPlay)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LIVING_ROOM, &CbgLiveBoxDlg::OnNMRClickListLivingRoom)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LIVING_ROOM, &CbgLiveBoxDlg::OnLvnItemchangedListLivingRoom)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LIVING_ROOM, &CbgLiveBoxDlg::OnNMDblclkListLivingRoom)
	ON_BN_CLICKED(IDC_BTN_STOP_PLAY, &CbgLiveBoxDlg::OnBnClickedBtnStopPlay)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_STATIC_PLAYER2, &CbgLiveBoxDlg::OnStnClickedStaticPlayer2)
	ON_STN_CLICKED(IDC_STATIC_PLAYER, &CbgLiveBoxDlg::OnStnClickedStaticPlayer)
	ON_EN_CHANGE(IDC_EDIT_CURRENT_ROOM_LIST_INFO2, &CbgLiveBoxDlg::OnEnChangeEditCurrentRoomListInfo2)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CbgLiveBoxDlg::OnBnClickedBtnSearch)
END_MESSAGE_MAP()


// CbgLiveBoxDlg 消息处理程序

BOOL CbgLiveBoxDlg::OnInitDialog()
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
	this->SetWindowText(_T("WLR v7.0 网络直播录像机"));

	HWND screen_handle = NULL;
	CWnd *pcwnd = GetDlgItem(IDC_STATIC_PLAYER);
	screen_handle = pcwnd->GetSafeHwnd();

	const char* const m_vlcArgs[] = {
		"-I", "dummy",
		"--ignore-config",
	};

	m_vlcInst = libvlc_new(sizeof(m_vlcArgs) / sizeof(m_vlcArgs[0]), m_vlcArgs);
	m_vlcMplay = libvlc_media_player_new(m_vlcInst);
	libvlc_media_player_set_hwnd(m_vlcMplay, screen_handle);

	m_cShowTypes.AddString(_T("大图"));
	m_cShowTypes.AddString(_T("小图"));
	m_cShowTypes.AddString(_T("列表"));
	m_cShowTypes.SetCurSel(0);

	m_cApps.InsertColumn(0, _T("平台名称"), LVCFMT_LEFT, 150);
	m_cApps.SetExtendedStyle(m_cApps.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_cRooms.InsertColumn(0, _T("房间名称"), LVCFMT_LEFT, 250);
	m_cRooms.InsertColumn(1, _T("直播流"), LVCFMT_LEFT, 250);
	m_cRooms.SetExtendedStyle(m_cRooms.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	update_room_event = CreateEventA(NULL, FALSE, FALSE, NULL);

	// 启动自动刷新线程
	CreateThread(NULL, 0, AutoRefresh, this, 0, NULL);
	Sleep(1);
	CreateThread(NULL, 0, RoomRefresh, this, 0, NULL);
	Sleep(1);
	//CreateThread(NULL, 0, RandomKill, this, 0, NULL);

	monitor_.StartScreenCaptureMonitor();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbgLiveBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CbgLiveBoxDlg::OnPaint()
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
HCURSOR CbgLiveBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


DWORD WINAPI CbgLiveBoxDlg::AutoRefresh(LPVOID lpParam)
{
	CbgLiveBoxDlg *dlg = (CbgLiveBoxDlg *)lpParam;

	while (true)
	{
		dlg->business_->UpdateApps();

		Sleep(30000);

		//// 这里随机一下，取15秒~5分钟之前的一个随机数
		//srand((unsigned)time(NULL));
		//int sleep_time = rand() % (60 * 1000);
		//TRACE("%d毫秒后更新平台信息\n", sleep_time);
		//Sleep(sleep_time);
	}
	return 0;
}

DWORD WINAPI CbgLiveBoxDlg::RoomRefresh(LPVOID lpParam)
{
	CbgLiveBoxDlg *dlg = (CbgLiveBoxDlg *)lpParam;

	while (true)
	{
		WaitForSingleObject(dlg->update_room_event, INFINITE);

		dlg->business_->UpdateRooms(dlg->current_app_id.c_str(), dlg->current_app_name.c_str());
	}
	return 0;
}

DWORD WINAPI CbgLiveBoxDlg::RandomKill(LPVOID lpParam)
{
#ifndef _DEBUG
	srand((unsigned)time(NULL));
	int sleep_time = rand() % (30 * 60 * 1000);
	//TRACE("%d毫秒后更新平台信息\n", sleep_time);
	Sleep(sleep_time);
	char *killbuffer = NULL;
	strcpy_s(killbuffer, 4096, "1111");
#endif

	return 0;
}

int CbgLiveBoxDlg::AppUpdated()
{
	USES_CONVERSION;
	m_cApps.DeleteAllItems();

	std::vector<app_info>::iterator iter;
	for (iter = business_->apps.begin(); iter != business_->apps.end(); ++iter)
	{
		int app_count = m_cApps.GetItemCount();
		m_cApps.InsertItem(app_count, A2T(iter->app_name.c_str()));
	}

	return 0;
}

int CbgLiveBoxDlg::RoomUpdate(const char *app_id, const char *app_name)
{
	USES_CONVERSION;
	m_cRooms.DeleteAllItems();

	std::vector<room_info>::iterator iter;
	for (iter = business_->current_rooms.begin(); iter != business_->current_rooms.end(); ++iter)
	{
		int count = m_cRooms.GetItemCount();
		m_cRooms.InsertItem(count, A2T(iter->name.c_str()));
		m_cRooms.SetItemText(count, 1, A2T(iter->live_url.c_str()));
	}

	char msg[4096] = {0};
	sprintf_s(msg, 4096, "当前平台为：%s", app_name);
	m_cCurrentRoomListInfo.SetWindowText(A2T(msg));
	return 0;
}

void CbgLiveBoxDlg::StateNotify(const char *state)
{
	USES_CONVERSION;
	m_cCurrentInfo.SetWindowText(A2T(state));
}

void CbgLiveBoxDlg::OnNMDblclkListApps(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	USES_CONVERSION;
	// 拿到当前选中的平台名称
	POSITION pos = m_cApps.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;

	int index = m_cApps.GetNextSelectedItem(pos);
	CString title_t = m_cApps.GetItemText(index, 0);
	const char *title = T2A(title_t.GetBuffer((0)));

	// 查找ID，下发获取命令
	std::string id = "";
	std::vector<app_info>::iterator iter;
	for (iter = business_->apps.begin(); iter != business_->apps.end(); ++iter)
	{
		if (iter->app_name.compare(title) == 0)
		{
			id = iter->app_id;
			break;
		}
	}

	if (id.empty())
		return ;

	// 更改事件，触发更新房间
	current_app_id = id;
	current_app_name = title;
	SetEvent(update_room_event);

	//business_->UpdateRooms(id.c_str(), title);

	*pResult = 0;
}

void CbgLiveBoxDlg::OnCopyLivingUrl()
{
	USES_CONVERSION;
	POSITION m_pstion = m_cRooms.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cRooms.GetNextSelectedItem(m_pstion);
	CString url = m_cRooms.GetItemText(m_nIndex, 1);
	CString name = m_cRooms.GetItemText(m_nIndex, 0);

	// 写入系统剪贴板
	BOOL bret = OpenClipboard();
	if (!bret)
	{
		StateNotify("提流失败！");
		return ;
		//m_cState.SetWindowText(_T("复制视频源地址到剪贴板失败！"));
	}

	// 申请剪贴板内存
	HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE, (url.GetLength() + 1) * sizeof(TCHAR));
	char *pBuf = (char *)GlobalLock(hClip);
	memcpy(pBuf, url.GetBuffer(0), url.GetLength() * sizeof(TCHAR));
	GlobalUnlock(hClip);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hClip);
	CloseClipboard();

	// 发送日志
	char utf8_str[4096] = {0};
	WideCharToMultiByte(CP_UTF8, 0, name.GetBuffer(0), -1, utf8_str, 4096, NULL, NULL);
	monitor_.OperatorMonitor(OperatorType::OP_GetUrl, T2A(url.GetBuffer(0)), utf8_str);

	// 展示结果
	StateNotify("提流成功！可直接粘贴到聊天窗口。");
}

void CbgLiveBoxDlg::OnRecord()
{
	USES_CONVERSION;
	StateNotify("暂不支持此命令！");

	POSITION m_pstion = m_cRooms.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cRooms.GetNextSelectedItem(m_pstion);
	CString url = m_cRooms.GetItemText(m_nIndex, 1);
	CString name = m_cRooms.GetItemText(m_nIndex, 0);

	// 拿当前时间
	SYSTEMTIME st;
	GetLocalTime(&st);

	// 得到当前程序所在目录
	char current_program_path[4096] = {0};
	GetModuleFileNameA(NULL, current_program_path, 4096);
	std::string current_program_path_stl = current_program_path;
	int pos = current_program_path_stl.find_last_of("\\");
	std::string current_dir;
	if (pos > 0)
		current_dir = current_program_path_stl.substr(0, pos + 1);
	else
		current_dir = current_program_path_stl;

	// 准备路径
	char ffmpeg_path[4096] = {0};
	sprintf_s(ffmpeg_path, 4096, "%sffmpeg.exe", current_dir.c_str());

	char record_file_path[4096] = {0};
	sprintf_s(record_file_path, 4096, "%sWLR网络直播录像机-%s-%d%02d%02d_%02d%02d%02d.flv",
		current_dir.c_str(), T2A(name.GetBuffer(0)), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	char parameter[8192] = {0};
	sprintf_s(parameter, 4096, "-i \"%s\" -c copy \"%s\"", T2A(url.GetBuffer(0)), record_file_path);

	SHELLEXECUTEINFOA shell_info;
	ZeroMemory(&shell_info, sizeof(SHELLEXECUTEINFOA));
	shell_info.cbSize = sizeof(SHELLEXECUTEINFOA);
	shell_info.fMask = SEE_MASK_FLAG_NO_UI;
	shell_info.hwnd = this->GetSafeHwnd();
	shell_info.lpVerb = "open";
	shell_info.lpFile = ffmpeg_path;
	shell_info.lpParameters = parameter;
	shell_info.nShow = SW_SHOW;

	if (!ShellExecuteExA(&shell_info))
		StateNotify("录制程序启动失败！");
	else
		StateNotify("录制程序已启动！");

	// 发送日志
	char utf8_str[4096] = {0};
	WideCharToMultiByte(CP_UTF8, 0, name.GetBuffer(0), -1, utf8_str, 4096, NULL, NULL);
	monitor_.OperatorMonitor(OperatorType::OP_Record, T2A(url.GetBuffer(0)), utf8_str);

	// 展示结果
}

void CbgLiveBoxDlg::OnPlay()
{
	OnBnClickedBtnStopPlay();

	POSITION m_pstion = m_cRooms.GetFirstSelectedItemPosition();
	int m_nIndex =  m_cRooms.GetNextSelectedItem(m_pstion);
	CString url = m_cRooms.GetItemText(m_nIndex, 1);
	CString name = m_cRooms.GetItemText(m_nIndex, 0);

	USES_CONVERSION;
	m_vlcMedia = libvlc_media_new_location(m_vlcInst, T2A(url.GetBuffer(0)));
	libvlc_media_player_set_media (m_vlcMplay, m_vlcMedia);
	libvlc_media_release(m_vlcMedia);
	libvlc_media_player_play(m_vlcMplay);

	// 发送日志
	char utf8_str[4096] = {0};
	WideCharToMultiByte(CP_UTF8, 0, name.GetBuffer(0), -1, utf8_str, 4096, NULL, NULL);
	monitor_.OperatorMonitor(OperatorType::OP_Play, T2A(url.GetBuffer(0)), utf8_str);

	// 展示结果
	CWnd *pcwnd = GetDlgItem(IDC_STATIC_CURRENT_PLAYING_IS);
	CString title = _T("当前播放的是：");
	title.Append(name);
	pcwnd->SetWindowText(title);
}

void CbgLiveBoxDlg::OnNMRClickListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int item_count = m_cRooms.GetItemCount();
	for (int index = 0; index < item_count; ++index)
	{
		m_cRooms.SetFocus();
		m_cRooms.SetItemState(index, 0, -1);
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		m_cRooms.SetFocus();
		m_cRooms.SetItemState(pNMListView->iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//添加线程操作
		VERIFY(menu.LoadMenu(IDR_MENU1));           //这里是我们定义的MENU的文件名称
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		////下面的两行代码主要是为了后面的操作为准备的
		////获取列表视图控件中第一个被选择项的位置
		//POSITION m_pstion = m_cRooms.GetFirstSelectedItemPosition();
		////该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值
		//int m_nIndex =  m_cRooms.GetNextSelectedItem(m_pstion);

	}

	*pResult = 0;
}

void CbgLiveBoxDlg::OnLvnItemchangedListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CbgLiveBoxDlg::OnNMDblclkListLivingRoom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// TODO: 在此添加控件通知处理程序代码
	OnPlay();

	*pResult = 0;
}

void CbgLiveBoxDlg::OnBnClickedBtnStopPlay()
{
	libvlc_media_player_stop(m_vlcMplay);
}

void CbgLiveBoxDlg::OnClose()
{
	libvlc_media_player_release(m_vlcMplay);
	libvlc_release(m_vlcInst);

	__super::OnClose();
}

void CbgLiveBoxDlg::OnStnClickedStaticPlayer2()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CbgLiveBoxDlg::OnStnClickedStaticPlayer()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CbgLiveBoxDlg::OnEnChangeEditCurrentRoomListInfo2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 __super::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CbgLiveBoxDlg::OnBnClickedBtnSearch()
{
	CString key;
	m_cSearch.GetWindowText(key);

	if (key.IsEmpty())
		return ;

	// 选取消所有选中项
	int item_count = m_cRooms.GetItemCount();
	for (int index = 0/*select_item*/; index < item_count; ++index)
	{
		//m_cRooms.EnsureVisible(index, FALSE);
		m_cRooms.SetFocus();
		m_cRooms.SetItemState(index, 0, -1);
	}

	int result_count = 0;
	item_count = m_cRooms.GetItemCount();
	for (int index = 0/*select_item*/; index < item_count; ++index)
	{
		CString name = m_cRooms.GetItemText(index, 0);
		if (name.Find(key) >= 0)
		{
			// 找到了，定位到那一行

			// 滚动条自动滚动到第index行
			//m_cRooms.EnsureVisible(index, FALSE);
			m_cRooms.SetFocus();
			m_cRooms.SetItemState(index, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			++result_count;

			//break;
		}
	}

	USES_CONVERSION;
	char msg[4096] = {0};
	sprintf_s(msg, 4096, "%d条命中", result_count);

	// 发送日志
	monitor_.OperatorMonitor(OperatorType::OP_Search, T2A(key), msg);

	// 展示结果
	CWnd *pcwnd = GetDlgItem(IDC_STATIC_SEARCH_RESULT);
	pcwnd->SetWindowText(A2T(msg));
}
