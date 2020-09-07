#if !defined(AFX_CONFIGUREDIALOG_H__781EF6C7_5CDB_4F74_A87F_F059631E29BF__INCLUDED_)
#define AFX_CONFIGUREDIALOG_H__781EF6C7_5CDB_4F74_A87F_F059631E29BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ConfigureDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigureDialog dialog

class CConfigureDialog : public CDialog
{
// Construction
public:
	CConfigureDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigureDialog)
	enum { IDD = IDD_CONFIGUREDIALOG_DIALOG };
	CButton	m_allow;
	CButton	m_storm;
	CButton	m_disabled;
	CButton	m_comment;
	CButton	m_never;
	CButton	m_rewrite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigureDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigureDialog)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGUREDIALOG_H__781EF6C7_5CDB_4F74_A87F_F059631E29BF__INCLUDED_)
