
// PEVIEWER-GUIDlg.h: 头文件
//

#pragma once
#include "ExportDlg.h"

// CPEVIEWERGUIDlg 对话框
class CPEVIEWERGUIDlg : public CDialogEx
{
// 构造
public:
	CPEVIEWERGUIDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEVIEWERGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:



	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_filepath;
	afx_msg void OnEnChangeEdit1();
	CString m_entry;
private:
	CListBox m_sections;
public:
//	CEdit m_imageofbase;
	CString m_imageofbase;
	afx_msg void OnLbnSelchangeSections();
private:
	CString m_rangeSection;
	CString m_pointerofraw;
	CString m_sizeofrawdata;
	CString m_arch;
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
};
