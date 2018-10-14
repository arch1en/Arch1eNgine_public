
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Logger.h"

#include <string>
#include <memory>

struct SDL_Window;
class Renderer;

class Application
{
public:

	Application() {}
	~Application() {}

	bool Initiate();
	void CreateApplicationWindow();
	void CreateRenderer(std::string RendererType);
	void Loop();

	std::string GetRendererType() const;

private:

	SDL_Window * ApplicationWindow;

	void LogicLoop();
	void RenderingLoop();

	std::shared_ptr<Renderer*> mRenderer = nullptr;

};

#endif