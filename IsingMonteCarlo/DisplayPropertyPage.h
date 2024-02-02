#pragma once
#include "afxcolorbutton.h"


// CDisplayPropertyPage dialog

class CDisplayPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CDisplayPropertyPage)

public:
	CDisplayPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAY_PROPPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;
	afx_msg void OnBnClickedMfccolorbutton();
	afx_msg void OnEnChangeEdit1();

	CMFCColorButton m_SpinUpColorControl;
	CMFCColorButton m_SpinDownColorControl;
	unsigned int m_timerInterval;
};
