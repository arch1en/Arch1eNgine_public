#pragma once

#include "Logger.h"

#include <cassert>

#define StaticAssert(Expression, Message)	\
	if (!Expression)	\
	{\
		Log(DebugType::EDT_Assertion, Message);\
		static_assert(Expression);\
	}

#define Assert(Expression, Message)	\
	if (!Expression) \
	{ \
		Log(DebugType::EDT_Assertion, Message); \
		assert(Expression); \
	}

//template<constexpr bool>		// Generic
//struct StaticAssert;
//
//template<>
//struct StaticAssert<true> { 
//
//	StaticAssert(const char* Message)
//	{
//		Log(DebugType::EDT_Assertion, Message);
//	}
//};