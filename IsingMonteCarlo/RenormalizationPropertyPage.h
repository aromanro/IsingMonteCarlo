#pragma once
#include "afxwin.h"

#include "NumberEdit.h"

// CRenormalizationPropertyPage dialog

class CRenormalizationPropertyPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(CRenormalizationPropertyPage)

public:
	CRenormalizationPropertyPage();
	virtual ~CRenormalizationPropertyPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RENORMALIZATION_PROPPAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void ApplyValues();
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit();
	afx_msg void OnBnClickedRadio();

	double m_Temp1;
	double m_Temp2;
	double m_Temp3;
	unsigned int m_maxRenormalizationSteps;
	int m_renormType;
	CNumberEdit m_Temp1Edit;
	CNumberEdit m_Temp2Edit;
	CNumberEdit m_Temp3Edit;
};
