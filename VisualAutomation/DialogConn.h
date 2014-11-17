#pragma once


// CDialogConn dialog

class CDialogConn : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogConn)


public:
	CDialogConn(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogConn();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
