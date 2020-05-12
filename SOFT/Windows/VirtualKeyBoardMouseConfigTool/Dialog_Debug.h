#pragma once
//添加父窗口类
//#include "VirtualKeyBoardMouseConfigToolDlg.h"

// Dialog_Debug 对话框

class Dialog_Debug : public CDialog
{
	DECLARE_DYNAMIC(Dialog_Debug)

public:
	Dialog_Debug(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dialog_Debug();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Debug };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	
	afx_msg void OnClickedButton2();
	afx_msg void OnEnChangeEdit1();
	CEdit edit_write;
	CEdit edit_read;
	CButton edit_read_hex;
	CButton edit_write_hex;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
};
