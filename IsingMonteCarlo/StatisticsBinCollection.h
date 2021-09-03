#pragma once

#include <array>
#include "StatisticsBin.h"

namespace Statistics
{

	template<unsigned int N> class StatisticsBinCollection
	{
		void Reset()
		{
			for (unsigned int i = 0; i < N; ++i)
				m_bins[i].Reset();
		}

		void AddValue(unsigned int bin, double val)
		{
			m_bins[bin].AddValue(val);
		}

		double GetAvgAverage() const
		{
			double avg = 0;
			for (int bin = 0; bin < N; ++bin)
				avg += bin[i].GetAverage();

			return avg / N;
		}

		double GetAvgVariance() const
		{
			double var = 0;

			for (int bin = 0; bin < N; ++bin)
				var += bin[i].GetVariance();

			return var / N;
		}

		double GetAvgAverageVariance() const
		{
			const double mm = GetAvgAverage();

			double avg2 = 0;
			for (int bin = 0; bin < N; ++bin)
			{
				const double m = bin[i].GetAverage();
				avg2 += m * m;
			}

			avg2 /= N;

			return  avg2 - mm * mm;
		}

		double GetAvgVarianceVariance() const
		{
			const double mm = GetAvgVariance();

			double avg2 = 0;
			for (int bin = 0; bin < N; ++bin)
			{
				const double m = bin[i].GetVariance();
				avg2 += m * m;
			}

			avg2 /= N;

			return  avg2 - mm * mm;
		}

	protected:
		std::array<StatisticsBin, N> m_bins;
	};

}

