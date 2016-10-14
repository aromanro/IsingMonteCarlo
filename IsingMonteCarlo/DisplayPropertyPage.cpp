// DisplayPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "IsingMonteCarlo.h"
#include "DisplayPropertyPage.h"
#include "afxdialogex.h"

#include "IsingMonteCarloDoc.h"
#include "IsingMonteCarloView.h"


// CDisplayPropertyPage dialog

IMPLEMENT_DYNAMIC(CDisplayPropertyPage, CMFCPropertyPage)

CDisplayPropertyPage::CDisplayPropertyPage()
	: CMFCPropertyPage(IDD_DISPLAY_PROPPAGE)	
{
	m_timerInterval = theApp.options.timerInterval;
}

CDisplayPropertyPage::~CDisplayPropertyPage()
{
}

void CDisplayPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_SpinUpColorControl);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, m_SpinDownColorControl);

	DDX_Text(pDX, IDC_EDIT1, m_timerInterval);

	DDV_MinMaxUInt(pDX, m_timerInterval, 5, 1000);
}


BEGIN_MESSAGE_MAP(CDisplayPropertyPage, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CDisplayPropertyPage::OnBnClickedMfccolorbutton)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON2, &CDisplayPropertyPage::OnBnClickedMfccolorbutton)
	ON_EN_CHANGE(IDC_EDIT1, &CDisplayPropertyPage::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CDisplayPropertyPage message handlers


void CDisplayPropertyPage::ApplyValues()
{
	theApp.options.timerInterval = m_timerInterval;

	theApp.options.spinUpColor = m_SpinUpColorControl.GetColor();
	theApp.options.spinDownColor = m_SpinDownColorControl.GetColor();

	theApp.options.Save();

	POSITION posDocTemplate = theApp.GetFirstDocTemplatePosition();
	while (posDocTemplate)
	{
		CDocTemplate* pDocTemplate = theApp.GetNextDocTemplate(posDocTemplate);

		// get each document open in given document template
		POSITION posDoc = pDocTemplate->GetFirstDocPosition();
		while (posDoc)
		{
			CIsingMonteCarloDoc* pDoc = (CIsingMonteCarloDoc*)pDocTemplate->GetNextDoc(posDoc);

			if (pDoc)
			{
				CIsingMonteCarloView *view = pDoc->GetView();

				if (view) view->ResetTimer();
			}
		}
	}
}


BOOL CDisplayPropertyPage::OnApply()
{
	UpdateData();

	ApplyValues();

	return CMFCPropertyPage::OnApply();
}


BOOL CDisplayPropertyPage::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_SpinUpColorControl.EnableOtherButton(_T("More Colors..."));
	m_SpinUpColorControl.SetColor(theApp.options.spinUpColor);

	m_SpinDownColorControl.EnableOtherButton(_T("More Colors..."));
	m_SpinDownColorControl.SetColor(theApp.options.spinDownColor);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDisplayPropertyPage::OnBnClickedMfccolorbutton()
{
	SetModified();
}

void CDisplayPropertyPage::OnEnChangeEdit1()
{
	SetModified();
}
