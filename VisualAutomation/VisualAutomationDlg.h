
// VisualAutomationDlg.h : header file
//
#pragma once

#include "Camera.h"

class CVisualAutomationDlgAutoProxy;


// CVisualAutomationDlg dialog
class CVisualAutomationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVisualAutomationDlg);
	friend class CVisualAutomationDlgAutoProxy;

// Construction
public:
	CVisualAutomationDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CVisualAutomationDlg();

// Dialog Data
	enum { IDD = IDD_VISUALAUTOMATION_DIALOG };
	

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CVisualAutomationDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;
	

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCancel();
};
