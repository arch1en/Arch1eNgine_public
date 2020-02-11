#include "Window_SDL2_OpenGL.h"

#include "SDL.h"

#include "Debug/LogSystem.h"

Window_SDL2_OpenGL::Window_SDL2_OpenGL(WindowProperties Properties)
	: Window_SDL2{Properties}
{
	Properties.Flags |= SDL_WINDOW_OPENGL;

	WindowHandle = SDL_CreateWindow(
		Properties.Title.c_str(),
		Properties.WindowPosition.PositionX,
		Properties.WindowPosition.PositionY,
		Properties.Width,
		Properties.Height,
		Properties.Flags
	);
}

std::string Window_SDL2_OpenGL::GetImplementationType()
{
	return "SDL2/OpenGL";
}

void Window_SDL2_OpenGL::SwapBuffers()
{

}
