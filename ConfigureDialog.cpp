// ConfigureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Unused Unprotector 2.h"
#include "ConfigureDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HICON hIcon;
extern CUnprotectMap map;
extern HMODULE hstorm;
extern bool canusestorm;

/////////////////////////////////////////////////////////////////////////////
// CConfigureDialog dialog


CConfigureDialog::CConfigureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigureDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigureDialog)
	//}}AFX_DATA_INIT
}


void CConfigureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigureDialog)
	DDX_Control(pDX, IDC_ALLOWOVERWRITE, m_allow);
	DDX_Control(pDX, IDC_USESTORM, m_storm);
	DDX_Control(pDX, IDC_DELETEDISABLED, m_disabled);
	DDX_Control(pDX, IDC_COMMENT, m_comment);
	DDX_Control(pDX, IDC_NEVER, m_never);
	DDX_Control(pDX, IDC_REWRITE, m_rewrite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigureDialog, CDialog)
	//{{AFX_MSG_MAP(CConfigureDialog)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigureDialog message handlers

BOOL CConfigureDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (map.GetOptionRef(0)) m_rewrite.SetCheck(BST_CHECKED);
	if (map.GetOptionRef(1)) m_comment.SetCheck(BST_CHECKED);
	if (map.GetOptionRef(2)) m_never.SetCheck(BST_CHECKED);
	if (map.GetOptionRef(3)) m_disabled.SetCheck(BST_CHECKED);
	if (map.GetOptionRef(4)) m_storm.SetCheck(BST_CHECKED);
	else if(!canusestorm) m_storm.EnableWindow(FALSE);
	if (map.GetOptionRef(5)) m_allow.SetCheck(BST_CHECKED);

	switch (map.GetOptionRef(6))
	{
	case -1:CheckRadioButton(IDC_RADIO2,IDC_RADIO5,IDC_RADIO2);break;
	case MAWA_QUALITY_LOW:CheckRadioButton(IDC_RADIO2,IDC_RADIO5,IDC_RADIO3);break;
	case MAWA_QUALITY_MEDIUM:CheckRadioButton(IDC_RADIO2,IDC_RADIO5,IDC_RADIO4);break;
	case MAWA_QUALITY_HIGH:CheckRadioButton(IDC_RADIO2,IDC_RADIO5,IDC_RADIO5);break;
	}

	SetIcon(hIcon, TRUE);
	SetIcon(hIcon, FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConfigureDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CDialog::OnHelpInfo(pHelpInfo);
}

void CConfigureDialog::OnOK() 
{
	// TODO: Add extra validation here
	switch (GetCheckedRadioButton(IDC_RADIO2,IDC_RADIO5))
	{
	case IDC_RADIO2:map.GetOptionRef(6)=-1;break;
	case IDC_RADIO3:map.GetOptionRef(6)=MAWA_QUALITY_LOW;break;
	case IDC_RADIO4:map.GetOptionRef(6)=MAWA_QUALITY_MEDIUM;break;
	case IDC_RADIO5:map.GetOptionRef(6)=MAWA_QUALITY_HIGH;break;
	}

	map.GetOptionRef(0)=(m_rewrite.GetCheck()==BST_CHECKED);
	map.GetOptionRef(1)=(m_comment.GetCheck()==BST_CHECKED);
	map.GetOptionRef(2)=(m_never.GetCheck()==BST_CHECKED);
	map.GetOptionRef(3)=(m_disabled.GetCheck()==BST_CHECKED);
	map.ChangeFunctionSet(map.GetOptionRef(4)=(m_storm.GetCheck()==BST_CHECKED),hstorm);
	map.GetOptionRef(5)=(m_allow.GetCheck()==BST_CHECKED);
	CDialog::OnOK();
}
