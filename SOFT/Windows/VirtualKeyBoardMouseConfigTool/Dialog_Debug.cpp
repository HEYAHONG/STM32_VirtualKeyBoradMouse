// Dialog_Debug.cpp: 实现文件
//

#include "pch.h"
#include "VirtualKeyBoardMouseConfigTool.h"
#include "Dialog_Debug.h"
#include "afxdialogex.h"
#include "VirtualKeyBoardMouseConfigToolDlg.h"
extern CVirtualKeyBoardMouseConfigToolDlg* parent;

// Dialog_Debug 对话框

IMPLEMENT_DYNAMIC(Dialog_Debug, CDialog)

Dialog_Debug::Dialog_Debug(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_Debug, pParent)
{

}

Dialog_Debug::~Dialog_Debug()
{
}

void Dialog_Debug::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog_Debug, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Dialog_Debug::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Dialog_Debug::OnClickedButton2)
END_MESSAGE_MAP()


// Dialog_Debug 消息处理程序


void Dialog_Debug::OnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("准备打开USB设备\n\r"), _T("提示"));
	if (parent->LibusbK_init() != 0)
	{
		MessageBox(_T("准备打开USB设备失败\n\r"), _T("警告"));
	}
}


void Dialog_Debug::OnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char out[64] = "VirtualKeyBoardMouse", in[64] = {};
	if (parent->LibusbK_write(out) > 0)
	{
		if (parent->LibusbK_read(in)>0)
		{
			MessageBoxA(NULL,(char *) in, "读取结果", MB_OK);
		}
	}
}
