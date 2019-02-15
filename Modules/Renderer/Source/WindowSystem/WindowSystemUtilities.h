#ifndef WINDOWSYSTEMUTILITIES_H
#define WINDOWSYSTEMUTILITIES_H

#include <string>

enum class WindowPositionMode
{
	Centered,
	Custom		// Use PositionX and PositionY to set the position.
};

// Needs to match RenderingSystemUtilities -> RenderingInstanceType
enum class WindowRendererType
{
	OpenGL,
	Vulkan,
	Direct3D,
	DirectX,
	Metal,
	TOTAL,
	INVALID
};

struct WindowProperties
{
	explicit WindowProperties()
		: Title{ "" }
		, Width{ 0 }
		, Height{ 0 }
		, Flags{ 0x0 }
		, RendererType{WindowRendererType::INVALID}
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

	WindowRendererType RendererType;
};


#endif