#ifndef WINDOW_SDL2_H
#define WINDOW_SDL2_H

#include <string>

#include "SDL_syswm.h"
#include "Window_Impl.h"
#include "RenderingSystem/Instance/RenderingInstance_Impl.h"

class RenderingInstance;
struct SDL_Window;

class Window_SDL2 : public I::Window_Impl
{
public:
	Window_SDL2(WindowProperties Properties);
	virtual void RetrieveWindowInfo() override;
	virtual std::string GetImplementationType() override;
	virtual void* GetWindowHandle() const override;
	virtual void DestroyWindow() override;
	const SDL_SysWMinfo* GetWindowInfo() const;
	virtual void SwapBuffers() override {};

protected:
	SDL_SysWMinfo WindowInfo;
	SDL_Window * WindowHandle;
	SDL_Surface* Surface;
	RenderingInstance* mRenderingInstance;

};

#endif
