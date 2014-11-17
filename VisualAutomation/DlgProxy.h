
// DlgProxy.h: header file
//

#pragma once

class CVisualAutomationDlg;


// CVisualAutomationDlgAutoProxy command target

class CVisualAutomationDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CVisualAutomationDlgAutoProxy)

	CVisualAutomationDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CVisualAutomationDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CVisualAutomationDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CVisualAutomationDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

