// Unused Unprotector 2Dlg.h : header file
//

#if !defined(AFX_UNUSEDUNPROTECTOR2DLG_H__E0910E46_6CF5_44D6_B5B4_D768AA1541EF__INCLUDED_)
#define AFX_UNUSEDUNPROTECTOR2DLG_H__E0910E46_6CF5_44D6_B5B4_D768AA1541EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2Dlg dialog

class CUnusedUnprotector2Dlg : public CDialog
{
// Construction
public:
	CUnusedUnprotector2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUnusedUnprotector2Dlg)
	enum { IDD = IDD_UNUSEDUNPROTECTOR2_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnusedUnprotector2Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_font;
	HACCEL m_hAccel;
public:
	void Print_Message(LPCTSTR lpszText);
protected:

	// Generated message map functions
	//{{AFX_MSG(CUnusedUnprotector2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOpen();
	afx_msg void OnSave();
	afx_msg void OnUnprotect();
	afx_msg void OnEditUnprotect();
	afx_msg void OnFileExit();
	afx_msg void OnFileOpen1();
	afx_msg void OnFileSave1();
	afx_msg void OnHelpAbout();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnAbout();
	afx_msg void OnBugreport();
	afx_msg void OnHelpBugreport();
	afx_msg void OnEditConfig();
	afx_msg void OnConfig();
	afx_msg void OnHelpNew();
	afx_msg void OnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNUSEDUNPROTECTOR2DLG_H__E0910E46_6CF5_44D6_B5B4_D768AA1541EF__INCLUDED_)
