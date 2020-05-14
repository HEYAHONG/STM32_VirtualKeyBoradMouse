
// VirtualKeyBoardMouseConfigToolDlg.h: 头文件
//

#include "Dialog_Debug.h"
#include "Dialog_Help.h"

//导入libusb库

#include "libusbK.h"



#ifdef WIN32
//32位库
#pragma comment(lib,"libusbK\\lib\\x86\\libusbK.lib")
//amd64位库
//#pragma comment(lib,"libusbK\\lib\\amd64\\libusbK.lib")
//IA64库
//#pragma comment(lib,"libusbK\\lib\\ia64\\libusbK.lib")
#endif

#pragma once




// CVirtualKeyBoardMouseConfigToolDlg 对话框
class CVirtualKeyBoardMouseConfigToolDlg : public CDialog
{
// 构造
public:
	CVirtualKeyBoardMouseConfigToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUALKEYBOARDMOUSECONFIGTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// TAB页
	CTabCtrl tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	Dialog_Debug tab_debug;
	Dialog_Help  tab_help;

	//LibUSBK相关变量
	KUSB_DRIVER_API Usb;
	KLST_HANDLE deviceList;
	KLST_DEVINFO_HANDLE deviceInfo;
	KUSB_HANDLE handle;

	int LibusbK_init();
	afx_msg void OnBnClickedButton1();
	int LibusbK_read(unsigned char* data);
	int LibusbK_write(unsigned char* data);
	void about_dialog();
};
