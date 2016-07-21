#include "stdafx.h"
#include "Statistics.h"

#include <cassert>

Statistics::Statistics()
	: 
	AvgE(0),
	AvgM(0),
	AvgE2(0),
	AvgM2(0),
	AvgAbsM(0),
	Temperature(0)
{
}

Statistics::Statistics(const Statistics& src)
	:
	AvgE(src.AvgE),
	AvgM(src.AvgM),
	AvgE2(src.AvgE2),
	AvgM2(src.AvgM2),
	AvgAbsM(src.AvgAbsM),
	Temperature(src.Temperature)
{
}

Statistics::~Statistics()
{
}

Statistics& Statistics::operator=(const Statistics& src)
{	
	AvgE = src.AvgE;
	AvgE2 = src.AvgE2;
	AvgM = src.AvgM;
	AvgM2 = src.AvgM2;
	AvgAbsM = src.AvgAbsM;
	Temperature = src.Temperature;

	return *this;
}

Statistics& Statistics::operator+=(const Statistics& src)
{
	assert(Temperature == src.Temperature);

	AvgE += src.AvgE;
	AvgE2 += src.AvgE2;
	AvgM += src.AvgM;
	AvgM2 += src.AvgM2;
	AvgAbsM += src.AvgAbsM;

	return *this;
}


Statistics& Statistics::operator/=(double div)
{
	AvgE /= div;
	AvgE2 /= div;
	AvgM /= div;
	AvgM2 /= div;
	AvgAbsM /= div;

	return *this;
}



