#ifndef WINDOW_RENDERINGCONTEXT_CONFIGURATOR_H
#define WINDOW_RENDERINGCONTEXT_CONFIGURATOR_H

#include "ConfiguratorInterface.h"
#include "WindowSystem/Implementations/Window_Impl.h"

namespace Configurator {

	class Window_RenderingInstance : public I::Configurator
	{
	public:
		Window_RenderingInstance() = delete;
		Window_RenderingInstance(I::Window_Impl* Window, I::RenderingInstance_Impl* RenderingInstance);

		void ConfigureImplementations();

	private:

		I::Window_Impl* mWindow;
		I::RenderingInstance_Impl* mRenderingInstance;

		virtual void Invalidate() override;

	};

}

#endif