#pragma once

#include "Logger.h"

#define StaticAssert(Expression, Message)				\
	if(!Expression)										\
	{													\
		Log(LogType::Assertion, Message);				\
		static_assert(Expression, Message);				\
	}													\

#define Assert(Expression, Message)						\
	if(!Expression)										\
	{													\
		Log(LogType::Assertion, Message);				\
		assert(Expression, Message);					\
	}	