
#include "Application.h"

#include "Configurators/Window_RenderingInstance_Configurator.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
#include "RenderingSystem/Vulkan/SwapChainHandler.h"
#include "RenderingSystem/Vulkan/RenderPassManager.h"
#include "examples/imgui_impl_vulkan.h"
#include "examples/imgui_impl_sdl.h"
#include "FileSystem/FileSystem.h"
#include "WindowSystem/Implementations/Window_SDL2.h" // [Todo] This should not be here.

#include "SDL.h"

bool Application::Initiate()
{
	mWindowSystem = std::make_unique<WindowSystem>();
	mRenderingSystem = std::make_unique<RenderingSystem>();
	mEngineEditor = std::make_unique<EngineEditor>(mWindowSystem.get(), mRenderingSystem.get());
	// Renderer
	RenderingInstanceType Type = RenderingInstanceType::Vulkan;

	CreateRenderer(Type);
	CreateApplicationWindow(Type);

	Configurator::Window_RenderingInstance WRIConfigurator(mWindowSystem->GetMainWindow(), mRenderingSystem->GetRenderingInstance());
	WRIConfigurator.Configure();

	mRenderingSystem->GetRenderingInstance()->LoadTextureImage(FileSystem::Path(FileSystem::Get()->GetModuleAssetsDir("Renderer") + "/Textures/texture2.jpg").c_str(), TextureImageFormat::R8G8B8A8_SRGB, "Main");

	//Temp
	auto Rend = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingSystem->GetRenderingInstance());
	const ImageData* ImgData = Rend->GetSwapChainHandler()->GetMemoryManager()->GetImageDataByID("Main");
	Rend->GetSwapChainHandler()->GetRenderPassManager()->GetPipelineSystem()->AssociateImage("main", ImgData);
	//~Temp

	// EngineEditor
	//Configurator::Renderer_EngineEditor REEConfigurator(mWindowSystem.get(), mRenderingSystem.get(), mEngineEditor.get());
	//REEConfigurator.Configure();
	//mEngineEditor->Initiate();

	MainLoop();

	return false;
}

void Application::CreateApplicationWindow(RenderingInstanceType Type)
{
	WindowProperties Properties;

	Properties.Title = "Arch1eNgine";
	Properties.WindowPosition.Mode = WindowPositionMode::Custom;
	Properties.WindowPosition.PositionX = 100;
	Properties.WindowPosition.PositionY = 100;
	Properties.Width = 640;
	Properties.Height = 480;
	Properties.RendererType = static_cast<WindowRendererType>(Type);
	Properties.Resizeable = true;

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
	while (mWindowSystem->IsMainWindowAvailable())
	{
		LogicLoop();
		RenderingLoop();
	}
}

void Application::LogicLoop()
{
	SDL_Event Event;
	
	while(SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
		{
			mWindowSystem->DestroyAllWindows();
			break;
		}
		case SDL_WINDOWEVENT_RESIZED:
		{
			int Width, Height;
			SDL_GetWindowSize(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()), &Width, &Height);
			mRenderingSystem->GetRenderingInstance()->ResizeCanvas(Width,Height);
			break;
		}
		}
	}
	// [todo] Made for SDL for now. Some kind of API binding system would be good to do here.

}

void Application::RenderingLoop()
{
	int Width, Height;
	if (mWindowSystem->IsMainWindowAvailable())
	{
		SDL_GetWindowSize(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()), &Width, &Height);
		//mEngineEditor->Render(Vector2<>(float(Width), float(Height)));
		mRenderingSystem->GetRenderingInstance()->RenderLoop();
	}

	//mRenderingSystem->GetRenderingInstance()->SetClearColor(Vector4<float>(0.f, 0.f, 0.f, 1.f));

//	I::RenderingInstanceProperties_ClearColor_Impl Properties;

//	Properties.ClearColorBuffer = true;

//	mRenderingSystem->GetRenderingInstance()->ClearInstance(Properties);

	//mWindowSystem->GetMainWindow()->SwapBuffers();
}

void Configurator::Renderer_EngineEditor::Configure()
{

}

void Configurator::Renderer_EngineEditor::OnResizeCanvasVulkan(int Width, int Height)
{

}
