
// IsingMonteCarlo.h : main header file for the IsingMonteCarlo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "Options.h"

#include <objidl.h>
#include <gdiplus.h>

#pragma comment (lib,"Gdiplus.lib")

// CIsingMonteCarloApp:
// See IsingMonteCarlo.cpp for the implementation of this class
//

class CIsingMonteCarloApp : public CWinAppEx
{
public:
	CIsingMonteCarloApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation:	
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

protected:
	ULONG_PTR gdiplusToken;
public:
	Options options;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CIsingMonteCarloApp theApp;
