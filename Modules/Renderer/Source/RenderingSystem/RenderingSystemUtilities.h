#ifndef RENDERINGSYSTEMUTILITIES_H
#define RENDERINGSYSTEMUTILITIES_H

enum class RenderingContextType
{
	OpenGL,
	Vulkan,
	Direct3D,
	DirectX,
	Metal,
	TOTAL,
	INVALID
};

struct RenderingContextProperties
{
	RenderingContextProperties()
		: Type{ RenderingContextType::INVALID}
		, BufferSwapInterval{ 1 }
	{}

	RenderingContextType Type;
	int BufferSwapInterval;
};

#endif