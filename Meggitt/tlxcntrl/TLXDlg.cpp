//===========================================================================
// FILENAME  : TLXDlg.cpp
//
// PROJECT   : TLX Flight Controler
//
// PURPOSE   : Main dialog for TLX Controler application.
//
// GROUPS    : About box dialog
//					Application dialog
//					Message handlers
//===========================================================================
#include "stdafx.h"
#include "TLXCtrl.h"
#include "TLXDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//===========================================================================
// CAboutDlg dialog used for App About
//===========================================================================
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//===========================================================================
// Color constants
//===========================================================================
const COLORREF crBLACK = RGB(0,0,0);
const COLORREF crWHITE = RGB(255,255,255);
const COLORREF crGRAY  = RGB(192,192,192);	// Background
const COLORREF crRED   = RGB(255,0,0);			// Error
const COLORREF crGREEN = RGB(0,128,0);			// Ok
const COLORREF crBLUE  = RGB(0,0,128);

//===========================================================================
// CTLXDlg dialog
//===========================================================================
CTLXDlg::CTLXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTLXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTLXDlg)
	m_nHeight = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTLXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTLXDlg)
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 20, 500);
	DDX_Control(pDX, IDC_STATIC_LAMPVOLTS, m_stcLampVolts);
	DDX_Control(pDX, IDC_STATIC_PUMPVOLTS, m_stcPumpVolts);
	DDX_Control(pDX, IDC_STATIC_BATTVOLTS, m_stcBattVolts);
	DDX_Control(pDX, IDC_STATIC_BURNERSTAT,m_stcBurnerStat);
	DDX_Control(pDX, IDC_STATIC_SQUIBSNUM, m_stcSquibNum);
	DDX_Control(pDX, IDC_SLIDER_HEIGHT,    m_sldHeight);
	DDX_Control(pDX, IDC_SLIDER_ALTITUDE,  m_sldAltitude);
	DDX_Control(pDX, IDC_SLIDER_ACCEL,     m_sldAccel);
	DDX_Control(pDX, IDC_SLIDER_WINGANG,   m_sldWingAng);
	DDX_Control(pDX, IDC_PROGRESS_SIGNAL,  m_progSignal);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTLXDlg, CDialog)
	//{{AFX_MSG_MAP(CTLXDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_LAMPON, OnCheckLampOn)
	ON_BN_CLICKED(IDC_CHECK_BURNERON, OnCheckBurnerOn)
	ON_BN_CLICKED(IDC_CHECK_FLYMODE, OnCheckFlyMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int CTLXDlg::HgtToSlide(const int nHGT)
{
	int nPos;

	if (nHGT < 20)
		nPos = 1;
	else if (nHGT > 500)
		nPos = 100;
	else															// 0.5 added for rounding
		nPos = int((70.81849 * log10(double(nHGT))) - 90.63698);

	return (100 - nPos) + 1;

}	// HgtToSlide

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int CTLXDlg::AltToSlide(const int nALT)
{
	int nPos;
	if (nALT < 1)
		nPos = 1;
	else if (nALT > 500)
		nPos = 100;
	else													// 0.5 added for rounding
		nPos = int((33.0 * log10(double(nALT))) + 1.5);

	return (100 - nPos) + 1;

}	// AltToSlide

//===========================================================================
// CTLXDlg message handlers
//===========================================================================

//---------------------------------------------------------------------------
// FUNCTION  : InitTLXDialog
//
// PURPOSE   : Initialize TCX dialog data and controls.
//
// INPUTS    : g_TLXIni
//
// OUTPUTS   : Dialog Controls
//
// RETURN    : void
//---------------------------------------------------------------------------
void CTLXDlg::InitTLXDialog()
{
	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));	// Setup bold font
	(this->GetFont())->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_fontBold.CreateFontIndirect(&lf);

	m_stcLampVolts.SetFont(&m_fontBold);
	m_stcLampVolts.SetColors(crBLACK,crGRAY);
	m_stcPumpVolts.SetFont(&m_fontBold);
	m_stcPumpVolts.SetColors(crBLACK,crGRAY);
	m_stcBattVolts.SetFont(&m_fontBold);
	m_stcBattVolts.SetColors(crBLACK,crGRAY);
	m_stcBurnerStat.SetFont(&m_fontBold);
	m_stcBurnerStat.SetColors(crBLACK,crGRAY);
	m_stcSquibNum.SetFont(&m_fontBold);
	m_stcSquibNum.SetColors(crBLACK,crGRAY);
	((CStatic*)GetDlgItem(IDC_STATIC_SQUIBSTOTAL))->SetFont(&m_fontBold);
	((CEdit*)GetDlgItem(IDC_EDIT_HEIGHT))->SetFont(&m_fontBold);

	m_sldHeight.SetRange  (1,100);
	m_sldAltitude.SetRange(1,100);
	m_sldAccel.SetRange   (-5,5);
	m_sldWingAng.SetRange (-8,0);

	m_progSignal.SetRange(0,100);

	m_sldAccel.SetTic(0);

	m_sldHeight.SetTic(HgtToSlide(200));
	m_sldHeight.SetTic(HgtToSlide(100));
	m_sldHeight.SetTic(HgtToSlide(50));
	m_sldHeight.SetTic(HgtToSlide(30));

	m_sldAltitude.SetTic(AltToSlide(500));
	m_sldAltitude.SetTic(AltToSlide(200));
	m_sldAltitude.SetTic(AltToSlide(100));
	m_sldAltitude.SetTic(AltToSlide(50));
	m_sldAltitude.SetTic(AltToSlide(20));
	m_sldAltitude.SetTic(AltToSlide(10));

}	// InitTLXDialog

//---------------------------------------------------------------------------
// FUNCTION  : OnInitDialog
//
// PURPOSE   : Initialize dialog, system menu, and icons.
//
// INPUTS    : System
//
// OUTPUTS   : Dialog
//
// RETURN    : Focus not set flag
//---------------------------------------------------------------------------
BOOL CTLXDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize TLX dialog data and controls
	InitTLXDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

//---------------------------------------------------------------------------
// FUNCTION  : OnSysCommand
//
// PURPOSE   : Process system command for about box or dialog
//
// INPUTS    : nID and lParam
//
// OUTPUTS   : None
//
// RETURN    : void
//---------------------------------------------------------------------------
void CTLXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

//---------------------------------------------------------------------------
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
//---------------------------------------------------------------------------
void CTLXDlg::OnPaint() 
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
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//---------------------------------------------------------------------------
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
//---------------------------------------------------------------------------
HCURSOR CTLXDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//---------------------------------------------------------------------------
// FUNCTION  : OnCancel
//
// PURPOSE   : Prompt user before closing application from Esc.
//
// INPUTS    : g_bRecording
//
// OUTPUTS   :	Prompt
//
// RETURN    : void
//---------------------------------------------------------------------------
void CTLXDlg::OnCancel() 
{
/**	if(AfxMessageBox("Exit TLX Controler program?",
							MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
*/	{
		CDialog::OnCancel();			// Exit application
	}
}	// OnCancel

//---------------------------------------------------------------------------
// FUNCTION  : OnOK
//
// PURPOSE   : Handle closing from the [Enter] key.
//
// INPUTS    : None
//
// OUTPUTS   :	None
//
// RETURN    : void
//---------------------------------------------------------------------------
void CTLXDlg::OnOK() 
{
	MessageBeep(-1);
	// Maybe issue a tab key message
}	// OnOK

void CTLXDlg::OnCheckBurnerOn() 
{
	// TODO: Add your control notification handler code here
	
}

void CTLXDlg::OnCheckFlyMode() 
{
	// TODO: Add your control notification handler code here
	
}

void CTLXDlg::OnCheckLampOn() 
{
	// TODO: Add your control notification handler code here
	
}
