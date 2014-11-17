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

void CDialogVision::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VALUE_X, labelValueX);
	labelValueX.SetWindowTextW(_T("45"));
	DDX_Control(pDX, IDC_VALUE_Y, labelValueY);
	DDX_Control(pDX, IDC_VALUE_ROT, labelValueRot);
}


BEGIN_MESSAGE_MAP(CDialogVision, CDialogEx)
END_MESSAGE_MAP()


// CDialogVision message handlers
