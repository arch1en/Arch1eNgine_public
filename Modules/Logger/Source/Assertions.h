#pragma once

#include "LogSystem.h"
#include <assert.h>

#define StaticAssert(Expression, Message)				\
	if(!(Expression))									\
	{													\
		Log(LogType::Assertion, Message);				\
		static_assert(Expression);						\
	}													\

#define Assert(Expression, Message)						\
	if(!(Expression))									\
	{													\
		Log(LogType::Assertion, Message);				\
		assert(Expression);								\
	}	