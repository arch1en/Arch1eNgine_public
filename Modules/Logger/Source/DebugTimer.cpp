////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Debugging class for measuring time between functions. Singleton.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"
#include "DebugTimer.h"


#include "LogSystem.h"
#include "LogSystemUtilities.h"

void DebugTimer::Start()
{
	StartTime = std::clock();
}

void DebugTimer::Stop()
{
	StopTime = std::clock();

}

double DebugTimer::Check()
{
	if (StartTime <= 0.0 || StopTime <= 0.0)
	{
		return 0.0;
	}

	return StopTime - StartTime;
}

void DebugTimer::ShowResult()
{
	double Difference = Check();
	if (Difference > 0.0)
	{
		Log(LogType::Notice, "Time Difference : %f ms", Check());
	}
	else
	{
		Log(LogType::Notice, "Time Difference : %f ms", Check());
	}
}
