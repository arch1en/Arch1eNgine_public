#ifndef SURFACE_SDL_H
#define SURFACE_SDL_H

#include "windows.h"
#include "vulkan/vulkan.h"

#include "RenderingSystem/Vulkan/Surface/SurfaceUtilities.h"
#include "Surface_Base.h"

class Surface_SDL : public Surface_Base
{
public:
	Surface_SDL(const SurfaceHandlerCreationData* Data);

	void Destroy(const VkInstance& InstanceHandle) override;

};

#endif