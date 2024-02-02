#pragma once

namespace Statistics
{

	class StatisticsBin
	{
	public:
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

		double GetAverage() const { return sum / static_cast<double>(count); }

		double GetAverage2() const { return sum2 / static_cast<double>(count); }

		double GetVariance() const 
		{ 
			const double m = GetAverage();

			return GetAverage2() - m * m;
		}

		double GetStandardDeviation() const
		{
			return sqrt(GetVariance());
		}

		double GetNonParametricStandardDeviation() const
		{
			return static_cast<double>(count) * GetStandardDeviation() / (static_cast<double>(count) - 1.);
		}


		long long int GetSamplesNumber() const { return count; }

	private:
		double sum = 0;
		double sum2 = 0;
		long long int count = 0;
	};

}

