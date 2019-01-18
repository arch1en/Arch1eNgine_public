#include "RenderingContext_OpenGL_SDL2.h"

#include "Assertions.h"

void RenderingContext_OpenGL_SDL2::Initialize()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	UseDoubleBuffering(true);
}

void RenderingContext_OpenGL_SDL2::UseDoubleBuffering(bool State)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, int(State));
}

const std::string RenderingContext_OpenGL_SDL2::GetImplementationType() const
{
	return "OpenGL/SDL2";
}

void* RenderingContext_OpenGL_SDL2::GetRenderingContextHandle() const
{
	return mContextHandle.get();
}

void RenderingContext_OpenGL_SDL2::SetRenderingContextHandle(void* Handle)
{
	Assert(static_cast<SDL_GLContext>(Handle) != nullptr, "Wrong handle type.");
	mContextHandle = std::unique_ptr<SDL_GLContext>(static_cast<SDL_GLContext*>(Handle));
}
