#pragma once
class Statistics
{
public:
	double AvgE;
	double AvgM;
	double AvgE2;
	double AvgM2;
	double AvgAbsM;

	double Temperature;

	Statistics();
	~Statistics();

	Statistics(const Statistics& src);

	Statistics& operator=(const Statistics& src);

	Statistics& operator+=(const Statistics& src);
	Statistics& operator/=(double div);	
	void CollectStats(double E, double M);
};

