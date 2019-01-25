#ifndef RENDERINGINSTANCE_VULKAN_SDL2_H
#define RENDERINGINSTANCE_VULKAN_SDL2_H

#include <vector>

#include "RenderingInstance_Impl.h"
#include "vulkan/vulkan.h"

class RenderingInstance_SDL2_Vulkan : public I::RenderingInstance_Impl
{
public:

	RenderingInstance_SDL2_Vulkan();
	~RenderingInstance_SDL2_Vulkan();

	bool AddExtensions(void* WindowHandle, std::vector<const char*>& Extensions);
	bool CreateVulkanInstance(void* WindowHandle);

	std::vector<VkLayerProperties>		CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers);
	std::vector<VkExtensionProperties>	GetAvailableExtensions();

	// Begin I::RenderingInstance_Impl Interface
	virtual void Initialize() override;
	virtual const std::string GetImplementationType() const override;
	virtual void* GetRenderingInstanceHandle() const override;
	virtual void SetRenderingInstanceHandle(void* Handle) override;
	virtual void SetSwapInterval(int Interval) override;
	virtual void SetClearColor(Vector4<float> ClearColor) override;
	virtual void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override;
	// End I::RenderingInstance_Impl Interface

private:

	bool EnableValidationLayers;
	VkInstance InstanceHandle;

};
#endif