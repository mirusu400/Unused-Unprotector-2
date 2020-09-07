// Unused Unprotector 2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Unused Unprotector 2.h"
#include "Unused Unprotector 2Dlg.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HICON hIcon;
CString ProgName;
extern CUnprotectMap map;
HMODULE hstorm;
bool canusestorm=true;

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2App

BEGIN_MESSAGE_MAP(CUnusedUnprotector2App, CWinApp)
	//{{AFX_MSG_MAP(CUnusedUnprotector2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2App construction

CUnusedUnprotector2App::CUnusedUnprotector2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUnusedUnprotector2App object

CUnusedUnprotector2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2App initialization

BOOL CUnusedUnprotector2App::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	HRSRC hRSrc;
	HGLOBAL hMem;
	PVOID ptr;
	CString storm,temppath,temppath2;
	int i;
	ProgName.LoadString(IDS_PROGRAM_NAME);
	HANDLE hMutex=CreateMutex(NULL,FALSE,TEXT("__UU2MutexObjectName__"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,TEXT("Another instance is already running."),ProgName,MB_OK|MB_ICONERROR|MB_TASKMODAL);
		return 0;
	}

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	AfxEnableControlContainer();

	hRSrc = FindResource(m_hInstance, MAKEINTRESOURCE(IDR_VCOD), TEXT("VCOD"));
	hMem = LoadResource(m_hInstance, hRSrc);
	ptr = LockResource(hMem);
	map.SetVCOD(ptr);

	hRSrc = FindResource(m_hInstance, MAKEINTRESOURCE(IDR_STORM1), TEXT("STORM"));
	hMem = LoadResource(m_hInstance, hRSrc);
	ptr = LockResource(hMem);

	GetTempPath(MAX_PATH,temppath.GetBuffer(MAX_PATH));
	CFile file;
	for (i=0; i<=0xFF; i++)
	{
		storm.Format(TEXT("USTF%02x.dll"),i);
		_tmakepath(temppath2.GetBuffer(MAX_PATH),NULL,temppath,storm,NULL);
		DeleteFile(temppath2);
		if (file.Open(temppath2,CFile::modeWrite|CFile::modeCreate))
		{
			file.Write(ptr,SizeofResource(m_hInstance,hRSrc));
			file.Close();
			if (hstorm=LoadLibrary(temppath2))
			{
				if (CUnprotectMap::Initialize(hstorm))
				{
					map.GetOptionRef(4)=true;
					goto out;
				}
			}
			DeleteFile(temppath2);
		}
	}
	MessageBox(0,TEXT("Loading Storm Library failed.\r\n")
		TEXT("\"Use Storm when opening\" option will be disabled."),ProgName,MB_OK|MB_ICONWARNING|MB_TASKMODAL);
	canusestorm=false;
	map.GetOptionRef(4)=false;


out:
	CUnusedUnprotector2Dlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	FreeLibrary(hstorm);
	if (canusestorm) DeleteFile(temppath2);
	DeleteObject(hMutex);

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
