
// LL.h : main header file for the LL application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CLLApp:
// See LL.cpp for the implementation of this class
//

class CLLApp : public CWinApp
{
public:
	CLLApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CLLApp theApp;
