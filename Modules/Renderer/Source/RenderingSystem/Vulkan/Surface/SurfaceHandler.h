#ifndef SURFACEHANDLER_H
#define SURFACEHANDLER_H

#include <vector>
#include <memory>

#include "vulkan/vulkan.h"
#include "Implementations/Surface_Base.h"

struct SurfaceHandlerCreationData;

class SurfaceHandler
{
public:
	void CreateSurface(const SurfaceHandlerCreationData* Data);
	void DestroySurface(const VkInstance* Instance, Surface_Base* Surface);
	void Destroy(const VkInstance* Instance);
	const Surface_Base* GetMainSurface() const;

private:

	std::vector<std::unique_ptr<Surface_Base>> mSurfaces;
};

#endif