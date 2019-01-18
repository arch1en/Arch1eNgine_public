#ifndef WINDOWSYSTEMUTILITIES_H
#define WINDOWSYSTEMUTILITIES_H

#include <string>

enum class WindowPositionMode
{
	Centered,
	Custom		// Use PositionX and PositionY to set the position.
};

struct WindowProperties
{
	explicit WindowProperties()
		: Title{ "" }
		, Width{ 0 }
		, Height{ 0 }
		, Flags{ 0x0 }
	{}

	std::string Title;
	int Width;
	int Height;
	struct {

		WindowPositionMode Mode;
		int PositionX;
		int PositionY;

	} WindowPosition;

	int Flags;
};


#endif