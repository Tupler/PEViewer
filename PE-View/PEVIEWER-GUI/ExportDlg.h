#pragma once
#include "afxdialogex.h"
//#include "PEVIEWER-GUIDlg.h"


// ExportDlg 对话框

class ExportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ExportDlg)

public:
	ExportDlg( CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ExportDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_exportlist;
	CString m_exportfunc;
	afx_msg void OnPaint();
	afx_msg void OnLbnSelchangeList1();
private:
	CString m_numberofexp;
};
