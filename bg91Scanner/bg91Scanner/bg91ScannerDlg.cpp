
// bg91ScannerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bg91Scanner.h"
#include "bg91ScannerDlg.h"

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


// Cbg91ScannerDlg 对话框




Cbg91ScannerDlg::Cbg91ScannerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cbg91ScannerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cbg91ScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROOT_URL, m_cURL);
	DDX_Control(pDX, IDC_LIST_ITEM_LIST, m_cVideoList);
}

BEGIN_MESSAGE_MAP(Cbg91ScannerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_START_SCAN, &Cbg91ScannerDlg::OnBnClickedBtnStartScan)
	ON_BN_CLICKED(IDC_BTN_PREVIOUS, &Cbg91ScannerDlg::OnBnClickedBtnPrevious)
	ON_BN_CLICKED(IDC_BTN_NEXT, &Cbg91ScannerDlg::OnBnClickedBtnNext)
END_MESSAGE_MAP()


// Cbg91ScannerDlg 消息处理程序

BOOL Cbg91ScannerDlg::OnInitDialog()
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
	m_cURL.SetWindowText(_T("https://0228.p15.rocks"));

	m_cVideoList.InsertColumn(0, _T("视频名称"), LVCFMT_LEFT, 400);
	m_cVideoList.InsertColumn(1, _T("视频时长"), LVCFMT_LEFT, 80);
	m_cVideoList.InsertColumn(2, _T("视频作者"), LVCFMT_LEFT, 100);
	m_cVideoList.InsertColumn(3, _T("视频地址"), LVCFMT_LEFT, 700);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cbg91ScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cbg91ScannerDlg::OnPaint()
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
HCURSOR Cbg91ScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cbg91ScannerDlg::OnBnClickedBtnStartScan()
{
	CString url;
	m_cURL.GetWindowText(url);

	USES_CONVERSION;
	int errCode = parse_.Initialize(T2A(url.GetBuffer(0)));
	if (errCode != 0)
	{
		MessageBox(_T("连接到服务器失败！"), _T("错误"), MB_OK|MB_ICONERROR);
		return ;
	}

	errCode = parse_.ScanFirstPage();

	// 展示第一页
	m_cVideoList.DeleteAllItems();
	std::vector<VIDEO_INFO>::iterator iter;
	for (iter = parse_.videos_.begin(); iter != parse_.videos_.end(); ++iter)
	{
		int count = m_cVideoList.GetItemCount();

		m_cVideoList.InsertItem(count, A2T(iter->video_name_.c_str()));
		m_cVideoList.SetItemText(count, 1, A2T(iter->video_duration_.c_str()));
		m_cVideoList.SetItemText(count, 2, A2T(iter->video_author_.c_str()));
		m_cVideoList.SetItemText(count, 3, A2T(iter->video_page_.c_str()));
	}
}

void Cbg91ScannerDlg::OnBnClickedBtnPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
}

void Cbg91ScannerDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
}
