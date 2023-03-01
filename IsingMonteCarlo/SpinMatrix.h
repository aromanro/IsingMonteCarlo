#pragma once

#include "Options.h"

#include <random>

namespace MonteCarlo {


	class SpinMatrix
	{
	protected:
		unsigned int m_rows;
		unsigned int m_cols;
		std::vector<int> m_spins;


		double Energy;
		double Magnetization;

		double J;
		double B;

		std::mt19937 rndEngine;

		std::vector<double> expMap;

		unsigned int RenormalizationsNo;

		// this is faster but works only for limited distance around boundary, for the periodic one for displaying use the next one
		inline unsigned int GetFastIndex(int index, int size) const {
			if (index >= 0)
			{
				if (index >= size) return index - size;

				return index;
			}


			return index + size;
		}

		inline unsigned int GetPeriodicIndex(int index, unsigned int size) const {
			if (index >= 0) return index % size;

			return size - 1 - abs(index + 1) % size;
		}

		// use neighbor for accessing neighbors - automatically adjusts for periodic boundary conditions
		inline int neighbor(int row, int col) const {
			return m_spins[GetFastIndex(row, m_cols) * m_cols + GetFastIndex(col, m_rows)];
		}

		inline int NeighborContribution(int row, int col) const {
			return neighbor(row - 1, col) + neighbor(row + 1, col) + neighbor(row, col - 1) + neighbor(row, col + 1);
		}

		// temporary, will be precalculated because a limited number of values is needed (3)
		inline double ExpMinusBetaE(double E) const {
			return exp(-1. / Temperature * E);
		}

		inline double EnergyDifForFlip(int row, int col) const {
			return 2. * GetSpin(row, col) * (J * NeighborContribution(row, col) + B);
		}

		inline double SiteEnergy(int row, int col) const {
			return -GetSpin(row, col) * (J * NeighborContribution(row, col) + B);
		}

		inline double PartSiteEnergy(int row, int col) const {
			return -GetSpin(row, col) * (J * (static_cast<double>(neighbor(1ULL + row, col)) + neighbor(row, 1ULL + col)) + B);
		}

	public:
		double Temperature;


		SpinMatrix(unsigned int rows, unsigned int columns = 0) noexcept;
		~SpinMatrix();

		SpinMatrix(const SpinMatrix& s);
		SpinMatrix(SpinMatrix&& s) noexcept;
		SpinMatrix& operator=(const SpinMatrix& s);
		SpinMatrix& operator=(SpinMatrix&& s) noexcept;

		inline unsigned int getCols() const { return m_cols; }
		inline unsigned int getRows() const { return m_rows; }

		void Init(const Options& options);

		inline int& operator()(int row, int col) {
			return m_spins[m_cols*row + col];
		}

		inline int GetSpin(int row, int col) const {
			return m_spins[m_cols*row + col];
		}

		inline int GetSpinPeriodic(int row, int col) const {
			return m_spins[GetPeriodicIndex(row, m_cols) * m_cols + GetPeriodicIndex(col, m_rows)];
		}


		inline double GetEnergy() const { return Energy; }
		inline double GetMagnetization() const { return Magnetization; }

		unsigned int GetRenormalizationsNumber() const { return RenormalizationsNo; }

		void MonteCarloSweep();

		void Clear();
		void setExpMap();

		void Seed();

		SpinMatrix Renormalize(Options::BlockRenormalizationType type = Options::BlockRenormalizationType::Majority) const;
		void RenormalizeInPlace(Options::BlockRenormalizationType type = Options::BlockRenormalizationType::Majority);
	};

}