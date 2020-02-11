#include "Surface_SDL.h"

#include "SDL.h"
#include "SDL_vulkan.h"
#include "vulkan/vulkan_win32.h"
#include "Debug/LogSystem.h"

Surface_SDL::Surface_SDL(const SurfaceHandlerCreationData* Data)
	: Surface_Base{ Data }
{
	Assert(Data->IsTypeOf("SurfaceHandlerCreationData_SDL"), "Wrong data type");
	const SurfaceHandlerCreationData_SDL* WinData = static_cast<const SurfaceHandlerCreationData_SDL*>(Data);

	VkWin32SurfaceCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	CreateInfo.hwnd = WinData->WindowInfo->info.win.window;
	CreateInfo.hinstance = GetModuleHandle(nullptr);

	if (vkCreateWin32SurfaceKHR(*WinData->VulkanInstanceRef, &CreateInfo, nullptr, &SurfaceHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Window surface creation failed !");
	}

	if (SDL_Vulkan_CreateSurface(WinData->WindowHandle, *WinData->VulkanInstanceRef, &SurfaceHandle) != SDL_bool::SDL_TRUE)
	{
		LogVk(LogType::Error, 0, "SDL surface creation failed.");
	}

}

void Surface_SDL::Destroy(const VkInstance& InstanceHandle)
{
	// InstanceHandle must be valid in order to destroy surface.
	vkDestroySurfaceKHR(InstanceHandle, SurfaceHandle, nullptr);
}
