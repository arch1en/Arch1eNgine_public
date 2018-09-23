////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Benchmarkging class for measuring time between functions. Singleton.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"
#include "BenchmarkTimer.h"

void BenchmarkTimer::Start()
{
	StartTime = std::clock();
}

void BenchmarkTimer::Stop()
{
	StopTime = std::clock();
}

double BenchmarkTimer::Check()
{
	if (StartTime <= 0.0 || StopTime <= 0.0)
	{
		return 0.0;
	}

	return StopTime - StartTime;
}

double BenchmarkTimer::StopAndCheck()
{
	Stop();
	return Check();
}