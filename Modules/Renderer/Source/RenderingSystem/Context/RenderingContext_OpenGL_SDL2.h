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
	void InitializeGLAD();
	virtual const std::string GetImplementationType() const override;
	void UseDoubleBuffering(bool State);
	virtual void* GetRenderingContextHandle() const override;
	virtual void SetRenderingContextHandle(void* Handle) override;
	virtual void SetSwapInterval(int Interval) override;
	virtual void SetClearColor(Vector4<float> ClearColor) override;
	virtual void ClearContext(I::RenderingContextProperties_ClearColor_Impl Properties) override;

private:

	std::unique_ptr<SDL_GLContext> mContextHandle;

};

#endif