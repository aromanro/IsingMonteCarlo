
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

// Implementation:	
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;


	Options options;

	afx_msg void OnAppAbout();

private:
	// Overrides
	BOOL InitInstance() override;
	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;
	int ExitInstance() override;

	ULONG_PTR gdiplusToken;

	DECLARE_MESSAGE_MAP()
};

extern CIsingMonteCarloApp theApp;
