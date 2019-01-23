
#include "Application.h"

#include "Configurators/Window_RenderingContext_Configurator.h"

static const std::string ApplicationName = "Arch1eNgine";

bool Application::Initiate()
{
	mWindowSystem = std::make_unique<WindowSystem>();
	mRenderingSystem = std::make_unique<RenderingSystem>();

	CreateApplicationWindow();
	CreateRenderer();

	Configurator::Window_RenderingContext Configurator(mWindowSystem->GetMainWindow(), mRenderingSystem->GetRenderingContext());

	Configurator.ConfigureImplementations();

	MainLoop();

	return false;
}

void Application::CreateApplicationWindow()
{

	WindowProperties Properties;

	Properties.Title = "Arch1eNgine";
	Properties.WindowPosition.Mode = WindowPositionMode::Centered;
	Properties.Width = 640;
	Properties.Height = 480;

	mWindowSystem->CreateWindow(Properties);
}

void Application::CreateRenderer()
{
	RenderingContextProperties Properties;

	Properties.Type = RenderingContextType::OpenGL;

	mRenderingSystem->CreateContext(Properties);
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
	mRenderingSystem->GetRenderingContext()->SetClearColor(Vector4<float>(0.f, 0.f, 0.f, 1.f));

	I::RenderingContextProperties_ClearColor_Impl Properties;

	Properties.ClearColorBuffer = true;

	mRenderingSystem->GetRenderingContext()->ClearContext(Properties);

	mWindowSystem->GetMainWindow()->SwapBuffers();
}
