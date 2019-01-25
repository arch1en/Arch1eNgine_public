#ifndef RENDERINGSYSTEMUTILITIES_H
#define RENDERINGSYSTEMUTILITIES_H


// Needs to match WindowSystemUtilities -> WindowRendererType
enum class RenderingInstanceType
{
	OpenGL,
	Vulkan,
	Direct3D,
	DirectX,
	Metal,
	TOTAL,
	INVALID
};

struct RenderingInstanceProperties
{
	RenderingInstanceProperties()
		: Type{ RenderingInstanceType::INVALID}
		, BufferSwapInterval{ 1 }
	{}

	RenderingInstanceType Type;
	int BufferSwapInterval;
};

#endif