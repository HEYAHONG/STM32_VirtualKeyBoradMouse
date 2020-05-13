
// VirtualKeyBoardMouseConfigToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "VirtualKeyBoardMouseConfigTool.h"
#include "VirtualKeyBoardMouseConfigToolDlg.h"
#include "afxdialogex.h"

#include "Dialog_Debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVirtualKeyBoardMouseConfigToolDlg* parent = NULL;//父窗口变量

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CVirtualKeyBoardMouseConfigToolDlg 对话框



CVirtualKeyBoardMouseConfigToolDlg::CVirtualKeyBoardMouseConfigToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_VIRTUALKEYBOARDMOUSECONFIGTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVirtualKeyBoardMouseConfigToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, tab);
}

BEGIN_MESSAGE_MAP(CVirtualKeyBoardMouseConfigToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CVirtualKeyBoardMouseConfigToolDlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CVirtualKeyBoardMouseConfigToolDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CVirtualKeyBoardMouseConfigToolDlg 消息处理程序

BOOL CVirtualKeyBoardMouseConfigToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	

	//TAB页
	CRect tabRect;   // 标签控件客户区的位置和大小 
	tab.GetClientRect(&tabRect);
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;
	
	parent = this;

	//初始化某些成员变量
	deviceInfo = NULL;
	deviceList = NULL;
	handle = NULL;



	//调试页
	tab.InsertItem(0, _T("调试"));

	tab_debug.Create(IDD_DIALOG_Debug,&tab);


	tab_debug.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

	//tab_debug.ShowWindow(SW_SHOW);

	//初始化libusb
	LibusbK_init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVirtualKeyBoardMouseConfigToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVirtualKeyBoardMouseConfigToolDlg::OnPaint()
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
HCURSOR CVirtualKeyBoardMouseConfigToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVirtualKeyBoardMouseConfigToolDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


int CVirtualKeyBoardMouseConfigToolDlg::LibusbK_init()
{
	// TODO: 在此处添加实现代码.
#define USB_VID 0x483
#define USB_PID 0x572B
#define USB_INTERFACE 0x02
	UINT deviceCount = 0;

	// Get the device list
	if (!LstK_Init(&deviceList, KLST_FLAG_NONE))
	{
		printf("Error initializing device list.\n");
		return -1;
	}

	LstK_Count(deviceList, &deviceCount);
	if (!deviceCount)
	{
		printf("Device list empty.\n");
		SetLastError(ERROR_DEVICE_NOT_CONNECTED);

		// If LstK_Init returns TRUE, the list must be freed.
		LstK_Free(deviceList);
		deviceList = NULL;
		return -1;
	}
	deviceInfo = NULL;
	LstK_FindByVidPid(deviceList, USB_VID, USB_PID, &deviceInfo);
	if (deviceInfo)
	{
		

		// Report the connection state of the example device
		printf("Using %04X:%04X (%s): %s - %s\n",
			deviceInfo->Common.Vid,
			deviceInfo->Common.Pid,
			deviceInfo->Common.InstanceID,
			deviceInfo->DeviceDesc,
			deviceInfo->Mfg);

		//return 0;
	}
	else
	{
		// Display some simple usage information for the example applications.
		CHAR programPath[MAX_PATH] = { 0 };
		PCHAR programExe = programPath;
		GetModuleFileNameA(GetModuleHandleA(NULL), programPath, sizeof(programPath));
		while (strpbrk(programExe, "\\/")) programExe = strpbrk(programExe, "\\/") + 1;
		printf("Device vid/pid %04X/%04X not found.\n\n", USB_VID, USB_PID);
		printf("USAGE: %s vid=%04X pid=%04X\n\n", programExe, USB_VID, USB_PID);

		// If LstK_Init returns TRUE, the list must be freed.
		LstK_Free(deviceList);
		deviceList = NULL;
		return -1;
	}

	LibK_LoadDriverAPI(&Usb, deviceInfo->DriverID);

	if (handle != NULL)
	{
		Usb.ReleaseInterface(handle, USB_INTERFACE, FALSE);
		Usb.Free(handle);
		handle = NULL;
	}

	// Initialize the device
	if (!Usb.Init(&handle, deviceInfo))
	{
		// If LstK_Init returns TRUE, the list must be freed.
		LstK_Free(deviceList);
		deviceList = NULL;
		return -1;
	}

	if (!Usb.ClaimInterface(handle, USB_INTERFACE,FALSE))
	{
		
		DWORD errorCode = GetLastError();
		if (errorCode == ERROR_NO_MORE_ITEMS)
			printf("Interface number %02Xh does not exists.\n", USB_INTERFACE);
		else
			printf("Usb.ClaimInterface failed. ErrorCode: %08Xh\n", errorCode);
		Usb.Free(handle);
		handle = NULL;
		// If LstK_Init returns TRUE, the list must be freed.
		LstK_Free(deviceList);
		deviceList = NULL;
		return -1;
	}

	
	return 0;
}


void CVirtualKeyBoardMouseConfigToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


#define USB_IN_EndPoint 0x85
#define USB_IN_EndPoint_Length 32 

int CVirtualKeyBoardMouseConfigToolDlg::LibusbK_read(unsigned char* data)//data的长度必须为32字节
{
	// TODO: 在此处添加实现代码.
	if (handle == NULL)
	{
		MessageBox(_T("USB设备尚未初始化!\n"), _T("警告"));
		return -1;
	}
	unsigned int ret = 0;
	Usb.ReadPipe(handle, USB_IN_EndPoint, data, USB_IN_EndPoint_Length, &ret, NULL);
	return ret;
}

#define USB_OUT_EndPoint 0x04
#define USB_OUT_EndPoint_Length 32



int CVirtualKeyBoardMouseConfigToolDlg::LibusbK_write(unsigned char* data)//data的长度必须为32字节
{
	// TODO: 在此处添加实现代码.
	if (handle == NULL)
	{
		MessageBox(_T("USB设备尚未初始化!\n"), _T("警告"));
		return -1;
	}
	unsigned int ret = 0;
	Usb.WritePipe(handle, USB_OUT_EndPoint, data, USB_OUT_EndPoint_Length, &ret, NULL);
	return ret;
	return 0;
}
