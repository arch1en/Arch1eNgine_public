#ifndef RENDERINGCONTEXT_IMPL_H
#define RENDERINGCONTEXT_IMPL_H

#include <string>
#include "Mathematics.inl"

// Namespaces must be separated, since you cannot use a I::RenderingContextProperties_ClearColor_Impl inside I::RenderingContext_Impl if they are in the same namespace block.

namespace I {
	class WindowImpl;
}

namespace I
{
	struct RenderingContextProperties_ClearColor_Impl
	{
		RenderingContextProperties_ClearColor_Impl()
			: ClearColorBuffer{ false }
			, ClearDepthBuffer{ false }
		{}

		bool ClearColorBuffer;
		bool ClearDepthBuffer;
	};
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
		virtual void SetSwapInterval(int Interval) = 0;
		virtual void SetClearColor(Vector4<float> ClearColor) = 0;
		virtual void ClearContext(I::RenderingContextProperties_ClearColor_Impl Properties) = 0;
	};
}

#endif 