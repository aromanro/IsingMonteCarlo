// ChartsPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "ChartsPropertyPage.h"
#include "afxdialogex.h"


// CChartsPropertyPage dialog

IMPLEMENT_DYNAMIC(CChartsPropertyPage, CPropertyPage)

CChartsPropertyPage::CChartsPropertyPage()
	: CPropertyPage(IDD_CHARTS_PROPPAGE)	
{
	m_lineThickness = theApp.options.chartLineThickness;
	m_specificHeatMax = theApp.options.maxSpecificHeat;
	m_susceptibilityMax = theApp.options.maxSusceptibility;
	m_specificHeatTicks = theApp.options.ticksSpecificHeat + 1;
	m_susceptibilityTicks = theApp.options.ticksSusceptibility + 1;
}

CChartsPropertyPage::~CChartsPropertyPage()
{
}

void CChartsPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_energyColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_magnetizationColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON3, m_specificHeatColor);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON4, m_susceptibilityColor);

	DDX_Text(pDX, IDC_EDIT1, m_lineThickness);
	DDX_Text(pDX, IDC_EDIT2, m_specificHeatMax);
	DDX_Text(pDX, IDC_EDIT3, m_susceptibilityMax);
	DDX_Text(pDX, IDC_EDIT4, m_specificHeatTicks);
	DDX_Text(pDX, IDC_EDIT5, m_susceptibilityTicks);

	DDV_MinMaxUInt(pDX, m_lineThickness, 0, 9);
	DDV_MinMaxUInt(pDX, m_specificHeatMax, 2, 100);
	DDV_MinMaxUInt(pDX, m_susceptibilityMax, 50, 1000);

	DDV_MinMaxUInt(pDX, m_specificHeatTicks, 2, 10);
	DDV_MinMaxUInt(pDX, m_susceptibilityTicks, 2, 10);
}


BEGIN_MESSAGE_MAP(CChartsPropertyPage, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CChartsPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT5, &CChartsPropertyPage::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CChartsPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CChartsPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON3, &CChartsPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON4, &CChartsPropertyPage::OnBnClickedMfccolorbutton)
END_MESSAGE_MAP()


// CChartsPropertyPage message handlers


void CChartsPropertyPage::ApplyValues()
{
	theApp.options.chartLineThickness = m_lineThickness;
	theApp.options.maxSpecificHeat = m_specificHeatMax;
	theApp.options.maxSusceptibility = m_susceptibilityMax;
	theApp.options.ticksSpecificHeat = m_specificHeatTicks - 1;
	theApp.options.ticksSusceptibility = m_susceptibilityTicks - 1;

	theApp.options.energyColor = m_energyColor.GetColor();
	theApp.options.magnetizationColor = m_magnetizationColor.GetColor();
	theApp.options.specificHeatColor = m_specificHeatColor.GetColor();
	theApp.options.susceptibilityColor = m_susceptibilityColor.GetColor();
	
	theApp.options.Save();
}


BOOL CChartsPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CPropertyPage::OnApply();
}


BOOL CChartsPropertyPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_energyColor.EnableOtherButton(_T("More Colors..."));
	m_energyColor.SetColor(theApp.options.energyColor);

	m_magnetizationColor.EnableOtherButton(_T("More Colors..."));
	m_magnetizationColor.SetColor(theApp.options.magnetizationColor);

	m_specificHeatColor.EnableOtherButton(_T("More Colors..."));
	m_specificHeatColor.SetColor(theApp.options.specificHeatColor);

	m_susceptibilityColor.EnableOtherButton(_T("More Colors..."));
	m_susceptibilityColor.SetColor(theApp.options.susceptibilityColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CChartsPropertyPage::OnEnChangeEdit()
{
	SetModified();
}



void CChartsPropertyPage::OnBnClickedMfccolorbutton()
{
	SetModified();
}


