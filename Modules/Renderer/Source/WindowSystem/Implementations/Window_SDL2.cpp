#include "Window_SDL2.h"
#include "LogSystem.h"
#include "SDL.h"

bool Window_SDL2::InitiateWindow(WindowProperties Properties)
{
	bool Result = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LogV(LogType::Error, "WindowSystem", 0, "Cannot initialize SDL Window. SDL_Init failed.");
		Result = false;
	}

	if (Properties.WindowPosition.Mode == WindowPositionMode::Centered)
	{
		Properties.WindowPosition.PositionX = SDL_WINDOWPOS_CENTERED;
		Properties.WindowPosition.PositionY = SDL_WINDOWPOS_CENTERED;
	}

	return Result;
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
