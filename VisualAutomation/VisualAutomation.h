
// VisualAutomation.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CVisualAutomationApp:
// See VisualAutomation.cpp for the implementation of this class
//

class CVisualAutomationApp : public CWinApp
{
public:
	CVisualAutomationApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVisualAutomationApp theApp;