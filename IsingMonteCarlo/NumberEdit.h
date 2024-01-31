#pragma once


// CNumberEdit

class CNumberEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumberEdit)
	DECLARE_MESSAGE_MAP()
public:
	void SetAllowNegative(bool allow) { allowNegative = allow; }

private:
	CString m_GoodValue;
	DWORD m_GoodSel = 0;
	bool m_Cancelling = false;
	bool allowNegative = true;

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEnUpdate();
	afx_msg	LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
};


