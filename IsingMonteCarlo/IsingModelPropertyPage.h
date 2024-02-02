#pragma once
#include "afxwin.h"

#include "NumberEdit.h"

// CIsingModelPropertyPage dialog

class CIsingModelPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CIsingModelPropertyPage)

public:
	CIsingModelPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ISING_PROPPAGE };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeEdit();
	void ApplyValues();
	BOOL OnApply() override;
	BOOL OnInitDialog() override;

	unsigned int m_latticeSize;
	double m_B;
	CNumberEdit m_BEdit;
};
