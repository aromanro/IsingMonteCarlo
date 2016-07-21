// OptionsPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "OptionsPropertySheet.h"


// OptionsPropertySheet

IMPLEMENT_DYNAMIC(COptionsPropertySheet, CPropertySheet)

COptionsPropertySheet::COptionsPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage), hIcon(0)
{

}

COptionsPropertySheet::COptionsPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage), hIcon(0)
{

}

COptionsPropertySheet::~COptionsPropertySheet()
{
	if (hIcon) DestroyIcon(hIcon);
}


BEGIN_MESSAGE_MAP(COptionsPropertySheet, CPropertySheet)
END_MESSAGE_MAP()


// OptionsPropertySheet message handlers


BOOL COptionsPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	hIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);

	SetIcon(hIcon, FALSE);

	return bResult;
}
