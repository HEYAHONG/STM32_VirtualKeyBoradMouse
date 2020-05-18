// Dialog_Help.cpp: 实现文件
//

#include "pch.h"
#include "VirtualKeyBoardMouseConfigTool.h"
#include "Dialog_Help.h"
#include "afxdialogex.h"


// Dialog_Help 对话框

IMPLEMENT_DYNAMIC(Dialog_Help, CDialog)

Dialog_Help::Dialog_Help(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_Help, pParent)
{

}

Dialog_Help::~Dialog_Help()
{
}

void Dialog_Help::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, help_edit);
}


BEGIN_MESSAGE_MAP(Dialog_Help, CDialog)
END_MESSAGE_MAP()


// Dialog_Help 消息处理程序


BOOL Dialog_Help::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置好help的编辑框大小
	CRect window;
	GetClientRect(&window);
	window.left += 5;
	window.bottom -= 5;
	window.right -= 5;
	window.top += 5;

	help_edit.SetWindowPos(NULL, window.left, window.top, window.Width(), window.Height(), SWP_SHOWWINDOW);


	//加载帮助资源
	//寻找资源
	HRSRC hrSrc=FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_TXT_Help), _T("TXT"));
	if (hrSrc != NULL)
	{
		//加载资源
		HGLOBAL handle = LoadResource(GetModuleHandle(NULL), hrSrc);
		if (handle != NULL)
		{
			//获得资源大小
			size_t resource_size = SizeofResource(GetModuleHandle(NULL), hrSrc);

			//锁定资源
			char* p = (char *)LockResource(handle);

			if (p != NULL)
			{
				wchar_t * buff = new wchar_t[resource_size+1];
				memset(buff, 0, sizeof(wchar_t) * (resource_size + 1));
				MultiByteToWideChar(CP_ACP, 0, (char*)p, resource_size, buff, resource_size);
				help_edit.SetWindowTextW(buff);
				delete[] buff;
			}

			//释放资源
			FreeResource(handle);
			//CloseHandle(handle);
		}
		//CloseHandle(hrSrc);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Dialog_Help::RefreshWindowSize()
{
	// TODO: 在此处添加实现代码.
	//设置好help的编辑框大小
	CRect window;
	GetClientRect(&window);
	window.left += 5;
	window.bottom -= 5;
	window.right -= 5;
	window.top += 5;

	help_edit.SetWindowPos(NULL, window.left, window.top, window.Width(), window.Height(), SWP_SHOWWINDOW);
}
