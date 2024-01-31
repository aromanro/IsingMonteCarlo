#pragma once
#include "afxwin.h"

#include "NumberEdit.h"

// CSimulationPropertyPage dialog

class CSimulationPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CSimulationPropertyPage)

public:
	CSimulationPropertyPage();
	~CSimulationPropertyPage() override;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULATION_PROPPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	BOOL OnInitDialog() override;
	BOOL OnApply() override;
	afx_msg void OnEnChangeEdit();

	unsigned int m_threadsNo;
	double m_minTemp;
	double m_maxTemp;
	double m_stepTemp;
	unsigned int m_warmupSteps;
	unsigned int m_equilibrationSteps;
	unsigned int m_statisticsSteps;
	unsigned int m_stepsBetweenCollect;

	CNumberEdit m_minTempEdit;
	CNumberEdit m_maxTempEdit;
	CNumberEdit m_stepTempEdit;
};
