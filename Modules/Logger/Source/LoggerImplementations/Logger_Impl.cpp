#include "Logger_Impl.h"

#include <string>

Logger_Impl::Logger_Impl()
{
#ifdef PLATFORM_WIN32
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

Logger_Impl::~Logger_Impl()
{

}

void Logger_Impl::Log_Internal(const char* InFilePath, int InLineNumber, LogType InDebugType, const char* InLogDomain, int InLogVerbosity, const char* InMessage, ...)
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
		printf("%s (%s) : %s\n", LogTypeString, InLogDomain, String.c_str());
	}
	else
	{
		printf("%s (%s) : %s | In %s Line (%i)\n", LogTypeString, InLogDomain, String.c_str(), InFilePath, InLineNumber);
	}
}

void Logger_Impl::SetMessageColor(int InForeground, int InBackground)
{
#ifdef PLATFORM_WIN32
	int Color = InForeground + InBackground * 16;

	SetConsoleTextAttribute(hConsole, Color);
#endif
}