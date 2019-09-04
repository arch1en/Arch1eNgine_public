#ifndef RENDERINGINSTANCE_VULKAN_SDL2_H
#define RENDERINGINSTANCE_VULKAN_SDL2_H

#include <vector>

#include "vulkan/vulkan.h"

#include "RenderingInstance_Impl.h"
#include "RenderingSystem/Vulkan/DeviceHandler.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceHandler.h"
#include "RenderingSystem/Vulkan/SwapChainHandler.h"
//#include "RenderingSystem/Vulkan/PipelineSystem/PipelineSystem.h"
//#include "RenderingSystem/Vulkan/PipelineSystem/RenderPassManager.h"
//#include "RenderingSystem/Vulkan/PipelineSystem/CommandsHandler.h"

// [todo] 

//constexpr int MaxFramesInFlight = 2;

class RenderingInstance_SDL2_Vulkan : public I::RenderingInstance_Impl
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

	std::vector<VkLayerProperties>		CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers);
	std::vector<VkExtensionProperties>	GetAvailableExtensions();

	DeviceHandler*		const	GetDeviceHandler() const;
	SurfaceHandler*		const	GetSurfaceHandler() const;
	SwapChainHandler*	const	GetSwapChainHandler() const;

	// Begin I::RenderingInstance_Impl Interface
	virtual void Initialize() override;
	virtual const std::string GetImplementationType() const override;
	virtual void* GetRenderingInstanceHandle() override;
	virtual void SetRenderingInstanceHandle(void* Handle) override;
	virtual void SetSwapInterval(int Interval) override;
	virtual void SetClearColor(Vector4<float> ClearColor) override;
	virtual void RenderLoop() override;
	virtual void ResizeCanvas(int Width, int Height) override;
	virtual void ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties) override;
	// End I::RenderingInstance_Impl Interface

private:

	VkInstance InstanceHandle;

	std::unique_ptr<DeviceHandler> mDeviceHandler;
	std::unique_ptr<SwapChainHandler> mSwapChainHandler;
	std::unique_ptr<SurfaceHandler> mSurfaceHandler;

};
#endif