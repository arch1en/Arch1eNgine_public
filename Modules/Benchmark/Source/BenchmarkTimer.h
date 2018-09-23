////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Benchmarkging class for measuring time between functions. Singleton.
//  @version    : 1.0.0
//
////////////////////////////////////////

#pragma once

#include <ctime>

class BenchmarkTimer
{
public:

	void Start();
	void Stop();
	double StopAndCheck();
	double Check();

private:

	BenchmarkTimer() = default;
	~BenchmarkTimer() = default;

	clock_t StartTime;
	clock_t StopTime;
	double DifferenceTime;
};