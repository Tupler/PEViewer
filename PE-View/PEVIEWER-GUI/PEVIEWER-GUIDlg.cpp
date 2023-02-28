
// PEVIEWER-GUIDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PEVIEWER-GUI.h"
#include "PEVIEWER-GUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* contents=0;
IMAGE_FILE_HEADER file_Header ;
PIMAGE_NT_HEADERS64 nt_Header64 = 0;
PIMAGE_NT_HEADERS nt_Header32 = 0;
PIMAGE_SECTION_HEADER secion_table = 0;
DWORD image_base = 0;
DWORD numberofsection = 0;
HICON m_hIcon = 0;
PIMAGE_DOS_HEADER dos_Header = 0;
int isx86 = 0;
int nums = 0;
IMAGE_OPTIONAL_HEADER32 optional_Header32;
IMAGE_OPTIONAL_HEADER64 optional_Header64;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPEVIEWERGUIDlg 对话框



CPEVIEWERGUIDlg::CPEVIEWERGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PEVIEWERGUI_DIALOG, pParent)
	, m_filepath(_T(""))
	, m_entry(_T(""))
	, m_imageofbase(_T(""))
	, m_rangeSection(_T(""))
	, m_pointerofraw(_T(""))
	, m_sizeofrawdata(_T(""))
	, m_arch(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEVIEWERGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_filepath);
	DDX_Text(pDX, IDC_ENTRY, m_entry);
	DDX_Control(pDX, IDC_SECTIONS, m_sections);
	DDX_Text(pDX, IDC_EDIT3, m_imageofbase);
	DDX_Text(pDX, IDC_EDIT4, m_rangeSection);
	DDX_Text(pDX, IDC_EDIT6, m_pointerofraw);
	DDX_Text(pDX, IDC_EDIT7, m_sizeofrawdata);
	DDX_Text(pDX, IDC_EDIT8, m_arch);
}

BEGIN_MESSAGE_MAP(CPEVIEWERGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPEVIEWERGUIDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CPEVIEWERGUIDlg::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_SECTIONS, &CPEVIEWERGUIDlg::OnLbnSelchangeSections)
	ON_BN_CLICKED(IDC_BUTTON3, &CPEVIEWERGUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CPEVIEWERGUIDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


BOOL CPEVIEWERGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	
	return TRUE;  
}

void CPEVIEWERGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEVIEWERGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPEVIEWERGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//RVA -> RAW(FOA)

void CPEVIEWERGUIDlg::OnBnClickedButton1()
{

	


	CString strFile = _T("");
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("PE Files(*.dll;*.exe)|*.dll;*.exe|All Files (*.*)|*.*||"), NULL);
	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
	}
	if (strFile.IsEmpty()==TRUE) {
		return;
	}
	int Count = m_sections.GetCount();
	for (int i = Count; i >= 0; i--)
		m_sections.DeleteString (i);
	m_filepath = strFile;

//	HeapFree(GetProcessHeap(), 0, secion_table);
	UpdateData(FALSE);
	HeapFree(GetProcessHeap(), 0, contents);

	//char loadname[] = argv;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	DWORD dwReadSize = 0;
	hFile = CreateFile(m_filepath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 分配内存用于存放读取的文件数据
	contents = (PCHAR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwFileSize);
	ReadFile(hFile, contents, dwFileSize, &dwReadSize, NULL);
	CloseHandle(hFile);
	int filesize = dwFileSize;
	dos_Header = (PIMAGE_DOS_HEADER)contents;
	if (dos_Header->e_magic != IMAGE_DOS_SIGNATURE)
	{
		MessageBox(TEXT("it's not pe1"));
		return ;
	}
	// 0x1234 -> contents    contents=0xdeadbeef  0xdeadbeef->DOS_header
	nt_Header64 = (PIMAGE_NT_HEADERS64)((DWORD)contents + dos_Header->e_lfanew);
	nt_Header32 = (PIMAGE_NT_HEADERS)((DWORD)contents + dos_Header->e_lfanew);
	if (nt_Header32->Signature != IMAGE_NT_SIGNATURE || nt_Header64->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(TEXT("it's not pe2"));
		return ;
	}
	file_Header = (IMAGE_FILE_HEADER)nt_Header32->FileHeader;
	numberofsection = file_Header.NumberOfSections;
	DWORD imageBase = 0;
	if (file_Header.Machine == IMAGE_FILE_MACHINE_I386)
	{
		isx86 = 0;
		m_arch = "x86";
		//optionnalHeader =nt_Header32->OptionalHeader;
		optional_Header32 = (IMAGE_OPTIONAL_HEADER32)nt_Header32->OptionalHeader;
		imageBase = optional_Header32.ImageBase;
		m_entry.Format(TEXT("0x%x"), imageBase+ optional_Header32.AddressOfEntryPoint);
		m_imageofbase.Format(TEXT("0x%x"), imageBase);
		UpdateData(FALSE);
		secion_table = (PIMAGE_SECTION_HEADER)((DWORD)nt_Header32 + 24 + file_Header.SizeOfOptionalHeader);
		//memcpy(secion_table, (PIMAGE_SECTION_HEADER)((DWORD)nt_Header32 + 24 + file_Header.SizeOfOptionalHeader), sizeof(IMAGE_SECTION_HEADER)*numberofsection);
		for (int i = 0; i < numberofsection; i++)
		{
			
			CString* sectionName = new CString((char*)secion_table[i].Name, 10);
			m_sections.InsertString(i, *sectionName);
			
		}
		
		
		
	}
	else if (file_Header.Machine == IMAGE_FILE_MACHINE_AMD64)
	{
		isx86 = 1;
		m_arch = "x64";
		optional_Header64 = (IMAGE_OPTIONAL_HEADER64)nt_Header64->OptionalHeader;
		imageBase = optional_Header64.ImageBase;
		m_entry.Format(TEXT("0x%x"), imageBase + optional_Header64.AddressOfEntryPoint);
		m_imageofbase.Format(TEXT("0x%x"), imageBase);
		UpdateData(FALSE);
		secion_table = (PIMAGE_SECTION_HEADER)((DWORD)nt_Header64 + 24 + file_Header.SizeOfOptionalHeader);
		//memcpy(secion_table, (PIMAGE_SECTION_HEADER)((DWORD)nt_Header32 + 24 + file_Header.SizeOfOptionalHeader), sizeof(IMAGE_SECTION_HEADER) * numberofsection);
		//printf("+[节数量]:%d\n", numberofsection);
		//printf("+[节名称]\n");
		for (int i = 0; i < numberofsection; i++)
		{

			CString* sectionName = new CString((char*)secion_table[i].Name, 10);
			m_sections.InsertString(i, *sectionName);

		}


	}
	//释放掉读文件的内存 

}


void CPEVIEWERGUIDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
}


void CPEVIEWERGUIDlg::OnLbnSelchangeSections()
{
	int i =m_sections.GetCurSel();
	m_rangeSection.Format(TEXT("0x%x-0x%x"), secion_table[i].VirtualAddress, secion_table[i + 1].VirtualAddress);
	m_pointerofraw.Format(TEXT("0x%x"), secion_table[i].PointerToRawData);
	m_sizeofrawdata.Format(TEXT("0x%x"), secion_table[i].SizeOfRawData);
	UpdateData(FALSE);
	//Clipboard.SetDataObject(m_sections.SelectedItem.ToString());
	// TODO: 在此添加控件通知处理程序代码
}



void CPEVIEWERGUIDlg::OnBnClickedButton3()
{
	



}


void CPEVIEWERGUIDlg::OnBnClickedButton2()
{

		ExportDlg dlg;
		dlg.DoModal();


	// TODO: 在此添加控件通知处理程序代码
}


void CAboutDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}
