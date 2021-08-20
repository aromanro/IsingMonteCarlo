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

	protected:
		std::array<StatisticsBin, N> m_bins;
	};

}

