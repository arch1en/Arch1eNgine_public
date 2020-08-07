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
	if (Color == ANSIEscapeColorCode::Black) { return "\033[1;30m"; }
	if (Color == ANSIEscapeColorCode::Red) { return "\033[1;31m"; }
	if (Color == ANSIEscapeColorCode::Green) { return "\033[1;32m"; }
	if (Color == ANSIEscapeColorCode::Yellow) { return "\033[1;33m"; }
	if (Color == ANSIEscapeColorCode::Blue) { return "\033[1;34m"; }
	if (Color == ANSIEscapeColorCode::Magneta) { return "\033[1;35m"; }
	if (Color == ANSIEscapeColorCode::Cyan) { return "\033[1;36m"; }
	if (Color == ANSIEscapeColorCode::White) { return "\033[1;37m"; }

	return "\033[1;37m";
}

#endif