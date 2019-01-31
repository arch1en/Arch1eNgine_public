#ifndef LOGGER_IMPL_H
#define LOGGER_IMPL_H

#ifdef PLATFORM_WIN32
#include <windows.h>
#endif

#include <cstdarg>

#include "LogSystemUtilities.h"

class Logger_Impl
{
public:
	Logger_Impl();
	virtual ~Logger_Impl();

	virtual void Log_Internal(const char* InFilePath, int InLineNumber, LogType InDebugType, const char* InLogDomain, int InLogVerbosity, const char* InMessage, ...);

protected:

	/**
	*   Changes text color on the console. Platform specific.
	*
	*   @param  InForeground    : Sets foreground color of a text (default 15 = White).
	*   @param  InBackground    : Sets background color of a text (default 0 = Black).
	*
	*/
	void SetMessageColor(int InForeground = 15, int InBackground = 0);

#ifdef PLATFORM_WIN32
	HANDLE hConsole;
	int Background = 0;
	int DefaultConsoleColor = 15 + 0 * 16;
#endif
};

#define ExecuteLog(LogType, LogDomain, LogVerbosity, Message, ...) Log_Internal(__FILE__, __LINE__, LogType, LogDomain, LogVerbosity, Message, __VA_ARGS__);

#endif