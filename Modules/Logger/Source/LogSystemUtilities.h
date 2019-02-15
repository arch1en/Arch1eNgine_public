#ifndef LOGSYSTEMUTILITIES_H
#define LOGSYSTEMUTILITIES_H

#define LOGDOMAIN_RENDERER_VULKAN "Renderer/Vulkan"
#define LOGDOMAIN_WINDOW_SDL2 "Window/SDL2"

enum LogImplementation
{
	General,
	Vulkan,
	TOTAL
};

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

#endif