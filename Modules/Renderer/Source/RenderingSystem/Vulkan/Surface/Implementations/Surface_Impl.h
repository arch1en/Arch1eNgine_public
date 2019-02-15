#ifndef SURFACE_IMPL_H
#define SURFACE_IMPL_H

#include "vulkan/vulkan.h"

#include "RenderingSystem/Vulkan/Surface/SurfaceUtilities.h"

namespace I
{
	class Surface_Impl
	{
	public:
		Surface_Impl() = delete;
		Surface_Impl(const SurfaceHandlerCreationData* Data) {}
		virtual ~Surface_Impl() {}
	};
}

#endif