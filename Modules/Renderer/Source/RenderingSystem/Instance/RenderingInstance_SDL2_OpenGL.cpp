#include "RenderingInstance_SDL2_OpenGL.h"

#include <glad/glad.h>
#include "Assertions.h"

void RenderingInstance_SDL2_OpenGL::CreateOpenGLContext(SDL_Window* WindowHandle)
{
	mInstanceHandle = std::make_unique<SDL_GLContext>(SDL_GL_CreateContext(WindowHandle));
}


void RenderingInstance_SDL2_OpenGL::Initialize()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	UseDoubleBuffering(true);
}

void RenderingInstance_SDL2_OpenGL::InitializeGLAD()
{
	if (!gladLoadGL())
	{
		LogV(LogType::Error, "Renderer", 0, "GLAD initialization failed.");
	}
}

void RenderingInstance_SDL2_OpenGL::UseDoubleBuffering(bool State)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, int(State));
}

const std::string RenderingInstance_SDL2_OpenGL::GetImplementationType() const
{
	return "SDL2/OpenGL";
}

void* RenderingInstance_SDL2_OpenGL::GetRenderingInstanceHandle()
{
	return mInstanceHandle.get();
}

void RenderingInstance_SDL2_OpenGL::SetRenderingInstanceHandle(void* Handle)
{
	Assert(static_cast<SDL_GLContext>(Handle) != nullptr, "Wrong handle type.");
	mInstanceHandle = std::unique_ptr<SDL_GLContext>(static_cast<SDL_GLContext*>(Handle));
}

void RenderingInstance_SDL2_OpenGL::SetSwapInterval(int Interval)
{
	SDL_GL_SetSwapInterval(Interval);
}

void RenderingInstance_SDL2_OpenGL::RenderLoop()
{

}

void RenderingInstance_SDL2_OpenGL::SetClearColor(Vector4<float> ClearColor)
{
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
}

void RenderingInstance_SDL2_OpenGL::ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties)
{
	int Flags = 0x0;

	if (Properties.ClearColorBuffer)
	{
		Flags |= GL_COLOR_BUFFER_BIT;
	}

	if (Properties.ClearDepthBuffer)
	{
		Flags |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(Flags);
}