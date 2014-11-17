// DialogCalib.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "DialogCalib.h"
#include "afxdialogex.h"
#include "Detection.h"
#include "Camera.h"


// DialogCalib dialog

IMPLEMENT_DYNAMIC(DialogCalib, CDialogEx)

DialogCalib::DialogCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogCalib::IDD, pParent)
{

}
void DialogCalib::initVision()
{
	//img = (CStatic *)GetDlgItem(IDC_CALIB_IMG);
	if (Camera::getInstance()->hasCamera == false)
		Camera::getInstance()->initCamera();
	Detection::getInstance()->imgWnd = &imageViewer;
	Detection::getInstance()->cannyWnd = &cannyViewer;
}
DialogCalib::~DialogCalib()
{
}

void DialogCalib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CALIB_IMG, imageViewer);
	DDX_Control(pDX, IDC_SLIDER_BLUR, sliderBlur);
	DDX_Control(pDX, IDC_SLIDER_BRIGH, sliderBright);
	DDX_Control(pDX, IDC_SLIDER_EXP, sliderExp);
	DDX_Control(pDX, IDC_SLIDER_THRE, sliderThres);

	sliderBlur.SetRange(0, 10, TRUE);
	sliderThres.SetRange(0, 255, TRUE);
	sliderBright.SetRange(0, 255, TRUE);
	sliderExp.SetRange(-10, 20, TRUE);
	DDX_Control(pDX, IDC_STATIC_CANNY, cannyViewer);
}


BEGIN_MESSAGE_MAP(DialogCalib, CDialogEx)
	ON_BN_CLICKED(IDOK, &DialogCalib::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DialogCalib::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_CALIB_IMG, &DialogCalib::OnStnClickedCalibImg)
	ON_WM_SYSCOMMAND()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BLUR, &DialogCalib::OnCustomdrawSliderBlur)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_THRE, &DialogCalib::OnNMCustomdrawSliderThre)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BRIGH, &DialogCalib::OnNMCustomdrawSliderBrigh)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EXP, &DialogCalib::OnNMCustomdrawSliderExp)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// DialogCalib message handlers


void DialogCalib::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	Detection::getInstance()->destroyWnd();
	Sleep(50);
	CDialogEx::OnOK();
}


void DialogCalib::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	Detection::getInstance()->destroyWnd();
	Sleep(50);
	CDialogEx::OnCancel();
}



void DialogCalib::OnStnClickedCalibImg()
{
	// TODO: Add your control notification handler code here
}
void DialogCalib::OnClose()
{
	AfxMessageBox(_T("Wnd is going to close!"));
}


void DialogCalib::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	
	if (lParam==SC_CLOSE)
		AfxMessageBox(_T("Close"));

	CDialogEx::OnSysCommand(nID, lParam);
}



void DialogCalib::OnCustomdrawSliderBlur(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DialogCalib::OnNMCustomdrawSliderThre(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DialogCalib::OnNMCustomdrawSliderBrigh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DialogCalib::OnNMCustomdrawSliderExp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void DialogCalib::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	int value = pSlider->GetPos();

	if (pSlider==&sliderBlur)
	{
		Detection::getInstance()->setBlur(value);
	}
	else if (pSlider == &sliderThres)
	{
		Detection::getInstance()->setThreshold(value);
	}
	else if (pSlider == &sliderExp)
	{
		Camera::getInstance()->setExposure(value);
	}
	else if(pSlider == &sliderBright)
	{
		Camera::getInstance()->setBrightness(value);
	}
	CString str;
	str.Format(_T("val: %d"), value);
	//AfxMessageBox(str);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
