#pragma once

namespace Statistics
{

	class StatisticsBin
	{
	public:
		StatisticsBin() : sum(0), sum2(0), count(0) {}

		void Reset()
		{
			count = 0;
			sum = sum2 = 0.;
		}

		// naive algorithms should be enough for the purpose of the example programs

		void AddValue(double val)
		{
			sum += val;
			sum2 += val * val;
			++count;
		}

		double GetAverage() const { return sum / count; }

		double GetAverage2() const { return sum2 / count; }

		double GetVariance() const { return (sum2 - sum * sum / count) / (count - 1); }

		long long int GetValuesNumber() const { return count; }

	protected:
		double sum;
		double sum2;
		long long int count;
	};

}

