#include "Window_SDL2_Vulkan.h"

#include "SDL.h"

#include "Logger.h"

bool Window_SDL2_Vulkan::InitiateWindow(WindowProperties Properties)
{
	bool Result = Window_SDL2::InitiateWindow(Properties);

	Properties.Flags |= SDL_WINDOW_VULKAN;

	WindowHandle = SDL_CreateWindow(
		Properties.Title.c_str(),
		Properties.WindowPosition.PositionX,
		Properties.WindowPosition.PositionY,
		Properties.Width,
		Properties.Height,
		Properties.Flags
	);

	return Result;
}

std::string Window_SDL2_Vulkan::GetImplementationType()
{
	return "SDL2/Vulkan";
}

void Window_SDL2_Vulkan::SwapBuffers()
{

}
