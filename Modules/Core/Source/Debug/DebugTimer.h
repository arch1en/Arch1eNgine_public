////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Debugging class for measuring time between functions. Singleton.
//  @version    : 1.0.0
//
////////////////////////////////////////

#pragma once

#include <ctime>

class DebugTimer
{
public:

	DebugTimer() = default;
	~DebugTimer() = default;

	void Start();
	void Stop();
	double Check();

	void ShowResult();

private:

	clock_t StartTime;
	clock_t StopTime;
	double DifferenceTime;
};