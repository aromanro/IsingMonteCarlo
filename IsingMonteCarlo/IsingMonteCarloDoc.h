
// IsingMonteCarloDoc.h : interface of the CIsingMonteCarloDoc class
//


#pragma once

#include <mutex>
#include <list>
#include <memory>
#include <atomic>

#include "MonteCarloThread.h"
#include "SpinMatrix.h"

#include "Chart.h"

class CIsingMonteCarloView;

class CIsingMonteCarloDoc : public CDocument
{
protected: // create from serialization only
	CIsingMonteCarloDoc();
	DECLARE_DYNCREATE(CIsingMonteCarloDoc)

// Attributes
public:
	std::mutex spinsSection;

	std::list<std::unique_ptr<MonteCarlo::MonteCarloThread>> threadsList;
	
	std::atomic_int threadsEnded{ 0 };

	MonteCarlo::SpinMatrix spins;

	MonteCarlo::SpinMatrix spinsRenorm1;
	MonteCarlo::SpinMatrix spinsRenorm2;
	MonteCarlo::SpinMatrix spinsRenorm3;

	MonteCarlo::SpinMatrix dispSpinsRenorm1;
	MonteCarlo::SpinMatrix dispSpinsRenorm2;
	MonteCarlo::SpinMatrix dispSpinsRenorm3;

	std::list<MonteCarlo::Statistics> statsList;

	bool computing;
	bool finishedCalc;
	bool displayingRenormalization;

	Options opt;

	Chart m_energyChart;
	Chart m_magnetizationChart;
	Chart m_specificHeatChart;
	Chart m_susceptibilityChart;

// Operations
public:
	void LaunchThreads();
	void StopThreads();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CIsingMonteCarloDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
protected:
	void SetupCharts();
	void SetupEnergyChartData();
	void SetupMagnetizationChartData();
	void SetupSpecificHeatChartData();
	void SetupSusceptibilityChartData();
public:
	MonteCarlo::SpinMatrix GetData();
	CIsingMonteCarloView* GetView();
	void SwitchDisplay();
	void SetChangeableChartsParams();
};
