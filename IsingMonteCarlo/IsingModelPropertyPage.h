#pragma once
#include "afxwin.h"

#include "NumberEdit.h"

// CIsingModelPropertyPage dialog

class CIsingModelPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CIsingModelPropertyPage)

public:
	CIsingModelPropertyPage();
	virtual ~CIsingModelPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ISING_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeEdit();
	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();

	unsigned int m_latticeSize;
	double m_B;
	CNumberEdit m_BEdit;
};
