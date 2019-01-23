
#include "RenderingSystem.h"

#include "RenderingSystem/Context/RenderingContext_OpenGL_SDL2.h"

void RenderingSystem::CreateContext(RenderingContextProperties Properties)
{
	I::RenderingContext_Impl* NewRenderingContext = nullptr;

	if (Properties.Type == RenderingContextType::OpenGL)
	{
#if PLATFORM_WIN32 || PLATFORM_WIN64
		NewRenderingContext = new RenderingContext_OpenGL_SDL2;
#elif PLATFORM_LINUX

#endif
	}

	mRenderingContext = std::unique_ptr<I::RenderingContext_Impl>(NewRenderingContext);
	mRenderingContext->SetSwapInterval(Properties.BufferSwapInterval);

}

I::RenderingContext_Impl* RenderingSystem::GetRenderingContext() const
{
	return mRenderingContext.get();
}
