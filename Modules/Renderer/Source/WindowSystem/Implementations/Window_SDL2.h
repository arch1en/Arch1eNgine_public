#ifndef WINDOW_SDL2_H
#define WINDOW_SDL2_H

#include <string>

#include "Window_Impl.h"
#include "RenderingSystem/Context/RenderingContext_Impl.h"

class RenderingContext;
struct SDL_Window;

class Window_SDL2 : public I::Window_Impl
{
public:
	virtual bool InitiateWindow(WindowProperties Properties) override;
	virtual std::string GetImplementationType() override;
	virtual void* GetWindowHandle() const override;

	void SwapCanvas();

protected:
	SDL_Window * WindowHandle;
	RenderingContext* mRenderingContext;

};

#endif
