
// IsingMonteCarloView.h : interface of the CIsingMonteCarloView class
//

#pragma once

#include "MemoryBitmap.h"

class CIsingMonteCarloView : public CView
{
protected: // create from serialization only
	CIsingMonteCarloView();
	DECLARE_DYNCREATE(CIsingMonteCarloView)

// Attributes
public:
	CIsingMonteCarloDoc* GetDocument() const;
// Operations
// Overrides
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CIsingMonteCarloView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT_PTR timer;

	MemoryBitmap* bitmap;

	MemoryBitmap* renorm1;
	MemoryBitmap* renorm2;
	MemoryBitmap* renorm3;

	int renorm1Pixels;
	int renorm2Pixels;
	int renorm3Pixels;


// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	void DisplayRenormalizationForSpins(CDC* pDC, bool inc, CRect& rect, MemoryBitmap* renorm, MonteCarlo::SpinMatrix& spinsRenorm, MonteCarlo::SpinMatrix& dispSpinsRenorm, int& renormPixels);


	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	void StartTimer();
	void StopTimer();
	void DisplayRenormalization(CDC* pDC, bool inc = false);
	void ResetTimer();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
};

#ifndef _DEBUG  // debug version in IsingMonteCarloView.cpp
inline CIsingMonteCarloDoc* CIsingMonteCarloView::GetDocument() const
   { return reinterpret_cast<CIsingMonteCarloDoc*>(m_pDocument); }
#endif

