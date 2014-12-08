// DialogCalib.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "DialogCalib.h"
#include "afxdialogex.h"
#include "Detection.h"
#include "Camera.h"
#include "Constants.h"
#include "XMLLoader.h"


// DialogCalib dialog

IMPLEMENT_DYNAMIC(DialogCalib, CDialogEx)

DialogCalib::DialogCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(DialogCalib::IDD, pParent)
	, typeThreshold(0)
//	, thresholdFunction(0)
{
}
void DialogCalib::initVision()
{
	//img = (CStatic *)GetDlgItem(IDC_CALIB_IMG);
	if (Camera::getInstance()->hasCamera == false)
	{
		Camera::getInstance()->initCamera();
		Camera::getInstance()->setWndDisplay(&imageViewer);
	}
	Detection::getInstance()->imgWnd = &imageViewer;
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
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, sliderContrast);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, sliderZoom);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, sliderThreshold);
	DDX_Radio(pDX, IDC_RADIO1, typeThreshold);
	DDV_MinMaxInt(pDX, typeThreshold, 0, 1);
	//  DDX_Radio(pDX, IDC_RADIO3, thresholdFunction);
	//  DDV_MinMaxInt(pDX, thresholdFunction, 0, 1);
	DDX_Control(pDX, IDC_CHECK1, checkAdaptive);
	DDX_Control(pDX, IDC_CHECK2, useThreshCheckl);
	DDX_Control(pDX, IDC_EDIT1, objWidthInput);
	DDX_Control(pDX, IDC_EDIT2, objHeightInput);

	sliderBlur.SetRange(0, 10, TRUE);
	sliderThres.SetRange(0, 255, TRUE);
	sliderBright.SetRange(0, 255, TRUE);
	sliderExp.SetRange(-10, 20, TRUE);
	sliderContrast.SetRange(0, 10, TRUE);
	sliderZoom.SetRange(0, 100, TRUE);
	sliderThreshold.SetRange(0, 255, TRUE);
	checkAdaptive.SetCheck(1);

	CString w;
	w.Format(_T("%d"), Constants::getInstance()->getObjWidth());
	CString h;
	h.Format(_T("%d"), Constants::getInstance()->getObjHeight());
	objWidthInput.SetWindowTextW(w);
	objHeightInput.SetWindowTextW(h);

	//  DDX_Check(pDX, IDC_CHECK1, checkAdaptive);
	//  DDV_MinMaxInt(pDX, checkAdaptive, 0, 1);

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
	ON_BN_CLICKED(IDC_BUTTON_CANNY_VIEWER, &DialogCalib::OnBnClickedButtonCannyViewer)
	ON_BN_CLICKED(IDC_BUTTON_PROC_IMG_VIEWER, &DialogCalib::OnBnClickedButtonProcImgViewer)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &DialogCalib::OnBnClickedMfccolorbutton1)
	ON_BN_CLICKED(IDC_RADIO1, &DialogCalib::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &DialogCalib::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK1, &DialogCalib::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &DialogCalib::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &DialogCalib::OnBnClickedButton1)
END_MESSAGE_MAP()


// DialogCalib message handlers


void DialogCalib::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	Detection::getInstance()->destroyWnd();
	Sleep(50);
	XMLLoader::getInstance()->saveXML();
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
		Constants::getInstance()->setBlur(value);
	}
	else if (pSlider == &sliderThres)
	{
		Detection::getInstance()->setThreshold(value);
		Constants::getInstance()->setCanny(value);
	}
	else if (pSlider == &sliderExp)
	{
		Camera::getInstance()->setExposure(value);
		Constants::getInstance()->setExposure(value);
	}
	else if(pSlider == &sliderBright)
	{
		Camera::getInstance()->setBrightness(value);
		Constants::getInstance()->setBright(value);
	}
	else if (pSlider == &sliderContrast)
	{                        
		Camera::getInstance()->setConstrast(value);
		Constants::getInstance()->setConstrast(value);
	}
	else if (pSlider == &sliderZoom)
	{
		Camera::getInstance()->setZoom(value);
		Constants::getInstance()->setZoom(value);
	}
	else if (pSlider == &sliderThreshold)
	{
		Detection::getInstance()->setThresholdValue(value);
		Constants::getInstance()->setThresholdVal(value);
	}
	CString str;
	str.Format(_T("val: %d"), value);
	//AfxMessageBox(str);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void DialogCalib::OnBnClickedButtonCannyViewer()
{
	Detection::getInstance()->showCanny();
}


void DialogCalib::OnBnClickedButtonProcImgViewer()
{
	Detection::getInstance()->showProcImage();
}


void DialogCalib::OnBnClickedMfccolorbutton1()
{
	// TODO: Add your control notification handler code here
	COLORREF color = colorPick.GetColor();
	int r = (GetRValue(color));
	int b = GetBValue(color);
	int g = GetGValue(color);
	Detection::getInstance()->setFilterColor(r, g, b);

}





void DialogCalib::OnBnClickedRadio1()
{
	CString str;
	str.Format(_T("val: %d"), typeThreshold);
	AfxMessageBox(str);
	if (typeThreshold==0)
		Detection::getInstance()->typeThreshold = ADAPTIVE_THRESH_GAUSSIAN_C;
	else
		Detection::getInstance()->typeThreshold = ADAPTIVE_THRESH_MEAN_C;
}


void DialogCalib::OnBnClickedRadio2()
{
	CString str;
	str.Format(_T("val: %d"),typeThreshold);
	AfxMessageBox(str);
	if (typeThreshold == 0)
		Detection::getInstance()->typeThreshold = ADAPTIVE_THRESH_GAUSSIAN_C;
	else
	{
		Detection::getInstance()->typeThreshold = ADAPTIVE_THRESH_MEAN_C;
		
	}
}





void DialogCalib::OnBnClickedCheck1()
{
	CString str;
	str.Format(_T("val: %d"), checkAdaptive.GetCheck());
//	AfxMessageBox(str);
	if (checkAdaptive.GetCheck())
		Detection::getInstance()->thresholdFunction = 0;
	else
		Detection::getInstance()->thresholdFunction = 1;
}


void DialogCalib::OnBnClickedCheck2()
{
	CString str;
	str.Format(_T("val: %d"), useThreshCheckl.GetCheck());
		//AfxMessageBox(str);
	if (useThreshCheckl.GetCheck())
		Detection::getInstance()->useThreshold = true;
	else
		Detection::getInstance()->useThreshold = false;
}


void DialogCalib::OnBnClickedButton1()
{
	CString width;
	CString height;
	objWidthInput.GetWindowTextW(width);
	objHeightInput.GetWindowTextW(height);

	int w = _wtoi(width);
	int h = _wtoi(height);
	Constants::getInstance()->setObjWidth(w);
	Constants::getInstance()->setObjHeight(h);

	AfxMessageBox(_T("WIDTH & HEIGHT CHANGED"), MB_OK|MB_ICONINFORMATION);
}
