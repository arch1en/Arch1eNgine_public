#ifndef WINDOW_SDL2_OPENGL_H
#define WINDOW_SDL2_OPENGL_H

#include "Window_SDL2.h"

class Window_SDL2_OpenGL : public Window_SDL2
{
public:
	Window_SDL2_OpenGL(WindowProperties Properties);
	std::string GetImplementationType() override;
	void SwapBuffers() override;
};

#endif