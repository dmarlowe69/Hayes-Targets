/*$Log:   N:/SW/Tools/Vcpp/Vcs/Controls/StaticCr.h_v  $
/*
/*   Rev 1.3   Aug 23 1999 15:21:00   Smith
/*Use m_brBkgnd.DeleteObject(), added SetColorsSys().
/*
/*   Rev 1.2   Jul 07 1999 16:23:08   Smith
/*Added LoadWindowText to put text with resource ID.
/*
/*   Rev 1.1   Jul 06 1999 10:14:24   Smith
/*Allow colors to be changed.
/*
/*   Rev 1.0   May 19 1999 08:36:32   Smith
/*Initial Revision.
*/
//===========================================================================
// FILENAME  : StaticCr.h
//
// PROJECT   : General tool for controls
//
// PURPOSE   : Provide a static control with support for color
//
// NOTES     : Default is black text and white background
//					Suggested color constants
//						const COLORREF crBLACK = RGB(0,0,0);
//						const COLORREF crWHITE = RGB(255,255,255);
//						const COLORREF crGRAY  = RGB(192,192,192);	// Background
//						const COLORREF crRED   = RGB(255,0,0);			// Error
//						const COLORREF crGREEN = RGB(0,128,0);			// Ok
//						const COLORREF crBLUE  = RGB(0,0,128);
//===========================================================================
#if !defined(STATICCR_H__INCLUDED)
#define STATICCR_H__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------------------------------
// CStaticColor
//---------------------------------------------------------------------------
class CStaticColor : public CStatic
{
public:
	// Constructor
	CStaticColor ( const COLORREF crText = RGB(0,0,0),
						const COLORREF crBkgnd = RGB(255,255,255) );
	// Destructor
	virtual ~CStaticColor();

// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticColor)
	//}}AFX_VIRTUAL

	// Implementation
public:
	// Assign colors for the static control
	virtual void SetColorText(COLORREF crColor);
	virtual void SetColorBkgnd(COLORREF crColor);
	virtual void SetColors( COLORREF crText = RGB(0,0,0),
									COLORREF crBkgnd= RGB(255,255,255) );
	virtual void SetColorsSys();

	// Use a resource ID to assign text to the static control
	BOOL LoadWindowText(const UINT uID);

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticColor)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

// Member data
public:
	COLORREF	m_crText;					// Text color
	COLORREF	m_crBkgnd;					// Background color
	CBrush	m_brBkgnd;					// Brush for background color
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(STATICCR_H__INCLUDED)
