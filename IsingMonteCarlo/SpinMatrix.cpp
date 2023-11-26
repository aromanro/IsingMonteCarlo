#include "stdafx.h"
#include "SpinMatrix.h"

#include <cassert>
#include <functional>

namespace MonteCarlo {


	SpinMatrix::SpinMatrix(unsigned int rows, unsigned int columns) noexcept
		: m_rows(rows), Temperature(0), Energy(0), Magnetization(0), J(1.), B(0), RenormalizationsNo(0)
	{
		assert(rows != 0);

		if (0 == columns) columns = rows;

		m_cols = columns;

		m_spins.resize(static_cast<size_t>(rows) * columns, 0);
	}


	SpinMatrix::~SpinMatrix()
	{
		Clear();
	}

	SpinMatrix::SpinMatrix(const SpinMatrix& s)
		: m_rows(s.m_rows), m_cols(s.m_cols), Temperature(s.Temperature), Energy(s.Energy), Magnetization(0), J(s.J), B(s.B), m_spins(s.m_spins), RenormalizationsNo(s.RenormalizationsNo)
	{
		setExpMap();
	}


	SpinMatrix::SpinMatrix(SpinMatrix&& s) noexcept
		: m_rows(s.m_rows), m_cols(s.m_cols), Temperature(s.Temperature), Energy(s.Energy), Magnetization(s.Magnetization), J(s.J), B(s.B), RenormalizationsNo(s.RenormalizationsNo)
	{
		expMap.swap(s.expMap);
		m_spins.swap(s.m_spins);
	}

	SpinMatrix& SpinMatrix::operator=(const SpinMatrix& s)
	{
		if (&s == this) return *this;

		m_rows = s.m_rows;
		m_cols = s.m_cols;
		J = s.J;
		B = s.B;

		RenormalizationsNo = s.RenormalizationsNo;

		Temperature = s.Temperature;
		Energy = s.Energy;

		m_spins = s.m_spins;
		expMap = s.expMap;

		return *this;
	}


	SpinMatrix& SpinMatrix::operator=(SpinMatrix&& s) noexcept
	{
		if (&s == this) return *this;

		m_rows = s.m_rows;
		m_cols = s.m_cols;
		J = s.J;
		B = s.B;

		RenormalizationsNo = s.RenormalizationsNo;

		Temperature = s.Temperature;
		Energy = s.Energy;

		m_spins = s.m_spins;
		expMap.swap(s.expMap);

		return *this;
	}

	void SpinMatrix::Clear()
	{
		m_spins.clear();
		expMap.clear();
	}


	void SpinMatrix::Seed()
	{
		std::random_device rdev{};
		rndEngine.seed(rdev());
	}

	void SpinMatrix::Init(const Options& options)
	{
		const unsigned int size = m_cols * m_rows;

		//Tc at 2.26918531421
		J = options.J;
		B = options.B;

		Seed();

		switch (options.startIsing)
		{
		case Options::IsingStart::InfiniteTemperature:
		{
			Temperature = 10;

			std::uniform_int_distribution<> d{ 0, 1 };

			for (unsigned int i = 0; i < size; ++i) m_spins[i] = (d(rndEngine) ? 1 : -1);

			Energy = 0;

			for (unsigned int row = 0; row < m_rows; ++row)
				for (unsigned int col = 0; col < m_cols; ++col)
				{
					Energy += PartSiteEnergy(row, col);
					Magnetization += GetSpin(row, col);
				}
		}
		break;
		case Options::IsingStart::ZeroTemperature:
			Temperature = 0.1;

			for (unsigned int i = 0; i < size; ++i) m_spins[i] = (options.B > 0 ? -1 : 1);

			Energy = (-2. - options.B) * size;
			Magnetization = size * (options.B > 0 ? -1. : 1.);
			break;
		}

		setExpMap();
	}


	void SpinMatrix::MonteCarloSweep()
	{
		const unsigned int size = m_rows * m_cols;

		std::uniform_int_distribution<unsigned int> rndRow{ 0, m_rows - 1 };
		std::uniform_int_distribution<unsigned int> rndCol{ 0, m_cols - 1 };

		std::uniform_real_distribution<double> dbl_dist{ 0 , 1 };

		for (unsigned int pos = 0; pos < size; ++pos)
		{
			const unsigned int row = rndRow(rndEngine);
			const unsigned int col = rndCol(rndEngine);

			const double energyDif = EnergyDifForFlip(row, col);
			if (energyDif < 0)
			{
				// accept it
				// flip the spin
				m_spins[m_cols * row + col] *= -1;
				Energy += energyDif;
				Magnetization += 2LL * GetSpin(row, col);
			}
			else
			{
				//double val = ExpMinusBetaE(energyDif);
				const double val = expMap[static_cast<unsigned int>(energyDif)];

				if (dbl_dist(rndEngine) < val)
				{
					// accept it
					// flip the spin
					m_spins[m_cols * row + col] *= -1;
					Energy += energyDif;
					Magnetization += 2LL * GetSpin(row, col);
				}
				// else reject it, which means do nothing
			}
		}
	}

	void SpinMatrix::setExpMap()
	{
		expMap.clear();
		const int size = static_cast<int>(9 + B);
		if (size <= 0) return;
		expMap.resize(size, 0.);

		int index = static_cast<int>(B);
		if (index >= 0) expMap[index] = ExpMinusBetaE(B);

		index = static_cast<int>(4 + B);
		if (index >= 0) expMap[index] = ExpMinusBetaE(4. + B);

		index = static_cast<int>(8 + B);
		if (index >= 0) expMap[index] = ExpMinusBetaE(8. + B);
	}

	SpinMatrix SpinMatrix::Renormalize(Options::BlockRenormalizationType type) const
	{
		if (type == Options::BlockRenormalizationType::Majority)
			return RenormalizeMajority();

		return RenormalizeDecimation();
	}

	SpinMatrix SpinMatrix::RenormalizeMajority() const
	{
		// make a matrix of the same size, using the periodic boundary conditions
		const unsigned int halfRows = m_rows / 2;
		const unsigned int halfCols = m_cols / 2;

		SpinMatrix result(m_rows, m_cols);

		for (unsigned int i = 0; i < m_rows; ++i)
		{
			const unsigned int twoiMinusHalfRows = 2 * i - halfRows;
			const unsigned int twoiMinusHalfRowsPlus1 = twoiMinusHalfRows + 1;
			for (unsigned int j = 0; j < m_cols; ++j)
			{
				// count the spins up in the block
				unsigned int spinsUp = 0;
				const unsigned int twojMinusHalfCols = 2 * j - halfCols;
				const unsigned int twojMinusHalfColsPlus1 = twojMinusHalfCols + 1;

				int spin = GetSpinPeriodic(twoiMinusHalfRows, twojMinusHalfCols);

				if (spin > 0) ++spinsUp;
				if (GetSpinPeriodic(twoiMinusHalfRowsPlus1, twojMinusHalfCols) > 0) ++spinsUp;
				if (GetSpinPeriodic(twoiMinusHalfRows, twojMinusHalfColsPlus1) > 0) ++spinsUp;
				if (GetSpinPeriodic(twoiMinusHalfRowsPlus1, twojMinusHalfColsPlus1) > 0) ++spinsUp;

				if (spinsUp > 2) result(i, j) = 1;
				else if (spinsUp < 2) result(i, j) = -1;
				else result(i, j) = spin; // two spins up, two spins down, just let decimation decide, a random value is sometimes used instead
			}
		}

		result.RenormalizationsNo = RenormalizationsNo + 1;

		return result;
	}

	SpinMatrix SpinMatrix::RenormalizeDecimation() const
	{
		// make a matrix of the same size, using the periodic boundary conditions
		const unsigned int halfRows = m_rows / 2;
		const unsigned int halfCols = m_cols / 2;

		SpinMatrix result(m_rows, m_cols);

		// decimation, just pick the value in the left upper corner of the block
		for (unsigned int i = 0; i < m_rows; ++i)
		{
			const unsigned int twoiMinusHalfRows = 2 * i - halfRows;
			for (unsigned int j = 0; j < m_cols; ++j)
				result(i, j) = GetSpinPeriodic(twoiMinusHalfRows, 2 * j - halfCols);
		}

		result.RenormalizationsNo = RenormalizationsNo + 1;

		return result;
	}

	void SpinMatrix::RenormalizeInPlace(Options::BlockRenormalizationType type)
	{
		*this = Renormalize(type);
	}

}