#include "stdafx.h"
#include "SpinMatrix.h"

#include <cassert>
#include <functional>




SpinMatrix::SpinMatrix(unsigned int rows, unsigned int columns)
	: m_rows(rows), Temperature(0), Energy(0), Magnetization(0), expMap(nullptr), J(1.), B(0), RenormalizationsNo(0)
{
	assert(rows != 0);

	if (0 == columns) columns = rows;

	m_cols = columns;

	m_spins = new int[rows * columns];
}


SpinMatrix::~SpinMatrix()
{
	Clear();
}

SpinMatrix::SpinMatrix(const SpinMatrix& s)	
	: m_rows(s.m_rows), m_cols(s.m_cols), Temperature(s.Temperature), Energy(s.Energy), J(s.J), B(s.B), m_spins(nullptr), expMap(nullptr), RenormalizationsNo(s.RenormalizationsNo)
{
	unsigned int size = m_rows * m_cols;

	assert(size > 0);
	m_spins = new int[size];

	for (unsigned int i = 0; i < size; ++i) m_spins[i] = s.m_spins[i];

	setExpMap();
}


SpinMatrix::SpinMatrix(SpinMatrix&& s)	
	: m_rows(s.m_rows), m_cols(s.m_cols), Temperature(s.Temperature), Energy(s.Energy), J(s.J), B(s.B), m_spins(s.m_spins), expMap(s.expMap), RenormalizationsNo(s.RenormalizationsNo)
{
	s.m_spins = nullptr;
	s.expMap = nullptr;
}

SpinMatrix& SpinMatrix::operator=(const SpinMatrix& s)
{
	if (&s == this) return *this;

	Clear();

	m_rows = s.m_rows;
	m_cols = s.m_cols;
	J = s.J;
	B = s.B;

	RenormalizationsNo = s.RenormalizationsNo;

	Temperature = s.Temperature;
	Energy = s.Energy;

	unsigned int size = m_rows * m_cols;
	m_spins = new int[size];
	
	for (unsigned int i = 0; i < size; ++i) m_spins[i] = s.m_spins[i];

	setExpMap();

	return *this;
}


SpinMatrix& SpinMatrix::operator=(SpinMatrix&& s)
{
	if (&s == this) return *this;

	Clear();

	m_rows = s.m_rows;
	m_cols = s.m_cols;
	J = s.J;
	B = s.B;

	RenormalizationsNo = s.RenormalizationsNo;

	Temperature = s.Temperature;
	Energy = s.Energy;

	m_spins = s.m_spins;
	expMap = s.expMap;

	s.m_spins = nullptr;
	s.expMap = nullptr;

	return *this;
}

void SpinMatrix::Clear()
{
	delete[] m_spins;
	delete[] expMap;
	m_spins = nullptr;
	expMap = nullptr;
}


void SpinMatrix::Seed()
{
	std::random_device rdev{};
	rndEngine.seed(rdev());
}

void SpinMatrix::Init(const Options& options)
{
	unsigned int size = m_cols * m_rows;

	//Tc at 2.26918531421
	J = options.J;
	B = options.B;

	Seed();
	
	switch (options.startIsing)	
	{
	case Options::IsingStart::InfiniteTemperature:
		{
		    Temperature = 10;//*0.1*/2.26918531421/0.99;

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
		Magnetization = size * (options.B > 0 ? -1 : 1);
		break;
	}

	setExpMap();
}


void SpinMatrix::MonteCarloSweep()
{
	unsigned int size = m_rows * m_cols;

	std::uniform_int_distribution<unsigned int> rndRow{ 0, m_rows - 1 };
	std::uniform_int_distribution<unsigned int> rndCol{ 0, m_cols - 1 };

	std::uniform_real_distribution<double> dbl_dist{ 0 , 1 };

	for (unsigned int pos = 0; pos < size; ++pos)
	{
		unsigned int row = rndRow(rndEngine);
		unsigned int col = rndCol(rndEngine);

		double energyDif = EnergyDifForFlip(row, col);
		if (energyDif < 0)
		{
			// accept it
			// flip the spin
			m_spins[m_cols*row + col] *= -1;
			Energy += energyDif;
			Magnetization += 2 * GetSpin(row, col);
		}
		else
		{
			//double val = ExpMinusBetaE(energyDif);
			double val = expMap[(unsigned int)energyDif];

			if (dbl_dist(rndEngine) < val)
			{
				// accept it
				// flip the spin
				m_spins[m_cols*row + col] *= -1;
				Energy += energyDif;
				Magnetization += 2 * GetSpin(row, col);
			}
			// else reject it, which means do nothing
		}
	}
}

void SpinMatrix::setExpMap()
{
	delete[] expMap;
	expMap = nullptr;

	int size = (int)(9 + B);

	if (size <= 0) return;

	expMap = new double[size];

	for (int i = 0; i < size; ++i) expMap[i] = 0;

	int index = (int)(B);
	if (index >= 0) expMap[index] = ExpMinusBetaE(B);
	
	index = (int)(4 + B);
	if (index >= 0) expMap[index] = ExpMinusBetaE(4+B);

	index = (int)(8 + B);
	if (index >= 0) expMap[index] = ExpMinusBetaE(8+B);
}

SpinMatrix SpinMatrix::Renormalize(Options::BlockRenormalizationType type) const
{
	// make a matrix of the same size, using the periodic boundary conditions
	unsigned int new_rows = m_rows;
	unsigned int new_cols = m_cols;
	
	SpinMatrix result(new_rows, new_cols);

	if (type == Options::BlockRenormalizationType::Majority)
	{
		for (unsigned int i = 0; i < new_rows; ++i)
			for (unsigned int j = 0; j < new_cols; ++j)
			{
				// count the spins up in the block
				unsigned int spinsUp = 0;

				int spin = GetSpinPeriodic(2 * i - m_rows/2, 2 * j - m_cols/2);

				if (spin > 0) ++spinsUp;
				if (GetSpinPeriodic(2 * i + 1 - m_rows/2, 2 * j - m_cols/2) > 0) ++spinsUp;
				if (GetSpinPeriodic(2 * i - m_rows/2, 2 * j + 1 - m_cols/2) > 0) ++spinsUp;
				if (GetSpinPeriodic(2 * i + 1 - m_rows/2, 2 * j + 1 - m_cols/2) > 0) ++spinsUp;

				if (spinsUp > 2) result(i, j) = 1;
				else if (spinsUp < 2) result(i, j) = -1;
				else result(i, j) = spin; // two spins up, two spins down, just let decimation decide, a random value is sometimes used instead
			}
	}
	else
	{
		// decimation, just pick the value in the left upper corner of the block
		for (unsigned int i = 0; i < new_rows; ++i)
			for (unsigned int j = 0; j < new_cols; ++j)
				result(i, j) = GetSpinPeriodic(2 * i - m_rows/2, 2 * j - m_cols/2);
	}

	result.RenormalizationsNo = RenormalizationsNo + 1;

	return result;
}

void SpinMatrix::RenormalizeInPlace(Options::BlockRenormalizationType type)
{
	*this = Renormalize(type);
}