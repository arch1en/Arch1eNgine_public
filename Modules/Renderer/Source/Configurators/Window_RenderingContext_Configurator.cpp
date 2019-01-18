#include "Window_RenderingContext_Configurator.h"

#include <SDL.h>

#include "Logger.h"
#include "WindowSystem/Implementations/Window_SDL2.h"
#include "RenderingSystem/Context/RenderingContext_OpenGL_SDL2.h"

Configurator::Window_RenderingContext::Window_RenderingContext(I::Window_Impl* Window, I::RenderingContext_Impl* RenderingContext)
	: mWindow{Window}
	, mRenderingContext{RenderingContext}
{}

void Configurator::Window_RenderingContext::AttachRenderingContextHandleToWindow()
{
	if (mWindow->GetImplementationType().compare("SDL2") == 0)
	{
		if (mRenderingContext->GetImplementationType().compare("OpenGL/SDL2") == 0)
		{
			SDL_GLContext NewContext = SDL_GL_CreateContext(static_cast<SDL_Window*>(mWindow->GetWindowHandle()));
			
			mRenderingContext->SetRenderingContextHandle(static_cast<void*>(NewContext));
			return;
		}
	}

	LogV(LogType::Warning, "Renderer", 0, "%s function is not implemented for Window{%s} and Context{%s} pair.", __FUNCTION__, mWindow->GetImplementationType().c_str(), mRenderingContext->GetImplementationType().c_str());
}

void Configurator::Window_RenderingContext::Invalidate()
{
	mWindow = nullptr;
	mRenderingContext = nullptr;
}