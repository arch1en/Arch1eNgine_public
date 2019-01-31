
#include "Application.h"

#include "Configurators/Window_RenderingInstance_Configurator.h"

static const std::string ApplicationName = "Arch1eNgine";

bool Application::Initiate()
{
	mWindowSystem = std::make_unique<WindowSystem>();
	mRenderingSystem = std::make_unique<RenderingSystem>();

	RenderingInstanceType Type = RenderingInstanceType::Vulkan;

	CreateRenderer(Type);
	CreateApplicationWindow(Type);

	Configurator::Window_RenderingInstance Configurator(mWindowSystem->GetMainWindow(), mRenderingSystem->GetRenderingInstance());

	Configurator.ConfigureImplementations();

	MainLoop();

	return false;
}

void Application::CreateApplicationWindow(RenderingInstanceType Type)
{

	WindowProperties Properties;

	Properties.Title = "Arch1eNgine";
	Properties.WindowPosition.Mode = WindowPositionMode::Centered;
	Properties.Width = 640;
	Properties.Height = 480;
	Properties.RendererType = static_cast<WindowRendererType>(Type);

	mWindowSystem->CreateNewWindow(Properties);
}

void Application::CreateRenderer(RenderingInstanceType Type)
{
	RenderingInstanceProperties Properties;

	Properties.Type = RenderingInstanceType::Vulkan;

	mRenderingSystem->CreateInstance(Properties);
}

void Application::MainLoop()
{
	while (mWindowSystem->GetMainWindow() != nullptr)
	{
		LogicLoop();
		RenderingLoop();
	}
}

void Application::LogicLoop()
{

}

void Application::RenderingLoop()
{
	mRenderingSystem->GetRenderingInstance()->SetClearColor(Vector4<float>(0.f, 0.f, 0.f, 1.f));

	I::RenderingInstanceProperties_ClearColor_Impl Properties;

	Properties.ClearColorBuffer = true;

	mRenderingSystem->GetRenderingInstance()->ClearInstance(Properties);

	mWindowSystem->GetMainWindow()->SwapBuffers();
}
