#ifndef WINDOW_SDL2_VULKAN_H
#define WINDOW_SDL2_VULKAN_H

#include "vulkan/vulkan.h"

#include "Window_SDL2.h"

class Window_SDL2_Vulkan : public Window_SDL2
{
public:
	Window_SDL2_Vulkan(WindowProperties Properties);
	std::string GetImplementationType() override;
	void SwapBuffers() override;
	void CreateWindowSurface(const VkInstance* InstanceHandle, VkSurfaceKHR* SurfaceHandle);
};

#endif