#pragma once

namespace MonteCarlo {


	class Statistics
	{
	public:
		double AvgE = 0;
		double AvgM = 0;
		double AvgE2 = 0;
		double AvgM2 = 0;
		double AvgAbsM = 0;

		double Temperature = 0;

		Statistics& operator+=(const Statistics& src);
		Statistics& operator/=(double div);
		void CollectStats(double E, double M);
	};


}
