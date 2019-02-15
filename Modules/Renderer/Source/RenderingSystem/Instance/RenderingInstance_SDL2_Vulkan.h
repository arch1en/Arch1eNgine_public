#ifndef RENDERINGINSTANCE_VULKAN_SDL2_H
#define RENDERINGINSTANCE_VULKAN_SDL2_H

#include <vector>
#include <memory>

#include "vulkan/vulkan.h"

#include "RenderingInstance_Impl.h"
#include "RenderingSystem/Vulkan/DeviceHandler.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceHandler.h"

struct SDL_Window;

class RenderingInstance_SDL2_Vulkan : public I::RenderingInstance_Impl
{
public:

	RenderingInstance_SDL2_Vulkan();
	~RenderingInstance_SDL2_Vulkan();

	bool AddExtensions(SDL_Window* WindowHandle, std::vector<const char*>& Extensions);
	bool CreateVulkanInstance(void* WindowHandle);
	void CreateDeviceHandler();
	void CreateSurfaceHandler();

	std::vector<VkLayerProperties>		CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers);
	std::vector<VkExtensionProperties>	GetAvailableExtensions();

	// Begin I::RenderingInstance_Impl Interface
	virtual void Initialize() override;
	virtual const std::string GetImplementationType() const override;
	const VkInstance* GetRenderingInstanceHandle() const;
	//virtual void SetRenderingInstanceHandle(void* Handle) override;
	virtual void SetSwapInterval(int Interval) override;
	virtual void SetClearColor(Vector4<float> ClearColor) override;
	virtual void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override;
	// End I::RenderingInstance_Impl Interface

	SurfaceHandler* GetSurfaceHandler() const;
	DeviceHandler* GetDeviceHandler() const;

private:

	VkInstance InstanceHandle;

	std::unique_ptr<DeviceHandler> mPhysicalDeviceHandler;
	std::unique_ptr<SurfaceHandler> mSurfaceHandler;

};
#endif