
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Logger.h"

#include <string>
#include <memory>

#include "WindowSystem\WindowSystem.h"
#include "RenderingSystem\RenderingSystem.h"
#include "RenderingSystem\RenderingSystemUtilities.h"

struct SDL_Window;

class Application
{
public:

	Application() {}
	~Application() {}

	bool Initiate();
	void CreateApplicationWindow();
	void CreateRenderer();

private:

	std::unique_ptr<WindowSystem> mWindowSystem;
	std::unique_ptr<RenderingSystem> mRenderingSystem;

	void LogicLoop();
	void RenderingLoop();


};

#endif