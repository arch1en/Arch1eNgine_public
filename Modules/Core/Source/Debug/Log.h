#ifndef CORE_DEBUG_LOG_H
#define CORE_DEBUG_LOG_H

#define LOGDOMAIN_RENDERER_VULKAN "Renderer/Vulkan"
#define LOGDOMAIN_WINDOW_SDL2 "Window/SDL2"

using LogImplementation = const char*;

enum LogType
{
	Info,
	Notice,
	Warning,
	Error,
	Fatal,
	Assertion,
	Total		// U can't touch this ! (HAMMER TIME)
};

enum ANSIEscapeColorCode
{
	Black,
	Red,
	Green,
	Yellow,
	Blue,
	Magneta,
	Cyan,
	White,
};

static const char* GetANSIEscapeColorCode(ANSIEscapeColorCode Color)
{
	if (ANSIEscapeColorCode::Black) return "\033[1;30m";
	if (ANSIEscapeColorCode::Red) return "\033[1;31m";
	if (ANSIEscapeColorCode::Green) return "\033[1;32m";
	if (ANSIEscapeColorCode::Yellow) return "\033[1;33m";
	if (ANSIEscapeColorCode::Blue) return "\033[1;34m";
	if (ANSIEscapeColorCode::Magneta) return "\033[1;35m";
	if (ANSIEscapeColorCode::Cyan) return "\033[1;36m";
	if (ANSIEscapeColorCode::White) return "\033[1;37m";

	return "\033[1;37m";
}

#define LogV(LogType, LogDomain, VerbosityLevel, Message, ...) LogSystem::GetInstance()->GetGeneralLogger()->ExecuteLog(LogType, LogDomain, VerbosityLevel, Message, __VA_ARGS__);
#define Log(LogType, Message, ...) LogV(LogType, "Temp", 0, Message, __VA_ARGS__)

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

namespace Core::Debug::Log
{
	void Log_Internal(const char* InFilePath, int InLineNumber, LogType InDebugType, const char* InLogDomain, int InLogVerbosity, const char* InMessage, ...);
}


#endif