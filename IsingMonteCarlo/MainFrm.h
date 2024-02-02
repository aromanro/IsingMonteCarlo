
// MainFrm.h : interface of the CMainFrame class
//

#pragma once


class CIsingMonteCarloDoc;

class CMainFrame : public CFrameWndEx
{	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	DECLARE_MESSAGE_MAP()
// Attributes
public:
// Operations
// Overrides
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL) override;

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

private:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// Generated message map functions

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

	CIsingMonteCarloDoc* GetDocument();

	afx_msg void OnViewRenormalization();
	afx_msg void OnUpdateViewRenormalization(CCmdUI *pCmdUI);
	afx_msg void OnViewOptions();
	afx_msg void OnSimulateRun();
	afx_msg void OnUpdateSimulateRun(CCmdUI *pCmdUI);
};


