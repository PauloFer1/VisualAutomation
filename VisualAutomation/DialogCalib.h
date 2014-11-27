#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"


// DialogCalib dialog

class DialogCalib : public CDialogEx
{
	DECLARE_DYNAMIC(DialogCalib)

public:
	DialogCalib(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogCalib();
	void initVision();
	CImage *m_img;
	CStatic s_img;

// Dialog Data
	enum { IDD = IDD_DIALOG_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedCalibImg();
	afx_msg void OnClose();
	CStatic imageViewer;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCustomdrawSliderBlur(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl sliderBlur;
	CSliderCtrl sliderBright;
	CSliderCtrl sliderExp;
	CSliderCtrl sliderThres;
	afx_msg void OnNMCustomdrawSliderThre(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderBrigh(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderExp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic cannyViewer;
	afx_msg void OnBnClickedButtonCannyViewer();
	afx_msg void OnBnClickedButtonProcImgViewer();
	afx_msg void OnBnClickedMfccolorbutton1();
	CMFCColorButton colorPick;
	CSliderCtrl sliderContrast;
	afx_msg void OnNMCustomdrawSliderContrast2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl sliderZoom;
	afx_msg void OnNMCustomdrawSliderThreshold(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl sliderThreshold;
//	CButton typeThreshold;
//	int typeThreshold;
//	bool typeThreshold;
	afx_msg void OnBnClickedRadio1();
	int typeThreshold;
	afx_msg void OnBnClickedRadio2();
//	int thresholdFunction;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
//	int checkAdaptive;
	afx_msg void OnBnClickedCheck1();
	CButton checkAdaptive;
	CButton useThreshCheckl;
	afx_msg void OnBnClickedCheck2();
};