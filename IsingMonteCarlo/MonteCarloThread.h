#pragma once
#include "ComputationThread.h"
#include "SpinMatrix.h"
#include "Statistics.h"

#include <atomic>
#include <list>

class CIsingMonteCarloDoc;

namespace MonteCarlo {

	class Statistics;

	class MonteCarloThread :
		public ComputationThread
	{
	protected:
		SpinMatrix spins;

	public:
		CIsingMonteCarloDoc *doc;
		Options opt;

		std::atomic_bool needsData;
		std::atomic_bool Terminate;

		int setRenormalizationValues;

		std::list<Statistics> statsList;

		MonteCarloThread(unsigned int rows = 32, unsigned int columns = 0);
		virtual ~MonteCarloThread();

	protected:
		virtual void Calculate();
		void PassData();
		void PassStats(const Statistics& stats);

		bool RunSweeps(unsigned int steps, bool collectStats = false);
		bool TemperatureStep(double temperature);
	};

}