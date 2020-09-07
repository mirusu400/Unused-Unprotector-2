// Unused Unprotector 2.h : main header file for the UNUSED UNPROTECTOR 2 application
//

#if !defined(AFX_UNUSEDUNPROTECTOR2_H__3F36EF57_053D_4A7A_8DF9_1A5109FA50FB__INCLUDED_)
#define AFX_UNUSEDUNPROTECTOR2_H__3F36EF57_053D_4A7A_8DF9_1A5109FA50FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnusedUnprotector2App:
// See Unused Unprotector 2.cpp for the implementation of this class
//

class CUnusedUnprotector2App : public CWinApp
{
public:
	CUnusedUnprotector2App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnusedUnprotector2App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnusedUnprotector2App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNUSEDUNPROTECTOR2_H__3F36EF57_053D_4A7A_8DF9_1A5109FA50FB__INCLUDED_)
