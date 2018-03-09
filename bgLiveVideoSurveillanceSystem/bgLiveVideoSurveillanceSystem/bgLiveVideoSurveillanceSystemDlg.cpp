
// bgLiveVideoSurveillanceSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "bgLiveVideoSurveillanceSystem.h"
#include "bgLiveVideoSurveillanceSystemDlg.h"


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


// CbgLiveVideoSurveillanceSystemDlg �Ի���




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


// CbgLiveVideoSurveillanceSystemDlg ��Ϣ�������

BOOL CbgLiveVideoSurveillanceSystemDlg::OnInitDialog()
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
	CString v = GetMyVersion();
	TCHAR version[4096] = {0};
	_stprintf_s(version, 4096, _T("����ֱ����Ƶ���ƽ̨ V%s"), v.GetBuffer(0));
	SetWindowText(version);

	// ��ʼ�������б�
	m_cNetworkDevices.SetExtendedStyle(m_cNetworkDevices.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cNetworkDevices.InsertColumn(0, _T("��������"), 0, 210);
	m_cNetworkDevices.InsertColumn(1, _T("��������"), 0, 210);
	m_cNetworkDevices.InsertColumn(2, _T("����"), 0, 100);
	m_cNetworkDevices.InsertColumn(3, _T("����·��"), 0, 200);
	m_cNetworkDevices.InsertColumn(4, _T("����"), 0, 200);

	m_cSnifferURL.SetExtendedStyle(m_cSnifferURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cSnifferURL.InsertColumn(0, _T("ץȡ����ƵԴ"), 0, 620);

	m_cSnifferURL.InsertItem(0, _T("rtmp://live.hkstv.hk.lxdns.com/live/hks"));

	m_cRecordURL.SetExtendedStyle(m_cRecordURL.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cRecordURL.InsertColumn(0, _T("¼���ļ���"), 0, 250);
	m_cRecordURL.InsertColumn(1, _T("��¼�ƴ�С"), 0, 100);
	m_cRecordURL.InsertColumn(2, _T("��ƵԴ"), 0, 270);

	// Ĭ��ʹ�ó�������Ŀ¼��Ϊ¼��洢��·��
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

	// ��ʼ��������
	RECT r;
	m_cScreen.GetRect(&r);
	int errCode = player_->Initialize(r.right - r.left, r.bottom - r.top, 1, m_cScreen.GetSafeHwnd());
	if (errCode != 0)
	{
		m_cState.SetWindowText(_T("��ʼ���������ʧ�ܣ�"));
		return TRUE;
	}

	// ö�ٵ�ǰ����
	EnumNetworkDevices();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CbgLiveVideoSurveillanceSystemDlg::OnPaint()
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

	GetModuleFileName(NULL, szFullPath, sizeof(szFullPath));			//��һ���ܹؼ��������Զ���ȡ.exe�ļ�·��
	dwVerInfoSize = GetFileVersionInfoSize(szFullPath, &dwVerHnd);	// ��һ�����ؼ���ֱ�Ӿ��ǻ�ȡ������
	if (dwVerInfoSize > 0)
	{
		HANDLE     hMem;
		LPVOID     lpvMem;
		unsigned   int   uInfoSize = 0;

		hMem = GlobalAlloc(GMEM_MOVEABLE, dwVerInfoSize);
		lpvMem = GlobalLock(hMem);
		GetFileVersionInfo(szFullPath, dwVerHnd, dwVerInfoSize, lpvMem);//�������ָ���ļ��İ汾��Ϣ��

		if (!::VerQueryValue(lpvMem, (LPTSTR)_T("\\"), (LPVOID*)&pFileInfo, &uInfoSize))//��ߵĻ���Ҫ�����ڴ�ָ���İ汾��ϢԴ��ȡ�汾��Ϣ
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
	// ����˿�ʼ��أ���������Ȼ�����������߳�
	POSITION pos = m_cNetworkDevices.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return ;

	int selected_item = m_cNetworkDevices.GetNextSelectedItem(pos);
	CString dev_path = m_cNetworkDevices.GetItemText(selected_item, 3);
	CString mask_ip = m_cNetworkDevices.GetItemText(selected_item, 4);

	USES_CONVERSION;
	int errCode = sniffer_->OpenNetworkDevice(T2A(dev_path.GetBuffer(0)), _ttoi(mask_ip.GetBuffer(0)));
	if (errCode != 0)
		m_cState.SetWindowText(_T("����������"));
	
}

void CbgLiveVideoSurveillanceSystemDlg::OnBnClickedBtnStopMonitor()
{
	// �����ֹͣ���
	sniffer_->CloseNetworkDevice();

	// ���±���һ�������豸
	//EnumNetworkDevices();
}

int CbgLiveVideoSurveillanceSystemDlg::SnifferResultReport(const char *protocol, const char *value)
{
	USES_CONVERSION;

	// ���ȼ�����url�Ƿ��������б����г��֣�����У��򲻴���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���
		VERIFY(menu.LoadMenu(IDR_MENU_SNIFFER));           //������������1�ж����MENU���ļ�����
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��
		POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ
		int m_nIndex =  m_cSnifferURL.GetNextSelectedItem(m_pstion);

	}
}

void CbgLiveVideoSurveillanceSystemDlg::OnNMRClickListRecords(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		//����̲߳���
		VERIFY(menu.LoadMenu(IDR_MENU_RECORD));           //���������Ƕ����MENU���ļ�����
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		//��������д�����Ҫ��Ϊ�˺���Ĳ���Ϊ׼����
		//��ȡ�б���ͼ�ؼ��е�һ����ѡ�����λ��
		POSITION m_pstion = m_cSnifferURL.GetFirstSelectedItemPosition();
		//�ú�����ȡ��posָ�����б����������Ȼ��pos����Ϊ��һ��λ�õ�POSITIONֵ
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

	// д��ϵͳ������
	BOOL bret = OpenClipboard();
	if (!bret)
		m_cState.SetWindowText(_T("������ƵԴ��ַ��������ʧ�ܣ�"));

	// ����������ڴ�
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
	// TODO: �ڴ���������������
	CString all_urls;
	int item_count = m_cSnifferURL.GetItemCount();
	for (int index = 0; index < item_count; ++index)
	{
		CString url = m_cSnifferURL.GetItemText(index, 0);
		all_urls += url;
		all_urls += _T("\r\n");
	}

	// д��ϵͳ������
	BOOL bret = OpenClipboard();
	if (!bret)
		m_cState.SetWindowText(_T("������ƵԴ��ַ��������ʧ�ܣ�"));

	// ����������ڴ�
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

	// ����url����¼��ģ��
	USES_CONVERSION;
	stream_mgr_->HandleURL(T2A(url.GetString()), false, true);
}


int CbgLiveVideoSurveillanceSystemDlg::StreamNotifer(enum STREAM_NOTIFY_TYPE msg_type, const char *url)
{
	int errCode = 0;

	switch (msg_type)
	{
	case StreamSave:
		// ��Ҫ�����������������ַ��Sniffer�б����Ƴ������뵽¼���б�
		// Ȼ��URL�Ӹ�¼��ģ��
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
		// ���¸�·��
		break;
	case StreamRecordUpdateRecordInfo:
		// ����¼����Ϣ
		break;
	case StreamRecordStart:
		// ��ʼ¼��
		break;
	case StreamRecordPause:
		// ��ͣ¼��
		break;
	case StreamRecordStop:
		// ֹͣ¼��
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