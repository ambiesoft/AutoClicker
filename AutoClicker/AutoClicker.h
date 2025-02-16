
// AutoClicker.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAutoClickerApp:
// See AutoClicker.cpp for the implementation of this class
//

class CAutoClickerApp : public CWinApp
{
public:
	CAutoClickerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	CString GetIniFile();
	DECLARE_MESSAGE_MAP()
};

extern CAutoClickerApp theApp;
