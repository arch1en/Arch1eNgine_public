#ifndef SURFACE_BASE_H
#define SURFACE_BASE_H

#include "vulkan/vulkan.h"

#include "Surface_Impl.h"

struct SurfaceHandlerCreationData;

class Surface_Base : I::Surface_Impl
{
public:
	Surface_Base(const SurfaceHandlerCreationData* Data) 
		: I::Surface_Impl{Data}
	{}

	inline const VkSurfaceKHR* GetHandle() const
	{
		return &SurfaceHandle;
	}

	virtual void Destroy(const VkInstance& InstanceHandle) = 0;

protected:

	VkSurfaceKHR SurfaceHandle;

private:

};

#endif