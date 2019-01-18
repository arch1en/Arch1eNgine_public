
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

	Configurator.AttachRenderingContextHandleToWindow();

	LogicLoop();

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

void Application::LogicLoop()
{

}

void Application::RenderingLoop()
{

}
