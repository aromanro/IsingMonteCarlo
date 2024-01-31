// RenormalizationPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "RenormalizationPropertyPage.h"
#include "afxdialogex.h"

#include <algorithm>


// CRenormalizationPropertyPage dialog

IMPLEMENT_DYNAMIC(CRenormalizationPropertyPage, CMFCPropertyPage)

CRenormalizationPropertyPage::CRenormalizationPropertyPage()
	: CMFCPropertyPage(IDD_RENORMALIZATION_PROPPAGE)	
{
	m_Temp1 = theApp.options.renormalizationTemperature1;
	m_Temp2 = theApp.options.renormalizationTemperature2;
	m_Temp3 = theApp.options.renormalizationTemperature3;

	m_maxRenormalizationSteps = theApp.options.MaxRenormalizations;

	m_renormType = (int)theApp.options.renormalizationType;
}

CRenormalizationPropertyPage::~CRenormalizationPropertyPage()
{
}

void CRenormalizationPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_Temp1Edit);
	DDX_Control(pDX, IDC_EDIT2, m_Temp2Edit);
	DDX_Control(pDX, IDC_EDIT3, m_Temp3Edit);

	DDX_Text(pDX, IDC_EDIT1, m_Temp1);
	DDX_Text(pDX, IDC_EDIT2, m_Temp2);
	DDX_Text(pDX, IDC_EDIT3, m_Temp3);

	DDX_Text(pDX, IDC_EDIT4, m_maxRenormalizationSteps);

	DDX_Radio(pDX, IDC_RADIO1, m_renormType);

	DDV_MinMaxUInt(pDX, m_maxRenormalizationSteps, 1, 64);
}


BEGIN_MESSAGE_MAP(CRenormalizationPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CRenormalizationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CRenormalizationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT3, &CRenormalizationPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT4, &CRenormalizationPropertyPage::OnEnChangeEdit)
	ON_BN_CLICKED(IDC_RADIO1, &CRenormalizationPropertyPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CRenormalizationPropertyPage::OnBnClickedRadio)
END_MESSAGE_MAP()


// CRenormalizationPropertyPage message handlers


void CRenormalizationPropertyPage::ApplyValues()
{
	if (m_Temp1 > m_Temp2) std::swap(m_Temp1, m_Temp2);
	if (m_Temp2 > m_Temp3) std::swap(m_Temp2, m_Temp3);
	if (m_Temp1 > m_Temp2) std::swap(m_Temp1, m_Temp2);

	theApp.options.renormalizationTemperature1 = m_Temp1;
	theApp.options.renormalizationTemperature2 = m_Temp2;
	theApp.options.renormalizationTemperature3 = m_Temp3;

	theApp.options.MaxRenormalizations = m_maxRenormalizationSteps;

	theApp.options.renormalizationType = (Options::BlockRenormalizationType)m_renormType;

	theApp.options.Save();
}


BOOL CRenormalizationPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL CRenormalizationPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Temp1Edit.SetAllowNegative(false);
	m_Temp2Edit.SetAllowNegative(false);
	m_Temp3Edit.SetAllowNegative(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CRenormalizationPropertyPage::OnEnChangeEdit()
{
	SetModified();
}

void CRenormalizationPropertyPage::OnBnClickedRadio()
{
	SetModified();
}


