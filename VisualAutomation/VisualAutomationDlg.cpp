
// VisualAutomationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "VisualAutomationDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"
#include "DialogConn.h"
#include "DialogCalib.h"
#include "Detection.h"
#include "DialogVision.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool hasCamera;
};
DialogCalib m_calib;

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVisualAutomationDlg dialog


IMPLEMENT_DYNAMIC(CVisualAutomationDlg, CDialogEx);

CVisualAutomationDlg::CVisualAutomationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVisualAutomationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}

CVisualAutomationDlg::~CVisualAutomationDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CVisualAutomationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVisualAutomationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CVisualAutomationDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CVisualAutomationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON2, &CVisualAutomationDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVisualAutomationDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDCANCEL, &CVisualAutomationDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CVisualAutomationDlg message handlers

BOOL CVisualAutomationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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


	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVisualAutomationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVisualAutomationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVisualAutomationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CVisualAutomationDlg::OnClose()
{
	if (CanExit())
	{
		if (Camera::getInstance()->hasCamera)
			Camera::getInstance()->closeCamera();
		CDialogEx::OnClose();
	}
}

void CVisualAutomationDlg::OnOK()
{
	if (CanExit())
	{
		if (Camera::getInstance()->hasCamera)
			Camera::getInstance()->closeCamera();
		CDialogEx::OnOK();
	}
}

void CVisualAutomationDlg::OnCancel()
{
	if (CanExit())
	{
		if (Camera::getInstance()->hasCamera)
			Camera::getInstance()->closeCamera();
		CDialogEx::OnCancel();
	}
}

BOOL CVisualAutomationDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}



void CVisualAutomationDlg::OnBnClickedButton1()
{
	CDialogConn m_conn;
	m_conn.DoModal();
}


void CVisualAutomationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CVisualAutomationDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	Detection *d;
	d = Detection::getInstance();
	//if (!Detection::getInstance()->hasCam)
	//	d->initCam();
	m_calib.initVision();
	d->showWnd();
	m_calib.DoModal();
//	m_calib.ShowWindow(WS_MAXIMIZEBOX);
	
}


void CVisualAutomationDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	CDialogVision m_vision;
	Detection::getInstance()->showWnd();
	Detection::getInstance()->startVision();
	m_vision.initVision();
	m_vision.DoModal();
}


void CVisualAutomationDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
