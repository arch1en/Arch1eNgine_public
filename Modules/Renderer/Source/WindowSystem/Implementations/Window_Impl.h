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
		Window_Impl() = delete;
		Window_Impl(WindowProperties Properties) {}
		virtual ~Window_Impl() {}
		virtual void RetrieveWindowInfo() {}
		virtual std::string GetImplementationType() = 0;
		virtual void* GetWindowHandle() const = 0;
		virtual void DestroyWindow() = 0;
		virtual void SwapBuffers() = 0;
	};
}

#endif