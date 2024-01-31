#include "stdafx.h"
#include "Statistics.h"

#include <cassert>
#include <algorithm>


namespace MonteCarlo {

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

	void Statistics::CollectStats(double E, double M)
	{
		AvgE += E;
		AvgE2 += E * E;
		AvgM += M;
		AvgM2 += M * M;
		AvgAbsM += abs(M);
	}

}