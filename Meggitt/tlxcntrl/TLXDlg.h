// TLXDlg.h : header file
//

#if !defined(AFX_TLXDLG_H__403D3847_8578_11D3_8433_EB1F7702B417__INCLUDED_)
#define AFX_TLXDLG_H__403D3847_8578_11D3_8433_EB1F7702B417__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StaticCr.h"

/////////////////////////////////////////////////////////////////////////////
// CTLXDlg dialog

class CTLXDlg : public CDialog
{
// Construction
public:
	CTLXDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	CFont		m_fontBold;

	//{{AFX_DATA(CTLXDlg)
	enum { IDD = IDD_TLXCTRL_DIALOG };
	int				m_nHeight;
	CStaticColor	m_stcLampVolts;
	CStaticColor	m_stcPumpVolts;
	CStaticColor	m_stcBattVolts;
	CStaticColor	m_stcBurnerStat;
	CStaticColor	m_stcSquibNum;
	CSliderCtrl		m_sldHeight;
	CSliderCtrl		m_sldAltitude;
	CSliderCtrl		m_sldAccel;
	CSliderCtrl		m_sldWingAng;
	CProgressCtrl	m_progSignal;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTLXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTLXDlg)
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	afx_msg void OnCheckBurnerOn();
	afx_msg void OnCheckFlyMode();
	afx_msg void OnCheckLampOn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int HgtToSlide(const int nHGT);
	int AltToSlide(const int nALT);
	void InitTLXDialog();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TLXDLG_H__403D3847_8578_11D3_8433_EB1F7702B417__INCLUDED_)
