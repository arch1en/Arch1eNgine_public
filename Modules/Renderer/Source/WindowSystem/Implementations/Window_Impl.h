#ifndef WINDOWIMPL_H
#define WINDOWIMPL_H

#include <string>

#include "RenderingSystem/Instance/RenderingInstance_Impl.h"
#include "WindowSystem/WindowSystemUtilities.h"

namespace I
{
	class Window_Impl
	{
	public:
		virtual ~Window_Impl() {}
		virtual bool InitiateWindow(WindowProperties Properties) = 0;
		virtual std::string GetImplementationType() = 0;
		virtual void* GetWindowHandle() const = 0;
		virtual void DestroyWindow() = 0;
		virtual void SwapBuffers() = 0;
	};
}

#endif