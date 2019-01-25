#include "Window_SDL2_OpenGL.h"

#include "SDL.h"

#include "Logger.h"

bool Window_SDL2_OpenGL::InitiateWindow(WindowProperties Properties)
{
	bool Result = Window_SDL2::InitiateWindow(Properties);

	Properties.Flags |= SDL_WINDOW_OPENGL;

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

std::string Window_SDL2_OpenGL::GetImplementationType()
{
	return "SDL2/OpenGL";
}

void Window_SDL2_OpenGL::SwapBuffers()
{

}
