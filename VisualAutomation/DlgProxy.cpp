
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "VisualAutomation.h"
#include "DlgProxy.h"
#include "VisualAutomationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisualAutomationDlgAutoProxy

IMPLEMENT_DYNCREATE(CVisualAutomationDlgAutoProxy, CCmdTarget)

CVisualAutomationDlgAutoProxy::CVisualAutomationDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CVisualAutomationDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CVisualAutomationDlg)))
		{
			m_pDialog = reinterpret_cast<CVisualAutomationDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CVisualAutomationDlgAutoProxy::~CVisualAutomationDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CVisualAutomationDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CVisualAutomationDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CVisualAutomationDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IVisualAutomation to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {00706EDC-9BBD-4FB7-BFAB-5AF49DE21980}
static const IID IID_IVisualAutomation =
{ 0x706EDC, 0x9BBD, 0x4FB7, { 0xBF, 0xAB, 0x5A, 0xF4, 0x9D, 0xE2, 0x19, 0x80 } };

BEGIN_INTERFACE_MAP(CVisualAutomationDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CVisualAutomationDlgAutoProxy, IID_IVisualAutomation, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {1B6079FA-92EF-4377-9BDA-7888529679DD}
IMPLEMENT_OLECREATE2(CVisualAutomationDlgAutoProxy, "VisualAutomation.Application", 0x1b6079fa, 0x92ef, 0x4377, 0x9b, 0xda, 0x78, 0x88, 0x52, 0x96, 0x79, 0xdd)


// CVisualAutomationDlgAutoProxy message handlers
