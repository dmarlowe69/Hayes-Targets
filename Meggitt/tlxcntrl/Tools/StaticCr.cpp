/*$Log:   N:/SW/Tools/Vcpp/Vcs/Controls/StaticCr.cpv  $
/* 
/*    Rev 1.3   Aug 23 1999 15:21:00   Smith
/* Use m_brBkgnd.DeleteObject(), added SetColorsSys().
/* 
/*    Rev 1.2   Jul 07 1999 16:23:08   Smith
/* Added LoadWindowText to put text with resource ID.
/* 
/*    Rev 1.1   Jul 06 1999 10:14:24   Smith
/* Allow colors to be changed.
/* 
/*    Rev 1.0   May 19 1999 08:36:28   Smith
/* Initial Revision.
*/
//===========================================================================
// FILENAME  : StaticCr.cpp
//
// PROJECT   : General tool for controls
//
// PURPOSE   : Provide a static control with support for color
//
// NOTES     : Default is Black text and White background
//===========================================================================
#include "stdafx.h"
#include "StaticCr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//---------------------------------------------------------------------------
// FUNCTION  : CStaticColor
//
// PURPOSE   :	Constructor for static with color
//
// INPUTS    : crText, crBkgnd
//
// OUTPUTS   : m_crText,m_crBkgnd,m_brBkgnd
//
// RETURN    : None
//---------------------------------------------------------------------------
CStaticColor::CStaticColor(const COLORREF crText, const COLORREF crBkgnd)
{
	m_crText = crText;
	m_crBkgnd = crBkgnd;
	m_brBkgnd.CreateSolidBrush(crBkgnd);

}	// CStaticColor

//---------------------------------------------------------------------------
// FUNCTION  : ~CStaticColor
//
// PURPOSE   :	Destructor for static with color
//
// INPUTS    : None
//
// OUTPUTS   : m_brBkgnd
//
// RETURN    : None
//---------------------------------------------------------------------------
CStaticColor::~CStaticColor()
{
	m_brBkgnd.DeleteObject();

}	// ~CStaticColor


BEGIN_MESSAGE_MAP(CStaticColor, CStatic)
	//{{AFX_MSG_MAP(CStaticColor)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//===========================================================================
// CStaticColor message handlers
//===========================================================================

//---------------------------------------------------------------------------
// FUNCTION  : CtrlColor
//
// PURPOSE   :	Assign colors for the static control.
//
// INPUTS    : pDC, m_crText,m_crBkgnd,m_brBkgnd
//
// OUTPUTS   : Text and Background Color
//
// RETURN    : m_brBkgnd
//---------------------------------------------------------------------------
HBRUSH CStaticColor::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Change color attributes of the DC
	pDC->SetTextColor(m_crText);
	pDC->SetBkColor(m_crBkgnd);
	
	// Return a non-NULL brush, the parent's handler should not be called
	return (HBRUSH)m_brBkgnd;

}	// CtlColor


//===========================================================================
// Member functions
//===========================================================================

//---------------------------------------------------------------------------
// FUNCTION  : SetColor..
//
// PURPOSE   :	Assign text and background color of the static control.
//
// INPUTS    : uID
//
// OUTPUTS   : Text on the static control
//
// RETURN    : void
//---------------------------------------------------------------------------

// Assign text color
void CStaticColor::SetColorText(COLORREF crColor)
{
	m_crText = crColor;

	Invalidate();

}	// SetColorText

// Assign background color
void CStaticColor::SetColorBkgnd(COLORREF crColor)
{
	m_crBkgnd = crColor;

	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crBkgnd);

	Invalidate();

}	// SetColorBkgnd

// Assign text and background color
void CStaticColor::SetColors(COLORREF crText, COLORREF crBkgnd)
{
	m_crText = crText;
	m_crBkgnd = crBkgnd;
	
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crBkgnd);

	Invalidate();

}	// SetColors

// Assign default system colors
void CStaticColor::SetColorsSys()
{
	SetColors(::GetSysColor(COLOR_BTNTEXT),::GetSysColor(COLOR_BTNFACE));

}	// SetColorsSys


//---------------------------------------------------------------------------
// FUNCTION  : SetWindowText
//
// PURPOSE   :	Use a resource ID to assign text to the static control.
//
// INPUTS    : uID
//
// OUTPUTS   : Text on the static control
//
// RETURN    : void
//---------------------------------------------------------------------------
BOOL CStaticColor::LoadWindowText(const UINT uID)
{
	CString str;
	if (!str.LoadString(uID))
		return FALSE;

	SetWindowText(str);
	return TRUE;

}	// SetWindowText
