#pragma once
#include "afxcolorbutton.h"


// CChartsPropertyPage dialog

class CChartsPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CChartsPropertyPage)

public:
	CChartsPropertyPage();
	~CChartsPropertyPage() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHARTS_PROPPAGE };
#endif

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;

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
