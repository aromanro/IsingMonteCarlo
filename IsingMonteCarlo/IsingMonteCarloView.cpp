
// IsingMonteCarloView.cpp : implementation of the CIsingMonteCarloView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IsingMonteCarlo.h"
#endif

#include "IsingMonteCarloDoc.h"
#include "IsingMonteCarloView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int spinSize = 4;

// CIsingMonteCarloView

IMPLEMENT_DYNCREATE(CIsingMonteCarloView, CView)

BEGIN_MESSAGE_MAP(CIsingMonteCarloView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CIsingMonteCarloView::OnFilePrintPreview)	
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CIsingMonteCarloView construction/destruction

CIsingMonteCarloView::CIsingMonteCarloView()
	: timer(0), bitmap(new MemoryBitmap()), renorm1(nullptr), renorm2(nullptr), renorm3(nullptr)
{	
}

CIsingMonteCarloView::~CIsingMonteCarloView()
{
	delete bitmap;
	delete renorm1;
	delete renorm2;
	delete renorm3;
}

BOOL CIsingMonteCarloView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIsingMonteCarloView drawing

void CIsingMonteCarloView::OnDraw(CDC* pDC)
{
	CIsingMonteCarloDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);
	
	if (pDoc->computing)
	{
		MonteCarlo::SpinMatrix matrix{ pDoc->GetData() };

		//bitmap->SetSize(rect.Width() + spinSize, rect.Height() + spinSize);
		
		int spinsWidth = static_cast<int>(ceill(static_cast<double>(rect.Width()) / spinSize));
		int spinsHeight = static_cast<int>(ceill(static_cast<double>(rect.Height()) / spinSize));

		bitmap->SetMatrix(matrix, spinsWidth, spinsHeight, spinSize);

		bitmap->Draw(pDC);

		CFont font;
		font.CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");


		rect.bottom = rect.top + 38;

		CFont* oldFont = pDC->SelectObject(&font);
		COLORREF oldColor = pDC->SetBkColor(RGB(210, 210, 210));
		UINT oldAlign = pDC->SetTextAlign(TA_CENTER);

		CString str;
		str.Format(L"Temperature = %.2f", matrix.Temperature);
		
		rect.left += rect.Width() / 2 - 150;
		rect.right = rect.left + 300;
		pDC->ExtTextOut(rect.left + rect.Width() / 2, rect.top, ETO_CLIPPED | ETO_OPAQUE, rect, str, NULL);

		pDC->SelectObject(oldFont);

		font.DeleteObject();

		pDC->SetBkColor(oldColor);
		pDC->SetTextAlign(oldAlign);
	}
	else if (pDoc->finishedCalc)
	{
		if (pDoc->displayingRenormalization)
		{	
			CBrush whiteBrush(RGB(255,255,255));
			pDC->FillRect(rect, &whiteBrush);
			DisplayRenormalization(pDC);
		}
		else
		{
			CRect rect1, rect2, rect3, rect4;

			rect1.top = rect2.top = 0;
			rect3.top = rect4.top = rect1.bottom = rect2.bottom = rect.Height() / 2;
			rect3.left = rect1.left = 0;
			rect3.right = rect4.left = rect1.right = rect2.left = rect.Width() / 2;
			rect4.right = rect2.right = rect.Width();
			rect4.bottom = rect3.bottom = rect.Height();

			pDoc->m_energyChart.Draw(pDC, rect1);
			pDoc->m_magnetizationChart.Draw(pDC, rect2);
			pDoc->m_specificHeatChart.Draw(pDC, rect3);
			pDoc->m_susceptibilityChart.Draw(pDC, rect4);
		}
	}
	else
	{
		Gdiplus::Graphics gdi(pDC->GetSafeHdc());
		Gdiplus::SolidBrush whiteBrush((Gdiplus::ARGB)Gdiplus::Color::White);
		gdi.FillRectangle(&whiteBrush, rect.left, rect.top, rect.Width(), rect.Height());
	}	
}


// CIsingMonteCarloView printing


void CIsingMonteCarloView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CIsingMonteCarloView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIsingMonteCarloView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIsingMonteCarloView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CIsingMonteCarloView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CIsingMonteCarloView::OnContextMenu(CWnd* /* pWnd */, CPoint /*point*/)
{
#ifndef SHARED_HANDLERS

#endif
}


// CIsingMonteCarloView diagnostics

#ifdef _DEBUG
void CIsingMonteCarloView::AssertValid() const
{
	CView::AssertValid();
}

void CIsingMonteCarloView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIsingMonteCarloDoc* CIsingMonteCarloView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIsingMonteCarloDoc)));
	return dynamic_cast<CIsingMonteCarloDoc*>(m_pDocument);
}
#endif //_DEBUG


// CIsingMonteCarloView message handlers


BOOL CIsingMonteCarloView::OnEraseBkgnd(CDC* pDC)
{
	if (pDC->IsPrinting())
		return CView::OnEraseBkgnd(pDC);

	return TRUE;
}


void CIsingMonteCarloView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	const CIsingMonteCarloDoc* pDoc = GetDocument();
	
	if (pDoc->displayingRenormalization) DisplayRenormalization(GetDC(), true);
	else Invalidate();

	CView::OnTimer(nIDEvent);
}


void CIsingMonteCarloView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void CIsingMonteCarloView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	StopTimer();
}




void CIsingMonteCarloView::StartTimer()
{
	if (!timer) timer = SetTimer(1, theApp.options.timerInterval, NULL);

	const CIsingMonteCarloDoc* pDoc = GetDocument();
	if (pDoc && pDoc->computing) BeginWaitCursor();
}


void CIsingMonteCarloView::StopTimer()
{
	if (timer) {
		KillTimer(timer);
		timer = 0;

		delete renorm1;
		delete renorm2;
		delete renorm3;
		renorm1 = renorm2 = renorm3 = nullptr;
	}
}

void CIsingMonteCarloView::ResetTimer()
{
	if (!timer) return;

	KillTimer(timer);
	timer = SetTimer(1, theApp.options.timerInterval, NULL);
}

void CIsingMonteCarloView::DisplayRenormalization(CDC* pDC, bool inc)
{
	CIsingMonteCarloDoc* pDoc = GetDocument();
	if (!renorm1) {
		renorm1 = new MemoryBitmap();
		renorm1->SetMatrix(pDoc->dispSpinsRenorm1, 64 * 2, 64 * 2, spinSize);
		renorm1Pixels = 64 * spinSize;
	}
	if (!renorm2) {
		renorm2 = new MemoryBitmap();
		renorm2->SetMatrix(pDoc->dispSpinsRenorm2, 64 * 2, 64 * 2, spinSize);
		renorm2Pixels = 64 * spinSize;
	}
	if (!renorm3) {
		renorm3 = new MemoryBitmap();
		renorm3->SetMatrix(pDoc->dispSpinsRenorm3, 64 * 2, 64 * 2, spinSize);
		renorm3Pixels = 64 * spinSize;
	}

	CRect rect;
	GetClientRect(rect);

	CRect rect1, rect2, rect3;

	rect1.top = rect2.top = rect3.top = rect.Height() / 2 - 64 / 2 * spinSize;
	rect1.bottom = rect2.bottom = rect3.bottom = rect1.top + 64 * spinSize;

	rect2.left = rect.Width() / 2 - 64 / 2 * spinSize;
	rect2.right = rect2.left + 64 * spinSize;

	rect1.left = rect2.left - 10 - 64 * spinSize;
	rect1.right = rect1.left + 64 * spinSize;

	rect3.left = rect2.right + 10;
	rect3.right = rect3.left + 64 * spinSize;

	DisplayRenormalizationForSpins(pDC, inc, rect1, renorm1, pDoc->spinsRenorm1, pDoc->dispSpinsRenorm1, renorm1Pixels);
	DisplayRenormalizationForSpins(pDC, inc, rect2, renorm2, pDoc->spinsRenorm2, pDoc->dispSpinsRenorm2, renorm2Pixels);
	DisplayRenormalizationForSpins(pDC, inc, rect3, renorm3, pDoc->spinsRenorm3, pDoc->dispSpinsRenorm3, renorm3Pixels);
}

void CIsingMonteCarloView::DisplayRenormalizationForSpins(CDC* pDC, bool inc, CRect& rect, MemoryBitmap* renorm, MonteCarlo::SpinMatrix& spinsRenorm, MonteCarlo::SpinMatrix& dispSpinsRenorm, int& renormPixels)
{
	renorm->Draw(pDC, rect, renormPixels, renormPixels);

	CFont font;
	font.CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, L"Times New Roman");

	CFont* oldFont = pDC->SelectObject(&font);	
	COLORREF oldColor = pDC->SetBkColor(RGB(255, 255, 255));
	UINT oldAlign = pDC->SetTextAlign(TA_CENTER);


	CString str;

	str.Format(L"Start T: %.2f", spinsRenorm.Temperature);

	CRect textRect;
	textRect.left = rect.left;
	textRect.right = rect.right;
	textRect.top = rect.top - 20;
	textRect.bottom = textRect.top + 16;

	pDC->ExtTextOut(textRect.left + textRect.Width()/2, textRect.top, ETO_CLIPPED | ETO_OPAQUE, textRect, str, NULL);
	
	textRect.top = rect.bottom + 4;
	textRect.bottom = textRect.top + 16;

	str.Format(L"L = %d", static_cast<int>(renormPixels / spinSize * pow(2, dispSpinsRenorm.GetRenormalizationsNumber())));
	pDC->ExtTextOut(textRect.left + textRect.Width() / 2, textRect.top, ETO_CLIPPED | ETO_OPAQUE, textRect, str, NULL);

	pDC->SelectObject(oldFont);

	font.DeleteObject();

	pDC->SetBkColor(oldColor);
	pDC->SetTextAlign(oldAlign);

	if (inc)
	{
		renormPixels += spinSize;

		if (renormPixels >= 2 * 64 * spinSize)
		{
			dispSpinsRenorm.RenormalizeInPlace(theApp.options.renormalizationType);
			renorm->SetMatrix(dispSpinsRenorm, 64 * 2, 64 * 2, spinSize);
			renormPixels = 64 * spinSize;

			if (dispSpinsRenorm.GetRenormalizationsNumber() >= theApp.options.MaxRenormalizations)
			{
				dispSpinsRenorm = spinsRenorm;
				renorm->SetMatrix(dispSpinsRenorm, 64 * 2, 64 * 2, spinSize);
			}
		}
	}
}


BOOL CIsingMonteCarloView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	const CIsingMonteCarloDoc* pDoc = GetDocument();

	if (pDoc && pDoc->computing)
	{
		RestoreWaitCursor();

		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CIsingMonteCarloView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pDC->IsPrinting())
	{
		CRect rect;
		GetClientRect(rect);

		pDC->SetMapMode(MM_ISOTROPIC);

		int cx = pDC->GetDeviceCaps(HORZRES);
		int cy = pDC->GetDeviceCaps(VERTRES);

		pDC->SetWindowExt(rect.Width(), rect.Height());
		pDC->SetViewportExt(cx, cy);
		pDC->SetViewportOrg(0, 0);
	}

	CView::OnPrepareDC(pDC, pInfo);
}
