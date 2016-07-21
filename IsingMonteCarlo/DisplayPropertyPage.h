#pragma once
#include "afxcolorbutton.h"


// CDisplayPropertyPage dialog

class CDisplayPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDisplayPropertyPage)

public:
	CDisplayPropertyPage();
	virtual ~CDisplayPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPLAY_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfccolorbutton();
	afx_msg void OnEnChangeEdit1();

	CMFCColorButton m_SpinUpColorControl;
	CMFCColorButton m_SpinDownColorControl;
	unsigned int m_timerInterval;
};
