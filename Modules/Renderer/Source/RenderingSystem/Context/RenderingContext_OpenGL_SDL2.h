#ifndef RENDERINGCONTEXT_OPENGL_SDL2_H
#define RENDERINGCONTEXT_OPENGL_SDL2_H

#include <memory>
#include <SDL.h>

#include "RenderingSystem/Context/RenderingContext_Impl.h"

class I::WindowImpl;

class RenderingContext_OpenGL_SDL2 : public I::RenderingContext_Impl
{
public:
	virtual void Initialize() override;
	virtual const std::string GetImplementationType() const override;
	void UseDoubleBuffering(bool State);
	virtual void* GetRenderingContextHandle() const override;
	virtual void SetRenderingContextHandle(void* Handle) override;

private:

	std::unique_ptr<SDL_GLContext> mContextHandle;

};

#endif