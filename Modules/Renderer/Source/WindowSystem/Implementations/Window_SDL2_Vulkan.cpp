#include "Window_SDL2_Vulkan.h"

#include "SDL.h"
#include "SDL_vulkan.h"

#include "LogSystem.h"

Window_SDL2_Vulkan::Window_SDL2_Vulkan(WindowProperties Properties)
	: Window_SDL2{Properties}
{
	Properties.Flags |= SDL_WINDOW_VULKAN;

	if (Properties.Resizeable)
	{
		Properties.Flags |= SDL_WINDOW_RESIZABLE;
	}

	WindowHandle = SDL_CreateWindow(
		Properties.Title.c_str(),
		Properties.WindowPosition.PositionX,
		Properties.WindowPosition.PositionY,
		Properties.Width,
		Properties.Height,
		Properties.Flags
	);
}

std::string Window_SDL2_Vulkan::GetImplementationType()
{
	return "SDL2/Vulkan";
}

void Window_SDL2_Vulkan::SwapBuffers()
{

}

void Window_SDL2_Vulkan::CreateWindowSurface(const VkInstance* InstanceHandle, VkSurfaceKHR* SurfaceHandle)
{
	Assert(InstanceHandle != nullptr, "InstanceHandle must be valid.");
	Assert(SurfaceHandle != nullptr, "SurfaceHandle must be valid.");

	if (SDL_Vulkan_CreateSurface(WindowHandle, *InstanceHandle, SurfaceHandle) != SDL_bool::SDL_TRUE)
	{
		LogVk(LogType::Error, 0, "Surface creation for Vulkan failed.");
	}
}
