#pragma once

#include "Debugger.h"

template<bool>		// Generic
struct StaticAssert;

template<>
struct StaticAssert<true> { 

	StaticAssert(const char* Message)
	{
		Log(DebugType::EDT_Assertion, Message);
	}
};
