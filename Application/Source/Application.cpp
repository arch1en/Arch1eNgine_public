
#include "Application.h"

#include "SDL2.h"
#include "Renderer_OpenGL.h"

static const std::string ApplicationName = "Arch1eNgine";

bool Application::Initiate()
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		Log(DebugType::EDT_Error, "SDL Initialization failed !");
		return false;
	}
}

void Application::CreateApplicationWindow()
{
	uint8_t RendererType = GetRendererType().compare("opengl") == 0 ? SDL_WINDOW_OPENGL : GetRendererType().compare("vulkan") == 0 ? SDL_WINDOW_VULKAN : 0;

	switch (RendererType)
	{
	case SDL_WINDOW_OPENGL :
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		break;
	}
	case SDL_WINDOW_VULKAN :
	{
		break;
	}
	}

	SDL_DisplayMode DisplayMode;
	SDL_GetCurrentDisplayMode(0, &DisplayMode);
	// @todo Replace with the application name.
	ApplicationWindow = SDL_CreateWindow(ApplicationName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, RendererType | SDL_WINDOW_RESIZABLE);
}

void Application::CreateRenderer(std::string RendererType)
{
	if (RendererType.compare("opengl") == 0)
	{
		mRenderer = std::make_shared<Renderer::OpenGL>();
		mRenderer->CreateContext();
		mRenderer->AttachToWindow(ApplicationWindow);
	}
}

void Application::Loop()
{

}

std::string Application::GetRendererType() const
{
	// @todo this value needs to be loaded from the config .ini file.
	return "opengl";
}