#include "Window_SDL2.h"
#include "Logger.h"
#include "SDL.h"

bool Window_SDL2::InitiateWindow(WindowProperties Properties)
{
	bool Result = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LogV(LogType::Error, "WindowSystem", 0, "Cannot initialize SDL Window. SDL_Init failed.");
		Result = false;
	}

	int WindowPositionX = 0;
	int WindowPositionY = 0;

	if (Properties.WindowPosition.Mode == WindowPositionMode::Centered)
	{
		WindowPositionX = SDL_WINDOWPOS_CENTERED;
		WindowPositionY = SDL_WINDOWPOS_CENTERED;
	}

	WindowHandle = SDL_CreateWindow(
		Properties.Title.c_str(),
		WindowPositionX,
		WindowPositionY,
		Properties.Width,
		Properties.Height,
		SDL_WINDOW_OPENGL
	);

	return Result;
}

std::string Window_SDL2::GetImplementationType()
{
	return "SDL2";
}

void* Window_SDL2::GetWindowHandle() const
{
	return WindowHandle;
}