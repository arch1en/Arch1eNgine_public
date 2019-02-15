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
	void DestroySurface(Surface_Base* Surface);

	const Surface_Base* GetMainSurface() const;

private:

	std::shared_ptr<VkInstance> mInstanceHandleRef;

	std::vector<std::unique_ptr<Surface_Base>> mSurfaces;
};

#endif