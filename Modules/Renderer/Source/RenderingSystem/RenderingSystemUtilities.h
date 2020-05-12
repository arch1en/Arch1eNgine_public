#ifndef RENDERINGSYSTEMUTILITIES_H
#define RENDERINGSYSTEMUTILITIES_H

#include "Utilities.h"

// Needs to match WindowSystemUtilities -> WindowRendererType
enum class RenderingInstanceType : uint8_t
{
	OpenGL,
	Vulkan,
	Direct3D,
	DirectX,
	Metal,
	TOTAL,
	INVALID
};
TypeSafeEnumBitmask(RenderingInstanceType)

enum class TextureImageFormat : uint8_t
{
	R8G8B8A8_SRGB,
	R8G8B8A8_UNORM
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