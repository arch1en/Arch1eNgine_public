#include "WindowSystem.h"

#include "WindowSystem/Implementations/Window_SDL2.h"
#include "WindowSystem/Implementations/Window_SDL2_OpenGL.h"
#include "WindowSystem/Implementations/Window_SDL2_Vulkan.h"

void WindowSystem::CreateNewWindow(WindowProperties aWindowProperties)
{
	I::Window_Impl* NewWindow = nullptr;
#if PLATFORM_WIN32 || PLATFORM_WIN64
	if (aWindowProperties.RendererType == WindowRendererType::Vulkan)
	{
		NewWindow = new Window_SDL2_Vulkan(aWindowProperties);
	}
	else if (aWindowProperties.RendererType == WindowRendererType::OpenGL)
	{
		NewWindow = new Window_SDL2_OpenGL(aWindowProperties);
	}
#endif
#if PLATFORM_LINUX
    NewWindow = new Window_Linux;
#endif

	NewWindow->RetrieveWindowInfo();

	Windows.push_back(std::unique_ptr<I::Window_Impl>(NewWindow));
}

I::Window_Impl* WindowSystem::GetMainWindow()
{
	return Windows[0].get();
}