#ifndef SURFACEUTILITIES_H
#define SURFACEUTILITIES_H

#include "windows.h"
#include "SDL.h"
#include "SDL_syswm.h"
#include "Utilities.h"

enum TargetSurface
{
	Win32
};

struct SurfaceHandlerCreationData : public TypeID
{
	const VkInstance*	VulkanInstanceRef;
	TargetSurface		TargetSurfaceEnum;
	virtual inline const char* GetTypeName() const override { return "SurfaceHandlerCreationData"; }
};

struct SurfaceHandlerCreationData_SDL : public SurfaceHandlerCreationData
{
	SDL_Window* WindowHandle;
	const SDL_SysWMinfo* WindowInfo;
	virtual inline const char* GetTypeName() const override { return "SurfaceHandlerCreationData_SDL"; }
};

#endif