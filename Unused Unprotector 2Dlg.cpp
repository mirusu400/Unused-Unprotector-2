// Unused Unprotector 2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Unused Unprotector 2.h"
#include "Unused Unprotector 2Dlg.h"
#include "configuredialog.h"
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int x,y;
CUnprotectMap map;
extern HICON hIcon;
extern CString ProgName;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2Dlg dialog

CUnusedUnprotector2Dlg::CUnusedUnprotector2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnusedUnprotector2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnusedUnprotector2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnusedUnprotector2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnusedUnprotector2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnusedUnprotector2Dlg, CDialog)
	//{{AFX_MSG_MAP(CUnusedUnprotector2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_UNPROTECT, OnUnprotect)
	ON_COMMAND(ID_EDIT_UNPROTECT, OnEditUnprotect)
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(ID_FILE_OPEN1, OnFileOpen1)
	ON_COMMAND(ID_FILE_SAVE1, OnFileSave1)
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BUGREPORT, OnBugreport)
	ON_COMMAND(ID_HELP_BUGREPORT, OnHelpBugreport)
	ON_COMMAND(ID_EDIT_CONFIG, OnEditConfig)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2Dlg message handlers

BOOL CUnusedUnprotector2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetWindowText(ProgName);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_font.CreateFont(12,0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1,VARIABLE_PITCH | FF_ROMAN,TEXT("돋움"));
	GetDlgItem(IDC_EDIT1)->SetFont(&m_font);
	m_hAccel = ::LoadAccelerators(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnusedUnprotector2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnusedUnprotector2Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnusedUnprotector2Dlg::OnQueryDragIcon()
{
	return (HCURSOR)hIcon;
}

HBRUSH CUnusedUnprotector2Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(255,255,255));
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	else if (nCtlColor == CTLCOLOR_DLG)
	{
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CUnusedUnprotector2Dlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog cfd(TRUE);
	cfd.m_ofn.lpstrFilter = TEXT("Starcraft Map File(*.scm;*.scx)\0*.scm;*.scx\0")
				TEXT("Raw Scenario File(*.chk)\0*.chk\0All Files?\0unused.unused;\0");
	cfd.m_ofn.Flags |= OFN_HIDEREADONLY | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	CMenu *menu = GetMenu();

	if (map.GetVersion())
	{
		if (MessageBox(TEXT("Map is already opened. Do you wish to continue?"),ProgName,
			MB_OK|MB_YESNO|MB_ICONQUESTION) == IDNO) return;
		map.CloseMap();
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_UNPROTECT)->EnableWindow(FALSE);
		menu->EnableMenuItem(ID_FILE_SAVE1, MF_GRAYED);
		menu->EnableMenuItem(ID_EDIT_UNPROTECT, MF_GRAYED);
		Print_Message(TEXT("Map is closed."));
	}

	if (cfd.DoModal() == IDOK)
	{
		Print_Message(TEXT("Opening map..."));
		try
		{
			if (map.OpenMap(cfd.GetPathName()))
			{
				GetDlgItem(IDC_UNPROTECT)->EnableWindow(TRUE);
				menu->EnableMenuItem(ID_EDIT_UNPROTECT, MF_ENABLED);
				Print_Message(TEXT("Done."));
			}
			else
			{
				Print_Message(TEXT("Opening map failed. :(\r\nThis file must not be a correct map file."));
			}
		}
		catch (...)
		{
			Print_Message(TEXT("Opening map failed. :(\r\nThis file must not be a correct map file."));
			map.CloseMap();
		}
	}
}

void CUnusedUnprotector2Dlg::Print_Message(LPCTSTR lpszText)
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT1);
	CString str;
	static bool called=false;

	pEdit->GetWindowText(str);

	if (called) str += TEXT("\r\n");
	called++;
	str += lpszText;
	pEdit->LimitText(str.GetLength());
	pEdit->SetWindowText(str);
	pEdit->SetSel(str.GetLength(),-1);
	pEdit->RedrawWindow();
}

void CUnusedUnprotector2Dlg::OnSave() 
{
	CMenu *menu = GetMenu();
	// TODO: Add your control notification handler code here
	if (!map.GetVersion())
	{
		Print_Message(TEXT("How you clicked this button although map isn't opened? :("));
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_UNPROTECT)->EnableWindow(FALSE);
		menu->EnableMenuItem(ID_FILE_SAVE1, MF_GRAYED);
		menu->EnableMenuItem(ID_EDIT_UNPROTECT, MF_GRAYED);
		return;
	}

	CFileDialog cfd(FALSE);

	if (map.GetVersion() == BROODWAR)
	{
		cfd.m_ofn.lpstrFilter = TEXT("Broodwar Map File(*.scx)\0*.scx\0");
		cfd.m_ofn.lpstrDefExt = TEXT("scx");
	}
	else
	{
		cfd.m_ofn.lpstrFilter = TEXT("Original Map File(*.scm)\0*.scm\0");
		cfd.m_ofn.lpstrDefExt = TEXT("scm");
	}
	cfd.m_ofn.Flags |= OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (cfd.DoModal() == IDOK)
	{
		Print_Message(TEXT("Saving map..."));
		if (map.SaveMap(cfd.GetPathName(),false,true,ProgName))
		{
			Print_Message(TEXT("Done."));
		}
		else if (!map.GetOptionRef(5) && lstrcmp(map.GetFileName(),cfd.GetPathName()) == 0)
		{
			Print_Message(TEXT("User tried to overwrite. Saving map aborted."));
		}
		else
		{
			Print_Message(TEXT("Saving map failed. WTF :("));
		}
	}
}

DWORD WINAPI dwPHPThreadFunc(LPVOID);

void CUnusedUnprotector2Dlg::OnUnprotect() 
{
	CMenu *menu = GetMenu();
	// TODO: Add your control notification handler code here
	Print_Message(TEXT("Unprotecting(This may take a while).."));
	GetDlgItem(IDC_UNPROTECT)->EnableWindow(FALSE);
	menu->EnableMenuItem(ID_EDIT_UNPROTECT,MF_GRAYED);

	if (map.Unprotect())
	{
		if (map.GetStringCount() > 1024)
		{
			MessageBox(TEXT("The number of strings is more than 1024.\r\n")
				TEXT("Staredit won't open this map."),ProgName,MB_OK|MB_ICONWARNING);
		}
		Print_Message(TEXT("Done."));
		GetDlgItem(IDC_SAVE)->EnableWindow(TRUE);
		menu->EnableMenuItem(ID_FILE_SAVE1,MF_ENABLED);
	}
	else
	{
		Print_Message(TEXT("An error was occured. map is closed."));
		GetDlgItem(IDC_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_UNPROTECT)->EnableWindow(FALSE);
		menu->EnableMenuItem(ID_FILE_SAVE1,MF_GRAYED);
		menu->EnableMenuItem(ID_EDIT_UNPROTECT,MF_GRAYED);
	}
}

BOOL CUnusedUnprotector2Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) return true;
	else if (pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)return true;
	else return CDialog::PreTranslateMessage(pMsg);
}

void CUnusedUnprotector2Dlg::OnEditUnprotect() 
{
	// TODO: Add your command handler code here
	OnUnprotect();
}

void CUnusedUnprotector2Dlg::OnFileExit() 
{
	// TODO: Add your command handler code here
	EndDialog(IDCANCEL);
}

void CUnusedUnprotector2Dlg::OnFileOpen1() 
{
	// TODO: Add your command handler code here
	OnOpen();
}

void CUnusedUnprotector2Dlg::OnFileSave1() 
{
	// TODO: Add your command handler code here
	OnSave();
}

void CUnusedUnprotector2Dlg::OnHelpAbout() 
{
	// TODO: Add your command handler code here
	static int count=0,opened=0;
	if (opened==0 && ++count == 10)
	{
		MessageBox(TEXT("나 좀 그만 띄워"),TEXT("About"),MB_OK|MB_ICONERROR);
		opened=1;
	}
	CAboutDlg cad;
	cad.DoModal();
}

BOOL CUnusedUnprotector2Dlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CAboutDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnHelpInfo(pHelpInfo);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CWnd *pWnd;
	CString str,str2;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	pWnd=GetDlgItem(IDC_STATIC1);

	pWnd->GetWindowText(str);
	str2.Format(str,(LPCTSTR)ProgName);
	pWnd->SetWindowText(str2);

	GetDlgItem(IDC_STATIC5)->SetWindowText("결국 여기까지 왔군요. 보다시피 마지막 버전입니다. UU2 개발하느라 귀찮기도 하고 어떤 기능을 넣을까 고민하기도 하고 디버깅하느라 휴일하루를 날려버린 적도 있지만 이제 다 끝이군요.\r\n\r\n소스를 버그신고게시판에 업로드하였습니다.제가 초보라서 소스가 상당히 지저분하지만 (딴에 클래스화시키겠다고 억지로 짜맞추거나 유니코드 지원하겠다고 발악한거나 지금 보면 다 헛짓이군요) 그래도 참고하실 분은 있겠지요.\r\n\r\n아쉽긴 하지만 후회는 없습니다. 지금껏 테스트를 거의 도맡아 주신 rpgkdh님이나 저에게 조언을 아끼지 않는 sarami형과 legacyweapon씨에게 다시 감사의 말을 드립니다. 글은 이만 줄이겠습니다. 그럼 모두들 안녕히 계십시오.\r\n\r\nPS: 어떤 일이 있어도 더 이상 업데이트는 하지 않습니다.");

	SetIcon(hIcon, TRUE);		// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUnusedUnprotector2Dlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg a;
	a.DoModal();
}

void CUnusedUnprotector2Dlg::OnBugreport() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open","http://unused.dothome.co.kr/bbs/zboard.php?id=uu2sug",NULL,NULL,SW_SHOW);
}

void CUnusedUnprotector2Dlg::OnHelpBugreport() 
{
	// TODO: Add your command handler code here
	OnBugreport();
}

void CUnusedUnprotector2Dlg::OnEditConfig() 
{
	// TODO: Add your command handler code here
	OnConfig();
}

void CUnusedUnprotector2Dlg::OnConfig() 
{
	// TODO: Add your control notification handler code here
	CConfigureDialog cfd;
	cfd.DoModal();
}

void CUnusedUnprotector2Dlg::OnUpdate()
{
	OnAbout();
}