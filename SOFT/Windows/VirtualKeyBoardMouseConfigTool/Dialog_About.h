#pragma once


// Dialog_About 对话框

class Dialog_About : public CDialog
{
	DECLARE_DYNAMIC(Dialog_About)

public:
	Dialog_About(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dialog_About();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
