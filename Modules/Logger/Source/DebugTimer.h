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

class MODULE_API DebugTimer
{
public:

	static DebugTimer& GetInstance();

	void Start();
	void Stop();
	double Check();

	void ShowResult();

private:

	DebugTimer() = default;
	~DebugTimer() = default;

	clock_t StartTime;
	clock_t StopTime;
	double DifferenceTime;
};