#include "Window_SDL2.h"
#include "LogSystem.h"
#include "SDL.h"

Window_SDL2::Window_SDL2(WindowProperties Properties)
	: I::Window_Impl{Properties}
	, WindowHandle{nullptr}
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LogV(LogType::Error, LOGDOMAIN_WINDOW_SDL2, 0, "Cannot initialize SDL Window. SDL_Init failed.");
	}

	if (Properties.WindowPosition.Mode == WindowPositionMode::Centered)
	{
		Properties.WindowPosition.PositionX = SDL_WINDOWPOS_CENTERED;
		Properties.WindowPosition.PositionY = SDL_WINDOWPOS_CENTERED;
	}

}

void Window_SDL2::RetrieveWindowInfo()
{
	if (WindowHandle == nullptr)
	{
		LogV(LogType::Error, LOGDOMAIN_WINDOW_SDL2, 0, "Cannot retrieve window info. WindowHandle must be valid.");
		return;
	}

	SDL_VERSION(&WindowInfo.version);

	if (!SDL_GetWindowWMInfo(WindowHandle, &WindowInfo))
	{
		LogV(LogType::Error, LOGDOMAIN_WINDOW_SDL2, 0, "Error retrieving window info.");
	}
}

std::string Window_SDL2::GetImplementationType()
{
	return "";
}

void* Window_SDL2::GetWindowHandle() const
{
	return WindowHandle;
}

void Window_SDL2::DestroyWindow()
{
	SDL_DestroyWindow(WindowHandle);

	// @todo There should also be SDL_Quit() but it should rather be made when we will do a WindowManager of some kind.
}

const SDL_SysWMinfo* Window_SDL2::GetWindowInfo() const
{
	return &WindowInfo;
}