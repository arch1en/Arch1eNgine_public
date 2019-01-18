#ifndef RENDERINGCONTEXT_IMPL_H
#define RENDERINGCONTEXT_IMPL_H

#include <string>

namespace I {
	class WindowImpl;
}

namespace I
{
	class RenderingContext_Impl
	{
	public:
		virtual void Initialize() = 0;
		virtual const std::string GetImplementationType() const = 0;
		virtual void* GetRenderingContextHandle() const = 0;
		virtual void SetRenderingContextHandle(void* Handle) = 0;
	};
}


#endif 