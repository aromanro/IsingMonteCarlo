
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
	DECLARE_MESSAGE_MAP()
public:
	MonteCarlo::SpinMatrix GetData();
	CIsingMonteCarloView* GetView();
	void SwitchDisplay();
	void SetChangeableChartsParams();

	// Attributes
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
	void LaunchThreads();
	void StopThreads();

// Overrides
	BOOL OnNewDocument() override;
	void Serialize(CArchive& ar) override;
#ifdef SHARED_HANDLERS
	void InitializeSearchContent() override;
	void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) override;
#endif // SHARED_HANDLERS

// Implementation
	 ~CIsingMonteCarloDoc() override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

// Generated message map functions
private:
#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	void SetupCharts();
	void SetupEnergyChartData();
	void SetupMagnetizationChartData();
	void SetupSpecificHeatChartData();
	void SetupSusceptibilityChartData();
};
