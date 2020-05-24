#ifndef RENDERINGINSTANCE_VULKAN_SDL2_H
#define RENDERINGINSTANCE_VULKAN_SDL2_H

#include <vector>

#include "vulkan/vulkan.h"

#include "RenderingInstance_Impl.h"
#include "RenderingSystem/Vulkan/DeviceHandler.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceHandler.h"
#include "RenderingSystem/Vulkan/SwapChainHandler.h"

using Parent = I::RenderingInstance_Impl;

class RenderingInstance_SDL2_Vulkan final : public Parent
{
public:

	RenderingInstance_SDL2_Vulkan();
	~RenderingInstance_SDL2_Vulkan();

	bool AddExtensions(void* WindowHandle, std::vector<const char*>& Extensions);
	bool CreateVulkanInstance(void* WindowHandle);
	void CreateRequiredSubsystems();
	void CreateDeviceHandler();
	void CreateSurfaceHandler();
	void CreateSwapChainHandler();

	std::vector<VkLayerProperties>		CheckValidationLayersAvailability(const std::vector<const char*>& DesiredLayers);
	std::vector<VkExtensionProperties>	GetAvailableExtensions();

	DeviceHandler*		const	GetDeviceHandler() const;
	SurfaceHandler*		const	GetSurfaceHandler() const;
	SwapChainHandler*	const	GetSwapChainHandler() const;

	// Begin I::RenderingInstance_Impl Interface
	void Initialize() override {}
	const std::string GetImplementationType() const override;
	void* GetRenderingInstanceHandle() override;
	void SetRenderingInstanceHandle(void* Handle) override {}
	void SetSwapInterval(int Interval) override {}
	void SetClearColor(Vector4<float> ClearColor) override {}
	void RenderLoop() override;
	void ResizeCanvas(int Width, int Height) override;
	void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override {}
	void LoadTextureImage(const char* Path, TextureImageFormat Format, const std::string& TextureID) override;
	void LoadTextureImage(unsigned char* Pixels, int TexWidth, int TexHeight, TextureImageFormat Format, const std::string& TextureID) override;
	// End I::RenderingInstance_Impl Interface

private:

	VkInstance InstanceHandle;

	std::unique_ptr<DeviceHandler> mDeviceHandler;
	std::unique_ptr<SwapChainHandler> mSwapChainHandler;
	std::unique_ptr<SurfaceHandler> mSurfaceHandler;

	VkFormat ParseTextureImageFormatToVkFormat(TextureImageFormat ImageFormat);

};
#endif