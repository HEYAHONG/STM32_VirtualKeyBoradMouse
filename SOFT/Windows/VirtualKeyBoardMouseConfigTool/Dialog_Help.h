#pragma once


// Dialog_Help 对话框

class Dialog_Help : public CDialog
{
	DECLARE_DYNAMIC(Dialog_Help)

public:
	Dialog_Help(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Dialog_Help();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Help };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit help_edit;
	void RefreshWindowSize();
};
