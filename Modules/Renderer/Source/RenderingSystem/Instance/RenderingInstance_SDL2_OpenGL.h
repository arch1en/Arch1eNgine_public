#ifndef RENDERINGCONTEXT_OPENGL_SDL2_H
#define RENDERINGCONTEXT_OPENGL_SDL2_H

#include <memory>
#include <SDL.h>

#include "RenderingSystem/Instance/RenderingInstance_Impl.h"

class I::WindowImpl;

class RenderingInstance_SDL2_OpenGL final : public I::RenderingInstance_Impl
{
public:
	void CreateOpenGLContext(SDL_Window* WindowHandle);

	void InitializeGLAD();
	void UseDoubleBuffering(bool State);

	// Begin I::RenderingInstance_Impl Interface
	void Initialize() override;
	const std::string GetImplementationType() const override;
	void* GetRenderingInstanceHandle() override;
	void SetRenderingInstanceHandle(void* Handle) override;
	void SetSwapInterval(int Interval) override;
	void RenderLoop() override {}
	void SetClearColor(Vector4<float> ClearColor) override;
	void ResizeCanvas(int Width, int Height) override {}
	void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override;
	void LoadTextureImage(const char* Path, TextureImageFormat Format, const std::string& TextureID) override {}
	void LoadTextureImage(unsigned char* Pixels, int TexWidth, int TexHeight, TextureImageFormat Format, const std::string& TextureID) override {}
	// End I::RenderingInstance_Impl Interface

private:

	std::unique_ptr<SDL_GLContext> mInstanceHandle;

};

#endif