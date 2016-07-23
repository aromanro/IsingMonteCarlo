#pragma once
#include "afxcolorbutton.h"


// CChartsPropertyPage dialog

class CChartsPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CChartsPropertyPage)

public:
	CChartsPropertyPage();
	virtual ~CChartsPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTS_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();

	afx_msg void OnEnChangeEdit();	
	afx_msg void OnBnClickedMfccolorbutton();
	CMFCColorButton m_energyColor;
	CMFCColorButton m_magnetizationColor;
	CMFCColorButton m_specificHeatColor;
	CMFCColorButton m_susceptibilityColor;
	unsigned int m_lineThickness;
	unsigned int m_specificHeatMax;
	unsigned int m_susceptibilityMax;
	unsigned int m_specificHeatTicks;
	unsigned int m_susceptibilityTicks;
	int m_useSplines;

	afx_msg void OnBnClickedCheck1();
};
