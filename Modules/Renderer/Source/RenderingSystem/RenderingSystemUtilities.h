#ifndef RENDERINGSYSTEMUTILITIES_H
#define RENDERINGSYSTEMUTILITIES_H

enum class RenderingContextType
{
	OpenGL,
	Vulkan,
	Direct3D,
	DirectX,
	Metal,
};

struct RenderingContextProperties
{
	RenderingContextType Type;
};

#endif