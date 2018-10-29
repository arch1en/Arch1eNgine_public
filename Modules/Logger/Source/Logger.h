////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Debugging tool, for using Log messages, and future similar mechanisms.
//  @version    : 1.0.0
//
////////////////////////////////////////
#pragma once

#define Log(LogType, Message, ...) Logger::GetInstance().Log_( __FILE__, __LINE__, LogType, Message, __VA_ARGS__);

enum class LogType : unsigned char
{
	Notice,
	Warning,
	Error,
	Fatal,
	Assertion,
	Total		// U can't touch this ! (HAMMER TIME)
};

class MODULE_API Logger
{
public:
	Logger();

	// "= delete" - C++11. Prevents compiler from generating these functions.
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	
	static Logger& GetInstance();

	void Logger::Log_(const char* InFilePath, int InLineNumber, LogType InDebugType, const char* InMessage, ...);

private:

#ifdef PLATFORM_WINDOWS
	HANDLE hConsole;
	int Background = 0;
	int DefaultConsoleColor = 15 + 0 * 16;
#endif

	void CreateLogFile();
	void SaveLogToFile();
	
	/**
	*   Changes text color on the console. Platform specific.
	*
	*   @param  InForeground    : Sets foreground color of a text (default 15 = White).
	*   @param  InBackground    : Sets background color of a text (default 0 = Black).
	*
	*/
	void SetMessageColor(int InForeground = 15, int InBackground = 0);
};