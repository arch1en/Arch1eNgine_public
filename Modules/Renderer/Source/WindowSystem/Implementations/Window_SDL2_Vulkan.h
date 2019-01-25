#ifndef WINDOW_SDL2_VULKAN_H
#define WINDOW_SDL2_VULKAN_H

#include "Window_SDL2.h"

class Window_SDL2_Vulkan : public Window_SDL2
{
public:
	virtual bool InitiateWindow(WindowProperties Properties) override;
	virtual std::string GetImplementationType() override;
	virtual void SwapBuffers() override;
};

#endif