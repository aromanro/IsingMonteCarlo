
// IsingMonteCarloView.h : interface of the CIsingMonteCarloView class
//

#pragma once

#include "MemoryBitmap.h"

class CIsingMonteCarloView : public CView
{
protected: // create from serialization only
	CIsingMonteCarloView();
	DECLARE_DYNCREATE(CIsingMonteCarloView)
	DECLARE_MESSAGE_MAP()

public:
	~CIsingMonteCarloView() override;

	CIsingMonteCarloDoc* GetDocument() const;
// Operations
	void ResetTimer();
	void StartTimer();
	void StopTimer();
// Overrides
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
protected:
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

// Implementation
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

private:
	UINT_PTR timer;

	MemoryBitmap* bitmap;

	MemoryBitmap* renorm1;
	MemoryBitmap* renorm2;
	MemoryBitmap* renorm3;

	int renorm1Pixels;
	int renorm2Pixels;
	int renorm3Pixels;


// Generated message map functions
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	void DisplayRenormalizationForSpins(CDC* pDC, bool inc, CRect& rect, MemoryBitmap* renorm, MonteCarlo::SpinMatrix& spinsRenorm, MonteCarlo::SpinMatrix& dispSpinsRenorm, int& renormPixels);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnInitialUpdate() override;
	afx_msg void OnDestroy();

	void DisplayRenormalization(CDC* pDC, bool inc = false);

	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL) override;
};

#ifndef _DEBUG  // debug version in IsingMonteCarloView.cpp
inline CIsingMonteCarloDoc* CIsingMonteCarloView::GetDocument() const
   { return reinterpret_cast<CIsingMonteCarloDoc*>(m_pDocument); }
#endif

