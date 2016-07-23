#include "stdafx.h"
#include "Options.h"

#include "IsingMonteCarlo.h"


Options::Options()
	: J(1.), B(0), startIsing(ZeroTemperature),
	warmupSteps(100), equilibrationSteps(100), collectionSteps(300),
	temperatureStep(0.05), lowTemperature(1), highTemperature(5),
	numThreads(4), renormalizationTemperature2(2.4), renormalizationTemperature1(2.25), renormalizationTemperature3(2.7),
	latticeSize(256),
	renormalizationType(Majority),
	MaxRenormalizations(6),
	timerInterval(100),
	spinUpColor(RGB(0, 0, 255)),
	spinDownColor(RGB(255, 0, 0)),
	chartLineThickness(1),
	energyColor(RGB(255, 0, 0)), magnetizationColor(RGB(0, 0, 255)), specificHeatColor(RGB(0, 255, 0)), susceptibilityColor(RGB(0, 255, 255)),
	maxSpecificHeat(4), maxSusceptibility(150),
	ticksSpecificHeat(4),
	ticksSusceptibility(3),
	useSplines(true)
{
}


Options::~Options()
{
}

void Options::Load()
{
	B = GetDouble(L"MagneticField", 0),
	warmupSteps = theApp.GetProfileInt(L"options", L"warmupSteps", 100);
	equilibrationSteps = theApp.GetProfileInt(L"options", L"equilibrationSteps", 100);
	collectionSteps = theApp.GetProfileInt(L"options", L"collectionSteps", 300);
	temperatureStep = GetDouble(L"temperatureStep", 0.05);
	lowTemperature = GetDouble(L"lowTemperature", 1);
	highTemperature = GetDouble(L"highTemperature", 5);
	numThreads = theApp.GetProfileInt(L"options", L"numThreads", 4);
	renormalizationTemperature1 = GetDouble(L"renormalizationTemperature1", 2.25);
	renormalizationTemperature2 = GetDouble(L"renormalizationTemperature2", 2.38);
	renormalizationTemperature3 = GetDouble(L"renormalizationTemperature3", 2.7);
	latticeSize = theApp.GetProfileInt(L"options", L"latticeSize", 256);
	renormalizationType = (BlockRenormalizationType)theApp.GetProfileInt(L"options", L"renormalizationType", (int)Majority);
	MaxRenormalizations = theApp.GetProfileInt(L"options", L"maxRenormalizations", 6);
	timerInterval = theApp.GetProfileInt(L"options", L"timerInterval", 100);
	spinUpColor = GetColor(L"spinUpColor", RGB(0, 0, 255));
	spinDownColor = GetColor(L"spinDownColor", RGB(255, 0, 0));
	chartLineThickness = theApp.GetProfileInt(L"options", L"chartLineThickness", 1);
	energyColor = GetColor(L"energyColor", RGB(255, 0, 0));
	magnetizationColor = GetColor(L"magnetizationColor", RGB(0, 0, 255));
	specificHeatColor = GetColor(L"specificHeatColor", RGB(0, 255, 0));
	susceptibilityColor = GetColor(L"susceptibilityColor", RGB(0, 255, 255));
	maxSpecificHeat = theApp.GetProfileInt(L"options", L"maxSpecificHeat", 4);
	maxSusceptibility = theApp.GetProfileInt(L"options", L"maxSusceptibility", 150);
	ticksSpecificHeat = theApp.GetProfileInt(L"options", L"ticksSpecificHeat", 4);
	ticksSusceptibility = theApp.GetProfileInt(L"options", L"ticksSusceptibility", 3);
	useSplines = (theApp.GetProfileInt(L"options", L"useSplines", 1) ? true : false);
}

void Options::Save()
{
	theApp.WriteProfileBinary(L"options", L"MagneticField", (LPBYTE)&B, sizeof(double));
	theApp.WriteProfileInt(L"options", L"warmupSteps", warmupSteps);
	theApp.WriteProfileInt(L"options", L"equilibrationSteps", equilibrationSteps);
	theApp.WriteProfileInt(L"options", L"collectionSteps", collectionSteps);
	theApp.WriteProfileBinary(L"options", L"temperatureStep", (LPBYTE)&temperatureStep, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"lowTemperature", (LPBYTE)&lowTemperature, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"highTemperature", (LPBYTE)&highTemperature, sizeof(double));
	theApp.WriteProfileInt(L"options", L"numThreads", numThreads);
	theApp.WriteProfileBinary(L"options", L"renormalizationTemperature1", (LPBYTE)&renormalizationTemperature1, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"renormalizationTemperature2", (LPBYTE)&renormalizationTemperature2, sizeof(double));
	theApp.WriteProfileBinary(L"options", L"renormalizationTemperature3", (LPBYTE)&renormalizationTemperature3, sizeof(double));
	theApp.WriteProfileInt(L"options", L"latticeSize", latticeSize);
	theApp.WriteProfileInt(L"options", L"renormalizationType", (int)renormalizationType);
	theApp.WriteProfileInt(L"options", L"maxRenormalizations", MaxRenormalizations);
	theApp.WriteProfileInt(L"options", L"timerInterval", timerInterval);
	theApp.WriteProfileBinary(L"options", L"spinUpColor", (LPBYTE)&spinUpColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"spinDownColor", (LPBYTE)&spinDownColor, sizeof(COLORREF));
	theApp.WriteProfileInt(L"options", L"chartLineThickness", chartLineThickness);
	theApp.WriteProfileBinary(L"options", L"energyColor", (LPBYTE)&energyColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"magnetizationColor", (LPBYTE)&magnetizationColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"specificHeatColor", (LPBYTE)&specificHeatColor, sizeof(COLORREF));
	theApp.WriteProfileBinary(L"options", L"susceptibilityColor", (LPBYTE)&susceptibilityColor, sizeof(COLORREF));
	theApp.WriteProfileInt(L"options", L"maxSpecificHeat", maxSpecificHeat);
	theApp.WriteProfileInt(L"options", L"maxSusceptibility", maxSusceptibility);
	theApp.WriteProfileInt(L"options", L"ticksSpecificHeat", ticksSpecificHeat);
	theApp.WriteProfileInt(L"options", L"ticksSusceptibility", ticksSusceptibility);
	theApp.WriteProfileInt(L"options", L"useSplines", useSplines ? 1 : 0);
}

COLORREF Options::GetColor(LPCTSTR param, COLORREF defval)
{
	UINT sz = 0;
	LPBYTE buf = NULL;

	if (theApp.GetProfileBinary(L"options", param, &buf, &sz))
	{
		if (sizeof(COLORREF) == sz)	return *((COLORREF*)buf);
		delete[] buf;
	}

	return defval;
}


double Options::GetDouble(LPCTSTR param, double defval)
{
	double val = defval;

	UINT sz = 0;
	LPBYTE buf = NULL;

	if (theApp.GetProfileBinary(L"options", param, &buf, &sz))
	{
		if (sizeof(double) == sz) val = *((double*)buf);
		delete[] buf;
	}

	return val;
}