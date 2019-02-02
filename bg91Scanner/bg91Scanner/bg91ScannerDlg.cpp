
// bg91ScannerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "bg91Scanner.h"
#include "bg91ScannerDlg.h"

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


// Cbg91ScannerDlg �Ի���




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


// Cbg91ScannerDlg ��Ϣ�������

BOOL Cbg91ScannerDlg::OnInitDialog()
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
	m_cURL.SetWindowText(_T("https://0228.p15.rocks"));

	m_cVideoList.InsertColumn(0, _T("��Ƶ����"), LVCFMT_LEFT, 400);
	m_cVideoList.InsertColumn(1, _T("��Ƶʱ��"), LVCFMT_LEFT, 80);
	m_cVideoList.InsertColumn(2, _T("��Ƶ����"), LVCFMT_LEFT, 100);
	m_cVideoList.InsertColumn(3, _T("��Ƶ��ַ"), LVCFMT_LEFT, 700);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cbg91ScannerDlg::OnPaint()
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
		MessageBox(_T("���ӵ�������ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
		return ;
	}

	errCode = parse_.ScanFirstPage();

	// չʾ��һҳ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void Cbg91ScannerDlg::OnBnClickedBtnNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
