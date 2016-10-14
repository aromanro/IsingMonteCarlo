// SimulationPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "SimulationPropertyPage.h"
#include "afxdialogex.h"

#include <algorithm>

// CSimulationPropertyPage dialog

IMPLEMENT_DYNAMIC(CSimulationPropertyPage, CMFCPropertyPage)

CSimulationPropertyPage::CSimulationPropertyPage()
	: CMFCPropertyPage(IDD_SIMULATION_PROPPAGE)
{
	m_threadsNo = theApp.options.numThreads;
	
	m_minTemp = theApp.options.lowTemperature;
	m_maxTemp = theApp.options.highTemperature;
	m_stepTemp = theApp.options.temperatureStep;

	m_warmupSteps = theApp.options.warmupSteps;
	m_equilibrationSteps = theApp.options.equilibrationSteps;
	m_statisticsSteps = theApp.options.collectionSteps;
}

CSimulationPropertyPage::~CSimulationPropertyPage()
{
}

void CSimulationPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_minTempEdit);
	DDX_Control(pDX, IDC_EDIT2, m_maxTempEdit);
	DDX_Control(pDX, IDC_EDIT3, m_stepTempEdit);

	DDX_Text(pDX, IDC_EDIT1, m_minTemp);
	DDX_Text(pDX, IDC_EDIT2, m_maxTemp);
	DDX_Text(pDX, IDC_EDIT3, m_stepTemp);

	DDX_Text(pDX, IDC_EDIT4, m_threadsNo);

	DDX_Text(pDX, IDC_EDIT5, m_warmupSteps);
	DDX_Text(pDX, IDC_EDIT6, m_equilibrationSteps);
	DDX_Text(pDX, IDC_EDIT7, m_statisticsSteps);

	DDV_MinMaxUInt(pDX, m_threadsNo, 1, 128);
	DDV_MinMaxDouble(pDX, m_stepTemp, 0.01, 1.);
	DDV_MinMaxUInt(pDX, m_statisticsSteps, 1, 1000000);
}


BEGIN_MESSAGE_MAP(CSimulationPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT4, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT1, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT6, &CSimulationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT7, &CSimulationPropertyPage::OnEnChangeEdit)
END_MESSAGE_MAP()


// CSimulationPropertyPage message handlers


void CSimulationPropertyPage::ApplyValues()
{

	theApp.options.numThreads = m_threadsNo;

	if (m_minTemp > m_maxTemp) std::swap(m_minTemp, m_maxTemp);

	theApp.options.lowTemperature = m_minTemp;
	theApp.options.highTemperature = m_maxTemp;
	theApp.options.temperatureStep = m_stepTemp;

	theApp.options.warmupSteps = m_warmupSteps;
	theApp.options.equilibrationSteps = m_equilibrationSteps;
	theApp.options.collectionSteps = m_statisticsSteps;

	theApp.options.Save();
}


BOOL CSimulationPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	m_minTempEdit.allowNegative = false;
	m_maxTempEdit.allowNegative = false;
	m_stepTempEdit.allowNegative = false;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSimulationPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


void CSimulationPropertyPage::OnEnChangeEdit()
{
	SetModified();
}


