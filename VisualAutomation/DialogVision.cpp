// DialogVision.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "DialogVision.h"
#include "afxdialogex.h"


// CDialogVision dialog

IMPLEMENT_DYNAMIC(CDialogVision, CDialogEx)

CDialogVision::CDialogVision(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogVision::IDD, pParent)
{
}
CDialogVision::~CDialogVision()
{
}
void CDialogVision::initVision()
{
	if (Camera::getInstance()->hasCamera == false)
	{
		Camera::getInstance()->initCamera();
	}
	Detection::getInstance()->imgWnd = &imageViewer;
	__hook(&Detection::enableOut, Detection::getInstance(), &CDialogVision::setEnable);
}
void CDialogVision::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VALUE_X, labelValueX);
	labelValueX.SetWindowTextW(_T("45"));
	DDX_Control(pDX, IDC_VALUE_Y, labelValueY);
	DDX_Control(pDX, IDC_VALUE_ROT, labelValueRot);
	DDX_Control(pDX, IDC_VISION_IMG, imageViewer);
	DDX_Control(pDX, IDC_STATUS_TEXT, statusText);

	CFont m_Font1;
	LOGFONT logFont;
	//m_Font1.CreatePointFont(150, _T("Arial Bold"));
	m_Font1.CreateFontW(300, 300, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Arial Bold"));
	statusText.SetFont(&m_Font1); 
}


BEGIN_MESSAGE_MAP(CDialogVision, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogVision::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &CDialogVision::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogVision::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDialogVision message handlers


void CDialogVision::OnBnClickedButton1()
{
	Point2i offset= Detection::getInstance()->getOffset();
	CString str;
	str.Format(_T("%d mm"), offset.x);
	labelValueX.SetWindowTextW(str);
	str.Format(_T("%d mm"), offset.y);
	labelValueY.SetWindowTextW(str);
}


void CDialogVision::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	Detection::getInstance()->destroyWnd();
	Sleep(50);
	CDialogEx::OnCancel();
}
void CDialogVision::setEnable(int i){
	statusText.SetWindowTextW(_T("ENABLE OUT"));
}


void CDialogVision::OnBnClickedButton2()
{
	Detection::getInstance()->enableRead();
	statusText.SetWindowTextW(_T("ENABLE READ"));
}
