#ifndef WINDOW_RENDERINGCONTEXT_CONFIGURATOR_H
#define WINDOW_RENDERINGCONTEXT_CONFIGURATOR_H

#include "ConfiguratorInterface.h"
#include "WindowSystem/Implementations/Window_Impl.h"

namespace Configurator {

	class Window_RenderingContext : public I::Configurator
	{
	public:
		Window_RenderingContext() = delete;
		Window_RenderingContext(I::Window_Impl* Window, I::RenderingContext_Impl* RenderingContext);

		void ConfigureImplementations();

	private:

		I::Window_Impl* mWindow;
		I::RenderingContext_Impl* mRenderingContext;

		virtual void Invalidate() override;

	};

}

#endif