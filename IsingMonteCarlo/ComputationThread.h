#pragma once

#include <thread>

class ComputationThread
{
public:
	void Start();
	void join();

protected:
	virtual ~ComputationThread();

	virtual void Calculate() = 0;

	std::thread mThread;
};

