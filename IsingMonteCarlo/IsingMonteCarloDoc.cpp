
// IsingMonteCarloDoc.cpp : implementation of the CIsingMonteCarloDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IsingMonteCarlo.h"
#endif

#include <algorithm>

#include "IsingMonteCarloDoc.h"
#include "IsingMonteCarloView.h"

#include <propkey.h>

#include "MonteCarloThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIsingMonteCarloDoc

IMPLEMENT_DYNCREATE(CIsingMonteCarloDoc, CDocument)

BEGIN_MESSAGE_MAP(CIsingMonteCarloDoc, CDocument)
END_MESSAGE_MAP()


// CIsingMonteCarloDoc construction/destruction

CIsingMonteCarloDoc::CIsingMonteCarloDoc()
	: spins(theApp.options.latticeSize), 
	spinsRenorm1(theApp.options.latticeSize),
	spinsRenorm2(theApp.options.latticeSize),
	spinsRenorm3(theApp.options.latticeSize),
	dispSpinsRenorm1(theApp.options.latticeSize),
	dispSpinsRenorm2(theApp.options.latticeSize),
	dispSpinsRenorm3(theApp.options.latticeSize),
	finishedCalc(false), computing(false), displayingRenormalization(false)
{
	// TODO: add one-time construction code here
	m_energyChart.title = L"Energy/spin";
	m_magnetizationChart.title = L"Magnetization/spin";
	m_specificHeatChart.title = L"Specific heat/spin";
	m_susceptibilityChart.title = L"Susceptibility/spin";

	m_energyChart.XAxisLabel = m_magnetizationChart.XAxisLabel = m_specificHeatChart.XAxisLabel = m_susceptibilityChart.XAxisLabel = L"T";

	m_energyChart.YAxisLabel = L"<E>";
	m_magnetizationChart.YAxisLabel = L"<|M|>";
	m_specificHeatChart.YAxisLabel = L"Cv";
	m_susceptibilityChart.YAxisLabel = L"X";

	m_energyChart.YAxisMax = 0;
	m_energyChart.YAxisMin = -2;

	m_magnetizationChart.YAxisMax = 1;

	m_energyChart.XAxisMin = m_magnetizationChart.XAxisMin = m_magnetizationChart.YAxisMin =
		m_specificHeatChart.XAxisMin = m_specificHeatChart.YAxisMin = m_susceptibilityChart.XAxisMin = m_susceptibilityChart.YAxisMin = 0;

	m_energyChart.SetNumBigTicksY(4);
	m_energyChart.SetNumTicksY(8);
}

CIsingMonteCarloDoc::~CIsingMonteCarloDoc()
{
	StopThreads();
}

BOOL CIsingMonteCarloDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


void CIsingMonteCarloDoc::LaunchThreads()
{
	StopThreads(); // just in case, there should not be any threads running when calling it

	// copy options because the ones from the app can be changed during threads running

	opt = theApp.options;

	// adjust some values to be sure they are ok
	if (opt.lowTemperature < opt.temperatureStep) opt.lowTemperature = opt.temperatureStep;

	if (opt.renormalizationTemperature1 < opt.lowTemperature || opt.renormalizationTemperature1 > opt.highTemperature) opt.renormalizationTemperature1 = opt.lowTemperature;
	if (opt.renormalizationTemperature2 < opt.lowTemperature || opt.renormalizationTemperature2 > opt.highTemperature) opt.renormalizationTemperature2 = (opt.lowTemperature + opt.highTemperature) / 2;
	if (opt.renormalizationTemperature3 < opt.lowTemperature || opt.renormalizationTemperature2 > opt.highTemperature) opt.renormalizationTemperature3 = opt.highTemperature;

	spins = MonteCarlo::SpinMatrix(opt.latticeSize);
	spins.Init(opt);

	unsigned int numThreads = theApp.options.numThreads;
	if (0 == numThreads) opt.numThreads = numThreads = 1;


	finishedCalc = false;
	computing = true;
	displayingRenormalization = false;

	SetTitle(L"Simulating");
	threadsEnded = 0;

	for (unsigned int i = 0; i < numThreads; ++i)
	{
		threadsList.emplace_back(std::make_unique<MonteCarlo::MonteCarloThread>(opt.latticeSize));

		threadsList.back()->doc = this;
		threadsList.back()->opt = opt; // better use the adjusted variant

		if (0 == i)
		{
			threadsList.front()->needsData = true;
			threadsList.front()->setRenormalizationValues = 3;
		}

		threadsList.back()->Start();
	}

	GetView()->StartTimer();
}

void CIsingMonteCarloDoc::StopThreads()
{
	for (const auto &thrd : threadsList) thrd->Terminate = true;
	threadsList.clear();

	computing = false;
	SetTitle(L"Finished");
	CIsingMonteCarloView* view = GetView();
	if (view) {
		view->StopTimer();
		view->Invalidate();
	}
}


// CIsingMonteCarloDoc serialization

void CIsingMonteCarloDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CIsingMonteCarloDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CIsingMonteCarloDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CIsingMonteCarloDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CIsingMonteCarloDoc diagnostics

#ifdef _DEBUG
void CIsingMonteCarloDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIsingMonteCarloDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CIsingMonteCarloDoc commands


MonteCarlo::SpinMatrix CIsingMonteCarloDoc::GetData()
{
	if (threadsList.size()) threadsList.front()->needsData = true;

	if (threadsEnded == opt.numThreads)
	{				
		// grab statistics
		std::vector<std::list<MonteCarlo::Statistics>::iterator> iterators;
		iterators.reserve(opt.numThreads);

		int numStats = 0;
		for (const auto &thrd : threadsList) {
			iterators.push_back(thrd->statsList.begin());
			if (!numStats) numStats = static_cast<int>(thrd->statsList.size());
		}

		statsList.clear();

		// now start to iterate and cumulate
		for (int i = 0; i < numStats; ++i)
		{
			MonteCarlo::Statistics stat;

			bool first = true;
			for (auto &iter : iterators)
			{
				if (first) {
					// this is done to copy temperature and prevent the assertion fail in debug
					stat = *iter;
					first = false;
				}
				else stat += *iter;

				++iter;
			}

			stat /= static_cast<double>(opt.numThreads) * opt.collectionSteps;

			statsList.push_back(stat);
		}

		// cleanup threads
		threadsList.clear();


		finishedCalc = true;
		computing = false;
		SetTitle(L"Charts");
		threadsEnded = 0;

		SetupEnergyChartData();
		SetupMagnetizationChartData();
		SetupSpecificHeatChartData();
		SetupSusceptibilityChartData();

		GetView()->StopTimer();
		GetView()->EndWaitCursor();

		SetupCharts();
	}

	std::lock_guard lock(spinsSection);


	MonteCarlo::SpinMatrix mat{ spins };

	return mat;
}


void CIsingMonteCarloDoc::SetupEnergyChartData()
{
	std::vector<std::pair<double, double>> data;

	data.reserve(statsList.size());

	for (auto &&stat : statsList)
	{
		double val = stat.AvgE / (static_cast<double>(opt.latticeSize) * opt.latticeSize);

		data.push_back(std::make_pair(stat.Temperature, val));
	}

	m_energyChart.clear();

	m_energyChart.AddDataSet(&data, static_cast<float>(opt.chartLineThickness), opt.energyColor);
}


void CIsingMonteCarloDoc::SetupMagnetizationChartData()
{
	std::vector<std::pair<double, double>> data;

	data.reserve(statsList.size());

	for (auto &&stat : statsList)
	{
		double val = stat.AvgAbsM / (static_cast<double>(opt.latticeSize) * opt.latticeSize);

		data.push_back(std::make_pair(stat.Temperature, val));
	}

	m_magnetizationChart.clear();

	m_magnetizationChart.AddDataSet(&data, static_cast<float>(opt.chartLineThickness), opt.magnetizationColor);
}


void CIsingMonteCarloDoc::SetupSpecificHeatChartData()
{
	std::vector<std::pair<double, double>> data;

	data.reserve(statsList.size());

	for (auto &&stat : statsList)
	{
		double val = abs(stat.AvgE2 - stat.AvgE * stat.AvgE) / (stat.Temperature * stat.Temperature * opt.latticeSize * opt.latticeSize);

		data.push_back(std::make_pair(stat.Temperature, val));
	}

	m_specificHeatChart.clear();

	m_specificHeatChart.AddDataSet(&data, static_cast<float>(opt.chartLineThickness), opt.specificHeatColor);
}


void CIsingMonteCarloDoc::SetupSusceptibilityChartData()
{
	std::vector<std::pair<double, double>> data;

	data.reserve(statsList.size());

	for (auto &&stat : statsList)
	{
		double val = abs(stat.AvgM2 - stat.AvgM * stat.AvgM) / (stat.Temperature * opt.latticeSize * opt.latticeSize);

		data.push_back(std::make_pair(stat.Temperature, val));
	}

	m_susceptibilityChart.clear();

	m_susceptibilityChart.AddDataSet(&data, static_cast<float>(opt.chartLineThickness), opt.susceptibilityColor);
}

CIsingMonteCarloView* CIsingMonteCarloDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	while (pos)
	{
		CView* pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CIsingMonteCarloView)))
			return dynamic_cast<CIsingMonteCarloView*>(pView);
	}

	return nullptr;
}


void CIsingMonteCarloDoc::SwitchDisplay()
{
	displayingRenormalization = !displayingRenormalization;

	CIsingMonteCarloView* view = GetView();

	if (displayingRenormalization) {
		dispSpinsRenorm1 = spinsRenorm1;
		dispSpinsRenorm2 = spinsRenorm2;
		dispSpinsRenorm3 = spinsRenorm3;
		view->StartTimer();
		SetTitle(L"Renormalization");
	}
	else {
		view->StopTimer();
		SetTitle(L"Charts");
	}

	view->Invalidate();
}

void CIsingMonteCarloDoc::SetupCharts()
{
	const double lowLimit = floor(opt.lowTemperature);
	const double highLimit = ceil(opt.highTemperature);

	m_susceptibilityChart.XAxisMin = m_specificHeatChart.XAxisMin = m_magnetizationChart.XAxisMin = m_energyChart.XAxisMin = lowLimit;
	m_susceptibilityChart.XAxisMax = m_specificHeatChart.XAxisMax = m_magnetizationChart.XAxisMax = m_energyChart.XAxisMax = highLimit;

	int nrIntervals = static_cast<int>(round(highLimit - lowLimit));

	m_susceptibilityChart.SetNumBigTicksX(nrIntervals);
	m_susceptibilityChart.SetNumTicksX(nrIntervals * 2);

	m_specificHeatChart.SetNumBigTicksX(nrIntervals);
	m_specificHeatChart.SetNumTicksX(nrIntervals * 2);

	m_magnetizationChart.SetNumBigTicksX(nrIntervals);
	m_magnetizationChart.SetNumTicksX(nrIntervals * 2);

	m_energyChart.SetNumBigTicksX(nrIntervals);
	m_energyChart.SetNumTicksX(nrIntervals * 2);

	std::list<CString> xlabels;

	for (int l = static_cast<int>(lowLimit); l <= highLimit; ++l)
	{
		CString label;
		label.Format(L"%d", l);
		xlabels.push_back(label);
	}

	m_susceptibilityChart.SetXLabels(xlabels);
	m_specificHeatChart.SetXLabels(xlabels);
	m_magnetizationChart.SetXLabels(xlabels);
	m_energyChart.SetXLabels(xlabels);

	SetChangeableChartsParams();
}

void CIsingMonteCarloDoc::SetChangeableChartsParams()
{
	m_specificHeatChart.YAxisMax = theApp.options.maxSpecificHeat;
	m_susceptibilityChart.YAxisMax = theApp.options.maxSusceptibility;

	m_specificHeatChart.SetNumBigTicksY(theApp.options.ticksSpecificHeat);
	m_specificHeatChart.SetNumTicksY(theApp.options.ticksSpecificHeat * 2);

	m_susceptibilityChart.SetNumBigTicksY(theApp.options.ticksSusceptibility);
	m_susceptibilityChart.SetNumTicksY(theApp.options.ticksSusceptibility * 2);


	m_energyChart.useSpline = theApp.options.useSplines;
	m_magnetizationChart.useSpline = theApp.options.useSplines;
	m_specificHeatChart.useSpline = theApp.options.useSplines;
	m_susceptibilityChart.useSpline = theApp.options.useSplines;

	for (auto &dataSet : m_energyChart.dataSets.dataSets)
	{
		dataSet.color = theApp.options.energyColor;
		dataSet.lineWidth = static_cast<float>(theApp.options.chartLineThickness);
	}

	for (auto &dataSet : m_magnetizationChart.dataSets.dataSets)
	{
		dataSet.color = theApp.options.magnetizationColor;
		dataSet.lineWidth = static_cast<float>(theApp.options.chartLineThickness);
	}

	for (auto &dataSet : m_specificHeatChart.dataSets.dataSets)
	{
		dataSet.color = theApp.options.specificHeatColor;
		dataSet.lineWidth = static_cast<float>(theApp.options.chartLineThickness);
	}

	for (auto &dataSet : m_susceptibilityChart.dataSets.dataSets)
	{
		dataSet.color = theApp.options.susceptibilityColor;
		dataSet.lineWidth = static_cast<float>(theApp.options.chartLineThickness);
	}

	if (finishedCalc && !displayingRenormalization) GetView()->Invalidate();
}
