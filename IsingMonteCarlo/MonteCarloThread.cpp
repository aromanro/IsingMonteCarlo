#include "stdafx.h"
#include "MonteCarloThread.h"
#include "IsingMonteCarlo.h"
#include "Options.h"

#include "IsingMonteCarloDoc.h"


#include <string>
#include <strstream>

MonteCarloThread::MonteCarloThread(unsigned int rows, unsigned int columns)
	: spins(rows, columns), doc(nullptr), Terminate(false), opt(theApp.options), needsData(false)
{	
	spins.Init(opt);
}


MonteCarloThread::~MonteCarloThread()
{
	if (mThread.joinable()) mThread.join();
}


void MonteCarloThread::Calculate()
{
	// warmup loop
	spins.Temperature = opt.lowTemperature;
	spins.setExpMap();
	for (unsigned long i = 0; i < opt.warmupSteps; ++i)
	{
		spins.MonteCarloSweep();

		if (Terminate)
		{
			if (doc) ++doc->threadsEnded;
			return;
		}

		PassData();
	}

	double tempStep = opt.temperatureStep;
	double startTemp = opt.lowTemperature;
	double endTemp = opt.highTemperature;

	if (opt.startIsing == Options::IsingStart::InfiniteTemperature)
	{
		std::swap(startTemp, endTemp);
		tempStep *= -1;
	}

	for (double temperature = startTemp; opt.startIsing == Options::IsingStart::ZeroTemperature ? temperature < endTemp : temperature > endTemp; temperature += tempStep)
	{
		if (!TemperatureStep(temperature)) return;

		// special case, calculate for renormalization temperature, too
		if ((temperature < opt.renormalizationTemperature1 && opt.renormalizationTemperature1 < temperature + tempStep) || (temperature > opt.renormalizationTemperature1 && opt.renormalizationTemperature1 > temperature + tempStep))
		{
			if (!TemperatureStep(opt.renormalizationTemperature1)) return;
		}
		if ((temperature < opt.renormalizationTemperature2 && opt.renormalizationTemperature2 < temperature + tempStep) || (temperature > opt.renormalizationTemperature2 && opt.renormalizationTemperature2 > temperature + tempStep))
		{
			if (!TemperatureStep(opt.renormalizationTemperature2)) return;
		}
		if ((temperature < opt.renormalizationTemperature3 && opt.renormalizationTemperature3 < temperature + tempStep) || (temperature > opt.renormalizationTemperature3 && opt.renormalizationTemperature3 > temperature + tempStep))
		{
			if (!TemperatureStep(opt.renormalizationTemperature3)) return;
		}
	}


	if (doc) ++doc->threadsEnded;
}


bool MonteCarloThread::TemperatureStep(double temperature)
{
	spins.Temperature = temperature;
	spins.setExpMap();

	// equilibration
	if (!RunSweeps(opt.equilibrationSteps))
	{
		if (doc) ++doc->threadsEnded;
		return false;
	}

	// collect statistics
	if (!RunSweeps(opt.collectionSteps, true))
	{
		if (doc) ++doc->threadsEnded;
		return false;
	}

	return true;
}

bool MonteCarloThread::RunSweeps(unsigned int steps, bool collectStats)
{
	Statistics stats;

	for (unsigned long int i = 0; i < steps; ++i)
	{
		spins.MonteCarloSweep();

		if (collectStats)
		{
			double E = spins.GetEnergy();
			double M = spins.GetMagnetization();

			stats.AvgE += E;
			stats.AvgE2 += E * E;
			stats.AvgM += M;
			stats.AvgM2 += M * M;
			stats.AvgAbsM += abs(M);
		}

		if (Terminate) return false;

		PassData();
	}

	if (collectStats)
	{
		stats.Temperature = spins.Temperature;
		
		// pass them to the main thread
		PassStats(stats);
	}

	return true;
}


void MonteCarloThread::PassData()
{
	// copy the value back to main thread
	if (needsData && doc) {
		needsData = false;

		std::lock_guard<std::mutex> lock(doc->spinsSection);

		doc->spins = spins;

		if (opt.renormalizationTemperature1 == spins.Temperature)
			doc->spinsRenorm1 = spins;
		if (opt.renormalizationTemperature2 == spins.Temperature)
			doc->spinsRenorm2 = spins;
		if (opt.renormalizationTemperature3 == spins.Temperature)
			doc->spinsRenorm3 = spins;
	}
}

void MonteCarloThread::PassStats(const Statistics& stats)
{
	statsList.push_back(stats);
}
