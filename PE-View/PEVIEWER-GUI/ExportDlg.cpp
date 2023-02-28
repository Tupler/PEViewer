// ExportDlg.cpp: 实现文件
//

#include "pch.h"
#include "PEVIEWER-GUI.h"
#include "afxdialogex.h"
#include "ExportDlg.h"


// ExportDlg 对话框

IMPLEMENT_DYNAMIC(ExportDlg, CDialogEx)

ExportDlg::ExportDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_exportfunc(_T(""))
	, m_numberofexp(_T(""))
{

}

ExportDlg::~ExportDlg()
{
}

void ExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_exportlist);
	DDX_Text(pDX, IDC_EDIT2, m_exportfunc);
	DDX_Text(pDX, IDC_EDIT1, m_numberofexp);
}


BEGIN_MESSAGE_MAP(ExportDlg, CDialogEx)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST1, &ExportDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// ExportDlg 消息处理程序


void ExportDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					 
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	/*CString str;
	str.Format(TEXT(
		"%d"
	), optional_Header64.DataDirectory[0].VirtualAddress);
	MessageBox(str);*/
	if (optional_Header32.DataDirectory[0].VirtualAddress != 0 && isx86==0) {

		DWORD eatoffset = RVA2RAW(optional_Header32.DataDirectory[0].VirtualAddress);
		//RAW加上imagebase 因为是直接读文件并没有映射为内存
		PIMAGE_EXPORT_DIRECTORY eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD)dos_Header + eatoffset);
		PDWORD functionanems = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNames));
		PWORD addressOfNameOrdinals = (PWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNameOrdinals));
		PDWORD addressOfFunction = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfFunctions));
		nums = eat->NumberOfNames;
		m_numberofexp.Format(TEXT("%d"), nums);
		UpdateData(FALSE);
		for (int i = 0; i < eat->NumberOfNames; i++)
		{
			WORD ordinalsIndex = addressOfNameOrdinals[i];
			CString* fuck = new CString((char*)RVA2RAW(functionanems[i]) + DWORD(dos_Header), strlen((char*)RVA2RAW(functionanems[i]) + DWORD(dos_Header)));
			m_exportlist.InsertString(0, *fuck);

		}
	}
	else if (optional_Header64.DataDirectory[0].VirtualAddress != 0 && isx86 == 1) {
		DWORD eatoffset = RVA2RAW(optional_Header64.DataDirectory[0].VirtualAddress);
		//RAW加上imagebase 因为是直接读文件并没有映射为内存
		PIMAGE_EXPORT_DIRECTORY eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD)dos_Header + eatoffset);
		PDWORD functionanems = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNames));
		PWORD addressOfNameOrdinals = (PWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfNameOrdinals));
		PDWORD addressOfFunction = (PDWORD)((DWORD)dos_Header + RVA2RAW(eat->AddressOfFunctions));
		nums = eat->NumberOfNames;
		m_numberofexp.Format(TEXT("%d"), nums);
		UpdateData(FALSE);
		for (int i = 0; i < eat->NumberOfNames; i++)
		{
			WORD ordinalsIndex = addressOfNameOrdinals[i];
			CString* fuck = new CString((char*)RVA2RAW(functionanems[i]) + DWORD(dos_Header), strlen((char*)RVA2RAW(functionanems[i]) + DWORD(dos_Header)));
			m_exportlist.InsertString(0, *fuck);
		}
	}
}


void ExportDlg::OnLbnSelchangeList1()
{
	//;
	m_exportlist.GetText(m_exportlist.GetCurSel(), m_exportfunc);
	UpdateData(FALSE);
	// TODO: 在此添加控件通知处理程序代码
}
