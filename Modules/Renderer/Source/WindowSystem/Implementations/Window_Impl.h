#ifndef WINDOWIMPL_H
#define WINDOWIMPL_H

#include <string>

#include "RenderingSystem/Context/RenderingContext_Impl.h"
#include "WindowSystem\WindowSystemUtilities.h"

namespace I
{
	class Window_Impl
	{
	public:

		virtual bool InitiateWindow(WindowProperties Properties) = 0;
		virtual std::string GetImplementationType() = 0;
		virtual void* GetWindowHandle() const = 0;
	};
}

#endif