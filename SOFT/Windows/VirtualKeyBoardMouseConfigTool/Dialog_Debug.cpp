// Dialog_Debug.cpp: 实现文件
//

#include "pch.h"
#include "VirtualKeyBoardMouseConfigTool.h"
#include "Dialog_Debug.h"
#include "afxdialogex.h"
#include "VirtualKeyBoardMouseConfigToolDlg.h"
#include "iostream"
#include "string"
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
	DDX_Control(pDX, IDC_EDIT1, edit_write);
	DDX_Control(pDX, IDC_EDIT2, edit_read);
	DDX_Control(pDX, IDC_CHECK2, edit_read_hex);
	DDX_Control(pDX, IDC_CHECK1, edit_write_hex);
}


BEGIN_MESSAGE_MAP(Dialog_Debug, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Dialog_Debug::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Dialog_Debug::OnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &Dialog_Debug::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON4, &Dialog_Debug::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &Dialog_Debug::OnBnClickedButton3)
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


void Dialog_Debug::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void Dialog_Debug::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	unsigned char buff[32] = {};

	if (edit_write_hex.GetCheck() == BST_CHECKED)
	{
		MessageBox(_T("hex模式"));
	}
	else
	{
		wchar_t w_buff[32] = {};
		edit_write.GetLine(0, (LPTSTR)w_buff,sizeof(w_buff)/sizeof(wchar_t));
		wcstombs((char *)buff, w_buff, sizeof(buff));
		parent->LibusbK_write(buff);
		//MessageBox(_T("非hex模式"));
	}
}


void Dialog_Debug::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned char buff[32] = {};

	if (edit_read_hex.GetCheck() == BST_CHECKED)
	{
		MessageBox(_T("hex模式"));
	}
	else
	{
		wchar_t w_buff[32] = {};
		if (parent->LibusbK_read(buff) > 0)
		{

			mbstowcs(w_buff, (char *)buff, sizeof(w_buff) / sizeof(wchar_t));
			edit_read.SetWindowTextW(w_buff);
			//MessageBoxA(NULL,(char *) buff,"读取内容",MB_OK);
		}
		//MessageBox(_T("非hex模式"));
	}
}


BOOL Dialog_Debug::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置初始状态
	edit_read_hex.SetCheck(BST_CHECKED);
	edit_write_hex.SetCheck(BST_CHECKED);

	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
