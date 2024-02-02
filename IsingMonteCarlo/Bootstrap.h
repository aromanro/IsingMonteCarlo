#pragma once
#include "StatisticsBinCollection.h"

namespace Statistics
{

    // not complete yet

    template<unsigned int N> class Bootstrap :
        public StatisticsBinCollection<N>
    {
    public:
        explicit Bootstrap(long long SampleSize) : sampleSize(SampleSize), count(0), binExcl(sampleSize / N) {}

        void Reset()
        {
            count = 0;
            StatisticsBinCollection<N>::Reset();
        }

        void AddValue(double value)
        {
            for (int bin = 0; bin < N; ++bin)
                if (bin != count / binExcl)
                    m_bins[bin].AddValue(value);

            ++count;
        }

        long long int GetSamplesNumber() const { return count; }

    protected:
        long long sampleSize;
        long long int count;
        long long binExcl;
    };

}

