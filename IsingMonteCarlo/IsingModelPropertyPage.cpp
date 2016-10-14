// IsingModelPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "IsingModelPropertyPage.h"
#include "afxdialogex.h"

#include "Options.h"

// CIsingModelPropertyPage dialog

IMPLEMENT_DYNAMIC(CIsingModelPropertyPage, CMFCPropertyPage)

CIsingModelPropertyPage::CIsingModelPropertyPage()
	: CMFCPropertyPage(IDD_ISING_PROPPAGE)
{
	m_latticeSize = theApp.options.latticeSize;
	m_B = theApp.options.B;
}

CIsingModelPropertyPage::~CIsingModelPropertyPage()
{
}

void CIsingModelPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT2, m_BEdit);

	DDX_Text(pDX, IDC_EDIT1, m_latticeSize);
	DDX_Text(pDX, IDC_EDIT2, m_B);

	DDV_MinMaxUInt(pDX, m_latticeSize, 32, 4096);	
}


BEGIN_MESSAGE_MAP(CIsingModelPropertyPage, CMFCPropertyPage)
	ON_EN_CHANGE(IDC_EDIT1, &CIsingModelPropertyPage::OnEnChangeEdit)
	ON_EN_CHANGE(IDC_EDIT2, &CIsingModelPropertyPage::OnEnChangeEdit)
END_MESSAGE_MAP()


// CIsingModelPropertyPage message handlers


void CIsingModelPropertyPage::OnEnChangeEdit()
{
	SetModified();
}


void CIsingModelPropertyPage::ApplyValues()
{
	theApp.options.latticeSize = m_latticeSize;
	theApp.options.B = m_B;

	theApp.options.Save();
}


BOOL CIsingModelPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL CIsingModelPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
