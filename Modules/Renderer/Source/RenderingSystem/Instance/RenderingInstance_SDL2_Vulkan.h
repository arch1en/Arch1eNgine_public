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
	//void CreateRenderPassManager();
	//void CreatePipelineSystem();
	//void CreateCommandsHandler();
	//void CreateSemaphores();
	//void CreateFences();

	std::vector<VkLayerProperties>		CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers);
	std::vector<VkExtensionProperties>	GetAvailableExtensions();

	DeviceHandler*		const	GetDeviceHandler() const;
	SurfaceHandler*		const	GetSurfaceHandler() const;
	SwapChainHandler*	const	GetSwapChainHandler() const;
	//RenderPassManager*	const	GetRenderPassManager() const;
	//PipelineSystem*		const	GetPipelineSystem() const;
	//CommandsHandler*	const	GetCommandsHandler() const;

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

	//size_t mCurrentFrameIndex = 0;

	VkInstance InstanceHandle;

	//std::unique_ptr<RenderPassManager> mRenderPassManager;
	std::unique_ptr<DeviceHandler> mDeviceHandler;
	std::unique_ptr<SwapChainHandler> mSwapChainHandler;
	std::unique_ptr<SurfaceHandler> mSurfaceHandler;
	//std::unique_ptr<PipelineSystem> mPipelineSystem;
	//std::unique_ptr<CommandsHandler> mCommandsHandler;

	//std::vector<VkSemaphore>	ImageAvailableSemaphores;
	//std::vector<VkSemaphore>	RenderFinishedSemaphores;
	//std::vector<VkFence>		InFlightFences;

	void CleanUp();
	//void DestroySemaphoreArray(std::vector<VkSemaphore>& Array);
	//void DestroyFenceArray(std::vector<VkFence>& Array);

};
#endif