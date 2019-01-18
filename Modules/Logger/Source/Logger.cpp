////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Debugging tool, for using Log messages, and future similar mechanisms.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"

#include "Logger.h"

#include <string>
#include <cstdarg>

Logger::Logger()
{
#ifdef PLATFORM_WINDOWS
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

Logger& Logger::GetInstance()
{
	static Logger Singleton;
	return Singleton;
}

void Logger::Log_(const char* InFilePath, int InLineNumber, LogType InDebugType, const char* InLogDomain, int InLogVerbosity, const char* InMessage, ...)
{
	va_list Arguments;
	va_start(Arguments, InMessage);

	const char* LogTypeString;

	if (InDebugType == LogType::Notice)
	{
		SetMessageColor();
		LogTypeString = "Notice";
	}
	else if (InDebugType == LogType::Warning)
	{
		SetMessageColor(14);
		LogTypeString = "Warning";
	}
	else if (InDebugType == LogType::Error)
	{
		SetMessageColor(12);
		LogTypeString = "Error";
	}
	else if (InDebugType == LogType::Fatal)
	{
		SetMessageColor(12);
		LogTypeString = "Fatal";
	}
	else
	{
		LogTypeString = "Undefined";
	}

	std::string String;

	while (*InMessage != '\0')
	{
		if (*InMessage == '/')
		{
			String += *(InMessage + 1);
		}
		else if (*InMessage == '%')
		{

			InMessage++;

			if (*InMessage == 's')
			{
				String.append(va_arg(Arguments, const char*));

			}
			else if (*InMessage == 'i' || *InMessage == 'd')
			{
				std::string Buffer = std::to_string(va_arg(Arguments, int));
				String.append(Buffer);
			}
			else if (*InMessage == 'f')
			{
				std::string Buffer = std::to_string(va_arg(Arguments, double));
				String.append(Buffer);
			}
		}
		else
		{
			String += *InMessage;
		}

		InMessage++;

	}

	if (InDebugType < LogType::Error)
	{
		printf("%s : %s\n", LogTypeString, String.c_str());
	}
	else
	{
		printf("%s : %s | In %s Line (%i)\n", LogTypeString, String.c_str(), InFilePath, InLineNumber);
	}
}

void Logger::CreateLogFile()
{
	// TODO : Implement log file creating according to DebuggingProperties.ini file.
}

void Logger::SaveLogToFile()
{
	// TODO : Implement log file creating according to DebuggingProperties.ini file.
}

void Logger::SetMessageColor(int InForeground, int InBackground)
{
#ifdef PLATFORM_WINDOWS
	int Color = InForeground + InBackground * 16;

	SetConsoleTextAttribute(hConsole, Color);
#endif
}