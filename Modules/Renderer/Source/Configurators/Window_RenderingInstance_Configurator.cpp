#include "Window_RenderingInstance_Configurator.h"

#include <SDL.h>

#include "Logger.h"
#include "WindowSystem/Implementations/Window_SDL2.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_OpenGL.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
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

		InstanceVkSDL2->CreateVulkanInstance(mWindow->GetWindowHandle());

		// @todo
		return;
	}
	else if (mRenderingInstance->GetImplementationType().compare("SDL2/OpenGL") == 0)
	{
		RenderingInstance_SDL2_OpenGL* InstanceOGLSDL2 = static_cast<RenderingInstance_SDL2_OpenGL*>(mRenderingInstance);
		SDL_GLContext NewInstance = SDL_GL_CreateContext(static_cast<SDL_Window*>(mWindow->GetWindowHandle()));

		InstanceOGLSDL2->SetRenderingInstanceHandle(static_cast<void*>(NewInstance));
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