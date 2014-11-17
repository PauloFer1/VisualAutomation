#pragma once
#include "afxwin.h"


// CDialogVision dialog

class CDialogVision : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogVision)

public:
	CDialogVision(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogVision();

// Dialog Data
	enum { IDD = IDD_DIALOG_VISION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic labelValueX;
	CStatic labelValueY;
	CStatic labelValueRot;
};
