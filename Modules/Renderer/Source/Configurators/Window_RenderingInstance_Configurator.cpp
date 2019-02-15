#include "Window_RenderingInstance_Configurator.h"

#include <SDL.h>

#include "LogSystem.h"
#include "WindowSystem/Implementations/Window_SDL2.h"
#include "WindowSystem/Implementations/Window_SDL2_Vulkan.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_OpenGL.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceUtilities.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceHandler.h"
#include "RenderingSystem/Vulkan/Surface/Implementations/Surface_Base.h"
#include "vulkan/vulkan.h"


Configurator::Window_RenderingInstance::Window_RenderingInstance(I::Window_Impl* Window, I::RenderingInstance_Impl* RenderingInstance)
	: mWindow{Window}
	, mRenderingInstance{RenderingInstance}
{}

void Configurator::Window_RenderingInstance::ConfigureImplementations()
{
	if (mWindow->GetImplementationType().compare("SDL2/Vulkan") == 0)
	{
		RenderingInstance_SDL2_Vulkan* InstanceVkSDL2 = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingInstance);
		Window_SDL2_Vulkan* WindowVkSDL2 = static_cast<Window_SDL2_Vulkan*>(mWindow);

		InstanceVkSDL2->CreateVulkanInstance(WindowVkSDL2->GetWindowHandle());
		InstanceVkSDL2->CreateSurfaceHandler();
		InstanceVkSDL2->CreateDeviceHandler();

		SurfaceHandlerCreationData_SDL SurfaceData;
		
		SurfaceData.VulkanInstanceRef = InstanceVkSDL2->GetRenderingInstanceHandle();
		SurfaceData.TargetSurfaceEnum = TargetSurface::Win32;
		SurfaceData.WindowHandle = static_cast<SDL_Window*>(WindowVkSDL2->GetWindowHandle());
		SurfaceData.WindowInfo = WindowVkSDL2->GetWindowInfo();

		InstanceVkSDL2->GetSurfaceHandler()->CreateSurface(&SurfaceData);

		InstanceVkSDL2->GetDeviceHandler()->Initiate(*InstanceVkSDL2->GetRenderingInstanceHandle(), *InstanceVkSDL2->GetSurfaceHandler()->GetMainSurface()->GetHandle());

		// @todo
		return;
	}
	else if (mRenderingInstance->GetImplementationType().compare("SDL2/OpenGL") == 0)
	{
		RenderingInstance_SDL2_OpenGL* InstanceOGLSDL2 = static_cast<RenderingInstance_SDL2_OpenGL*>(mRenderingInstance);

		InstanceOGLSDL2->CreateOpenGLContext(static_cast<SDL_Window*>(mWindow->GetWindowHandle()));
		InstanceOGLSDL2->InitializeGLAD();
		return;
	}

	LogV(LogType::Warning, "Renderer", 0, "%s function is not implemented for Window{%s} and Instance{%s} pair.", __FUNCTION__, mWindow->GetImplementationType().c_str(), mRenderingInstance->GetImplementationType().c_str());
}

void Configurator::Window_RenderingInstance::Invalidate()
{
	mWindow = nullptr;
	mRenderingInstance = nullptr;
}