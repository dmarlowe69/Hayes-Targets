// TLXCtrl.h : main header file for the TLXCTRL application
//

#if !defined(AFX_TLXCTRL_H__403D3845_8578_11D3_8433_EB1F7702B417__INCLUDED_)
#define AFX_TLXCTRL_H__403D3845_8578_11D3_8433_EB1F7702B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTLXApp:
// See TLXCtrl.cpp for the implementation of this class
//

class CTLXApp : public CWinApp
{
public:
	CTLXApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTLXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTLXApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TLXCTRL_H__403D3845_8578_11D3_8433_EB1F7702B417__INCLUDED_)
