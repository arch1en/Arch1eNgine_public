#ifndef RENDERINGCONTEXT_OPENGL_SDL2_H
#define RENDERINGCONTEXT_OPENGL_SDL2_H

#include <memory>
#include <SDL.h>

#include "RenderingSystem/Instance/RenderingInstance_Impl.h"

class I::WindowImpl;

class RenderingInstance_SDL2_OpenGL : public I::RenderingInstance_Impl
{
public:
	void CreateOpenGLContext(SDL_Window* WindowHandle);
	virtual void Initialize() override;
	void InitializeGLAD();
	virtual const std::string GetImplementationType() const override;
	void UseDoubleBuffering(bool State);
	virtual void* GetRenderingInstanceHandle() override;
	virtual void SetRenderingInstanceHandle(void* Handle) override;
	virtual void SetSwapInterval(int Interval) override;
	virtual void RenderLoop() override;
	virtual void SetClearColor(Vector4<float> ClearColor) override;
	virtual void ResizeCanvas(int Width, int Height) override {}
	virtual void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override;

private:

	std::unique_ptr<SDL_GLContext> mInstanceHandle;

};

#endif