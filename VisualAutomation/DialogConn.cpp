// DialogConn.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "DialogConn.h"
#include "afxdialogex.h"


// CDialogConn dialog

IMPLEMENT_DYNAMIC(CDialogConn, CDialogEx)

CDialogConn::CDialogConn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogConn::IDD, pParent)
{

}

CDialogConn::~CDialogConn()
{
}

void CDialogConn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogConn, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogConn::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogConn::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDialogConn message handlers


void CDialogConn::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CDialogConn::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
