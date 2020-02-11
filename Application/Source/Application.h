
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Debug/LogSystem.h"

#include <string>
#include <memory>

#include "WindowSystem\WindowSystem.h"
#include "EngineEditor.h"
#include "RenderingSystem\RenderingSystem.h"
#include "RenderingSystem\RenderingSystemUtilities.h"
#include "ConfiguratorInterface.h"

struct SDL_Window;

class Application
{
public:

	Application() {}
	~Application() {}

	bool Initiate();
	void CreateApplicationWindow(RenderingInstanceType Type);
	void CreateRenderer(RenderingInstanceType Type);

private:

	std::unique_ptr<WindowSystem> mWindowSystem;
	std::unique_ptr<RenderingSystem> mRenderingSystem;
	std::unique_ptr<EngineEditor> mEngineEditor;

	void MainLoop();
	void LogicLoop();
	void RenderingLoop();

	void RendererToEngineEditorConfiguration(RenderingSystem* const Renderer, EngineEditor* const EngineEditor);

};

namespace Configurator
{
	class Renderer_EngineEditor final : public I::Configurator
	{
	public:
		Renderer_EngineEditor(WindowSystem* const aWindowSystem, RenderingSystem* const aRenderingSystem, EngineEditor* const aEngineEditor)
			: mWindowSystem{aWindowSystem}
			, mRenderingSystem{aRenderingSystem}
			, mEngineEditor{aEngineEditor}
		{}

		// Configure should be used immediately after the constructor invocation.
		void Configure() override;
		void Invalidate() override {}

	private:

		WindowSystem* const mWindowSystem;
		RenderingSystem* const mRenderingSystem;
		EngineEditor* const mEngineEditor;
	};
}


#endif