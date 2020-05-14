// Dialog_About.cpp: 实现文件
//

#include "pch.h"
#include "VirtualKeyBoardMouseConfigTool.h"
#include "Dialog_About.h"
#include "afxdialogex.h"


// Dialog_About 对话框

IMPLEMENT_DYNAMIC(Dialog_About, CDialog)

Dialog_About::Dialog_About(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_ABOUTBOX, pParent)
{

}

Dialog_About::~Dialog_About()
{
}

void Dialog_About::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog_About, CDialog)
END_MESSAGE_MAP()


// Dialog_About 消息处理程序
