#pragma once
#include "afxwin.h"

#include "NumberEdit.h"

// CSimulationPropertyPage dialog

class CSimulationPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CSimulationPropertyPage)

public:
	CSimulationPropertyPage();
	virtual ~CSimulationPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIMULATION_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	afx_msg void OnEnChangeEdit();

	unsigned int m_threadsNo;
	double m_minTemp;
	double m_maxTemp;
	double m_stepTemp;
	unsigned int m_warmupSteps;
	unsigned int m_equilibrationSteps;
	unsigned int m_statisticsSteps;

	CNumberEdit m_minTempEdit;
	CNumberEdit m_maxTempEdit;
	CNumberEdit m_stepTempEdit;
};
