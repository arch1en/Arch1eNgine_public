
#include "RenderingSystem.h"

#include "RenderingSystem/Instance/RenderingInstance_SDL2_OpenGL.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"

void RenderingSystem::CreateInstance(RenderingInstanceProperties Properties)
{
	I::RenderingInstance_Impl* NewRenderingInstance = nullptr;

	if (Properties.Type == RenderingInstanceType::OpenGL)
	{
#if PLATFORM_WIN32 || PLATFORM_WIN64
		NewRenderingInstance = new RenderingInstance_SDL2_OpenGL;
#elif PLATFORM_LINUX

#endif
	}
	else if (Properties.Type == RenderingInstanceType::Vulkan)
	{
		NewRenderingInstance = new RenderingInstance_SDL2_Vulkan;
	}

	mRenderingInstance = std::unique_ptr<I::RenderingInstance_Impl>(NewRenderingInstance);
	mRenderingInstance->SetSwapInterval(Properties.BufferSwapInterval);
}

I::RenderingInstance_Impl* RenderingSystem::GetRenderingInstance() const
{
	return mRenderingInstance.get();
}
