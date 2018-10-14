#ifndef RENDERER_UTILITIES_H
#define RENDERER_UTILITIES_H

enum class RendererType
{
	OpenGL,
	Vulkan,
	DirectX,
	TOTAL,
	INVALID
};

enum class ShaderType
{
	Vertex,
	Pixel,
	Geometry,
	Tesselation,
	TOTAL,
	INVALID
};

#endif