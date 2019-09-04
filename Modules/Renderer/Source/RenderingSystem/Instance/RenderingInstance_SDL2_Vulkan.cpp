#include "RenderingInstance_SDL2_Vulkan.h"

#include <vector>

#include "SDL.h"
#include "SDL_vulkan.h"

#include "RenderingSystem/RenderingSystemUtilities.h"
#include "LogSystem.h"

// Windows has a "max" macro.
#undef max

RenderingInstance_SDL2_Vulkan::RenderingInstance_SDL2_Vulkan()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		LogV(LogType::Error, LOGDOMAIN_WINDOW_SDL2, 0, "SDL Initiation failed!");
	}

}

RenderingInstance_SDL2_Vulkan::~RenderingInstance_SDL2_Vulkan()
{
	mSwapChainHandler->Destroy(GetDeviceHandler()->GetLogicalDeviceHandle());
	mSurfaceHandler->Destroy(&InstanceHandle);
	vkDestroyInstance(InstanceHandle, nullptr);
}

bool RenderingInstance_SDL2_Vulkan::CreateVulkanInstance(void* WindowHandle)
{
	VkApplicationInfo ApplicationInfo = {};
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pApplicationName = "Arch1eNgine";
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.pEngineName = "No Engine";
	ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	// Extensions
	std::vector<const char*> Extensions =
	{
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME	// Required for appropriate Logger_Vulkan behavior.
	};

	if (!AddExtensions(WindowHandle, Extensions))
	{
		return false;
	}
	// ~Extensions

	// Validation Layers
	LogSystem::GetInstance()->GetVulkanLogger()->AddValidationLayer("VK_LAYER_LUNARG_standard_validation");

	const std::vector<const char*>& ValidationLayers = LogSystem::GetInstance()->GetVulkanLogger()->GetValidationLayers();
	bool EnableValidationLayers = LogSystem::GetInstance()->GetVulkanLogger()->AreValidationLayersEnabled();

	if (EnableValidationLayers)
	{
		std::vector<VkLayerProperties> AvailableDesiredLayers = CheckValidationLayersAvailability(ValidationLayers);

		if (AvailableDesiredLayers.size() != ValidationLayers.size())
		{
			LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Avaliable validation layers count do not match with desired validation layers.");
			return false;
		}
	}

	// ~Validation Layers

	VkInstanceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &ApplicationInfo;
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
	CreateInfo.ppEnabledExtensionNames = Extensions.data();
	if (EnableValidationLayers)
	{
		CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		CreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&CreateInfo, nullptr, &InstanceHandle) != VK_SUCCESS)
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance creation failed.");
		return false;
	}

	// @todo [Urgent] Decide what to do with the logger class (maybe make it classless ?)
	LogSystem::GetInstance()->GetVulkanLogger()->InitiateDebugMessenger(InstanceHandle);


	// [TODO] Configurator has too much stuff in it that should be in this class (not nescesserily in this function). Move here what's nescessary.

	return true;
}

void RenderingInstance_SDL2_Vulkan::CreateRequiredSubsystems()
{
	SwapChainCreationInfo SwapChainCI = {};

	const VkDevice* LogicalDevice = GetDeviceHandler()->GetLogicalDeviceHandle();
	const DeviceHandler* const pDeviceHandler = GetDeviceHandler();

	SwapChainCI.mLogicalDevice = LogicalDevice;
	SwapChainCI.mPhysicalDevice = &pDeviceHandler->GetPhysicalDevicesProperties()->at(0).DeviceHandle;
	SwapChainCI.mSurface = GetSurfaceHandler()->GetMainSurface()->GetHandle();
	SwapChainCI.mQueueFamilyHandler = pDeviceHandler->GetQueueFamilyHandler();

	SwapChainHandlerInitiationInfo SwapChainHandlerII = {};

	SwapChainHandlerII.mLogicalDevice = LogicalDevice;

	GetSwapChainHandler()->Initiate(SwapChainHandlerII);
	GetSwapChainHandler()->CreateSwapChain(SwapChainCI);
}

void RenderingInstance_SDL2_Vulkan::CreateDeviceHandler()
{
	mDeviceHandler = std::make_unique<DeviceHandler>();
}

DeviceHandler* const RenderingInstance_SDL2_Vulkan::GetDeviceHandler() const
{
	return mDeviceHandler.get();
}

void RenderingInstance_SDL2_Vulkan::CreateSurfaceHandler()
{
	mSurfaceHandler = std::make_unique<SurfaceHandler>();
}

void RenderingInstance_SDL2_Vulkan::CreateSwapChainHandler()
{
	mSwapChainHandler = std::make_unique<SwapChainHandler>();
}

//void RenderingInstance_SDL2_Vulkan::CreateRenderPassManager()
//{
//	mRenderPassManager = std::make_unique<RenderPassManager>();
//}

//void RenderingInstance_SDL2_Vulkan::CreatePipelineSystem()
//{
//	mPipelineSystem = std::make_unique<PipelineSystem>();
//}

//void RenderingInstance_SDL2_Vulkan::CreateCommandsHandler()
//{
//	mCommandsHandler = std::make_unique<CommandsHandler>();
//}

SurfaceHandler* const RenderingInstance_SDL2_Vulkan::GetSurfaceHandler() const
{
	return mSurfaceHandler.get();
}

SwapChainHandler * const RenderingInstance_SDL2_Vulkan::GetSwapChainHandler() const
{
	return mSwapChainHandler.get();
}

//RenderPassManager* const RenderingInstance_SDL2_Vulkan::GetRenderPassManager() const
//{
//	return mRenderPassManager.get();
//}
//
//PipelineSystem* const RenderingInstance_SDL2_Vulkan::GetPipelineSystem() const
//{
//	return mPipelineSystem.get();
//}
//
//CommandsHandler* const RenderingInstance_SDL2_Vulkan::GetCommandsHandler() const
//{
//	return mCommandsHandler.get();
//}

bool RenderingInstance_SDL2_Vulkan::AddExtensions(void* WindowHandle, std::vector<const char*>& Extensions)
{
	unsigned int ExtensionsCount = 0;

	SDL_Window* Window = static_cast<SDL_Window*>(WindowHandle);

	if (!SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionsCount, nullptr))
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance extensions receivement failed.");
		return false;
	}

	size_t AdditionalExtensionsCount = Extensions.size();

	Extensions.resize(AdditionalExtensionsCount + ExtensionsCount);

	if (!SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionsCount, Extensions.data() + AdditionalExtensionsCount))
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance extensions addition failed.");
		return false;
	}

	return true;
}

std::vector<VkExtensionProperties> RenderingInstance_SDL2_Vulkan::GetAvailableExtensions()
{
	uint32_t ExtensionCount = 0;

	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
	std::vector<VkExtensionProperties> Extensions(ExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, Extensions.data());

	return Extensions;
}

std::vector<VkLayerProperties> RenderingInstance_SDL2_Vulkan::CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers)
{
	uint32_t LayerCount = 0;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	std::vector<VkLayerProperties> AvailableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	std::vector<VkLayerProperties> AvailableDesiredLayers;

	for (auto LayerName : DesiredLayers)
	{
		for (const auto& LayerProperties : AvailableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				AvailableDesiredLayers.push_back(LayerProperties);
			}
		}
	}

	return AvailableDesiredLayers;
}

void RenderingInstance_SDL2_Vulkan::Initialize()
{
}

const std::string RenderingInstance_SDL2_Vulkan::GetImplementationType() const
{
	return "SDL2/Vulkan";
}

void* RenderingInstance_SDL2_Vulkan::GetRenderingInstanceHandle()
{
	return &InstanceHandle;
}

void RenderingInstance_SDL2_Vulkan::SetRenderingInstanceHandle(void * Handle)
{
}

void RenderingInstance_SDL2_Vulkan::SetSwapInterval(int Interval)
{
}

void RenderingInstance_SDL2_Vulkan::SetClearColor(Vector4<float> ClearColor)
{
}

void RenderingInstance_SDL2_Vulkan::RenderLoop()
{
	const std::vector<VkCommandBuffer>& RenderPassCommandBuffers = *GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassCommandBuffers();

	//for (size_t i = 0; i < GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassCommandBuffers()->size(); i++)
	//{
	//	VkCommandBufferBeginInfo CommandBufferBI = {};
	//	CommandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	CommandBufferBI.flags = 0; // Optional
	//	CommandBufferBI.pInheritanceInfo = nullptr; // Optional

	//	if (vkBeginCommandBuffer(RenderPassCommandBuffers[i], &CommandBufferBI) != VK_SUCCESS)
	//	{
	//		LogVk(LogType::Error, 0, "Error beginning command buffer.");
	//	}

	//	VkRenderPassBeginInfo RenderPassBI = {};
	//	RenderPassBI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//	RenderPassBI.renderPass = *GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassHandle();
	//	RenderPassBI.framebuffer = (*GetSwapChainHandler()->GetRenderPassManager()->GetFramebuffers())[i];
	//	RenderPassBI.renderArea.offset = {0,0};
	//	RenderPassBI.renderArea.extent = GetSwapChainHandler()->GetSwapChainExtent();

	//	VkClearValue ClearColor = { 0.1f, 0.1f, 0.1f, 1.f };

	//	RenderPassBI.clearValueCount = 1;
	//	RenderPassBI.pClearValues = &ClearColor;

	//	vkCmdBeginRenderPass((*GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassCommandBuffers())[i], &RenderPassBI, VK_SUBPASS_CONTENTS_INLINE);

	//	vkCmdBindPipeline(RenderPassCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSwapChainHandler()->GetPipelineSystem()->GetPipelineHandle());

	//	vkCmdDraw(RenderPassCommandBuffers[i], 3, 1, 0, 0);

	//	vkCmdEndRenderPass(RenderPassCommandBuffers[i]);

	//	if (vkEndCommandBuffer(RenderPassCommandBuffers[i]) != VK_SUCCESS)
	//	{
	//		LogVk(LogType::Error, 0, "Failed to record command buffer!");
	//	}
	//}

	EDrawFrameErrorCode ErrorCode = GetSwapChainHandler()->DrawFrame(*GetDeviceHandler()->GetLogicalDeviceHandle(), GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData()->QueueHandle);

	switch (ErrorCode)
	{
	case EDrawFrameErrorCode::SwapChainRecreationRequired:
	{
		SwapChainCreationInfo SwapChainCI = {};

		SwapChainCI.mLogicalDevice = GetDeviceHandler()->GetLogicalDeviceHandle();
		SwapChainCI.mPhysicalDevice = &GetDeviceHandler()->GetPhysicalDevicesProperties()->at(0).DeviceHandle;
		SwapChainCI.mSurface = GetSurfaceHandler()->GetMainSurface()->GetHandle();
		SwapChainCI.mQueueFamilyHandler = GetDeviceHandler()->GetQueueFamilyHandler();

		GetSwapChainHandler()->ReCreateSwapChain(SwapChainCI);
		break;
	}
	}
}

void RenderingInstance_SDL2_Vulkan::ResizeCanvas(int Width, int Height)
{
	GetSwapChainHandler()->SetActualSwapChainExtent({ uint32_t(Width), uint32_t(Height) });
	GetSwapChainHandler()->RequestFrameBufferResizing();
}
//void RenderingInstance_SDL2_Vulkan::DrawFrame()
//{
//	const std::vector<VkCommandBuffer>& RenderPassCommandBuffers = *GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassCommandBuffers();
//	const VkDevice* Device = GetDeviceHandler()->GetLogicalDeviceHandle();
//
//	Assert(Device, "Device must be valid at this point!");
//
//	uint32_t ImageIndex;
//
//	uint64_t Timeout = std::numeric_limits <uint64_t>::max(); // Timeout in nanoseconds. Using the maximum value of a 64bit unsigned integer disables the timeout.
//
//	vkWaitForFences(*Device, 1, &InFlightFences[mCurrentFrameIndex], VK_TRUE, UINT64_MAX);
//	vkResetFences(*Device, 1, &InFlightFences[mCurrentFrameIndex]);
//
//	vkAcquireNextImageKHR(*Device, *GetSwapChainHandler()->GetSwapChainHandle(), Timeout, ImageAvailableSemaphores[0], VK_NULL_HANDLE, &ImageIndex);
//
//	VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
//
//	VkSemaphore WaitSemaphores[] = { ImageAvailableSemaphores[mCurrentFrameIndex] };
//	VkSemaphore SignalSemaphores[] = { RenderFinishedSemaphores[mCurrentFrameIndex] };
//
//	VkSubmitInfo SubmitInfo = {};
//	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//	SubmitInfo.waitSemaphoreCount = 1;
//	SubmitInfo.pWaitSemaphores = WaitSemaphores;
//	SubmitInfo.pWaitDstStageMask = WaitStages;
//	SubmitInfo.commandBufferCount = 1;
//	SubmitInfo.pCommandBuffers = &RenderPassCommandBuffers[ImageIndex];
//	SubmitInfo.pSignalSemaphores = SignalSemaphores;
//	SubmitInfo.signalSemaphoreCount = 1;
//
//	const VkQueue& PresentQueue = GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData()->QueueHandle;
//
//	if (vkQueueSubmit(PresentQueue, 1, &SubmitInfo, InFlightFences[mCurrentFrameIndex]) != VK_SUCCESS)
//	{
//		LogVk(LogType::Error, 0, "Queue submission failed!");
//	}
//
//	VkPresentInfoKHR PresentInfo = {};
//	PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//	PresentInfo.waitSemaphoreCount = 1;
//	PresentInfo.pWaitSemaphores = SignalSemaphores;
//	PresentInfo.swapchainCount = 1;
//	PresentInfo.pSwapchains = { GetSwapChainHandler()->GetSwapChainHandle() };
//	PresentInfo.pImageIndices = &ImageIndex;
//	PresentInfo.pResults = nullptr;
//
//	vkQueuePresentKHR(PresentQueue , &PresentInfo);
//
//	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % MaxFramesInFlight;
//}

void RenderingInstance_SDL2_Vulkan::ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties)
{
}

//void RenderingInstance_SDL2_Vulkan::CreateSemaphores()
//{
//	Assert(ImageAvailableSemaphores.size() == 0, "Array must be empty at this point.");
//	Assert(RenderFinishedSemaphores.size() == 0, "Array must be empty at this point.");
//
//	ImageAvailableSemaphores.resize(MaxFramesInFlight);
//	RenderFinishedSemaphores.resize(MaxFramesInFlight);
//
//	VkSemaphoreCreateInfo CreateInfo = {};
//	CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
//
//	for (int i = 0; i < ImageAvailableSemaphores.size(); i++)
//	{
//		if (vkCreateSemaphore(*GetDeviceHandler()->GetLogicalDeviceHandle(), &CreateInfo, nullptr, &ImageAvailableSemaphores[i]) != VK_SUCCESS)
//		{
//			LogVk(LogType::Error, 0, "Image Available Semaphores Creation failed!");
//		}
//	}
//
//	for (int i = 0; i < RenderFinishedSemaphores.size(); i++)
//	{
//		if (vkCreateSemaphore(*GetDeviceHandler()->GetLogicalDeviceHandle(), &CreateInfo, nullptr, &RenderFinishedSemaphores[i]) != VK_SUCCESS)
//		{
//			LogVk(LogType::Error, 0, "Rendering Finished Semaphores Creation failed!");
//		}
//	}
//}

//void RenderingInstance_SDL2_Vulkan::CreateFences()
//{
//	Assert(InFlightFences.size() == 0, "Array must be empty at this point.");
//
//	InFlightFences.resize(MaxFramesInFlight);
//
//	VkFenceCreateInfo CreateInfo = {};
//	CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
//	// By default Fences are created in an unsignaled state. That means vkWaitForFences will wait forever for them.
//	// We can change its state on the creation time, so that vkWaitForFences will catch it the first time before rendering.
//	CreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; 
//
//	for (int i = 0; i < InFlightFences.size(); i++)
//	{
//		if (vkCreateFence(*GetDeviceHandler()->GetLogicalDeviceHandle(), &CreateInfo, nullptr, &InFlightFences[i]) != VK_SUCCESS)
//		{
//			LogVk(LogType::Error, 0, "In Flight Fences Creation failed!");
//		}
//	}
//}

void RenderingInstance_SDL2_Vulkan::CleanUp()
{
	//DestroySemaphoreArray(ImageAvailableSemaphores);
	//DestroySemaphoreArray(RenderFinishedSemaphores);
	//DestroyFenceArray(InFlightFences);

	//vkDeviceWaitIdle(*GetDeviceHandler()->GetLogicalDeviceHandle());
}

//void RenderingInstance_SDL2_Vulkan::DestroySemaphoreArray(std::vector<VkSemaphore>& Array)
//{
//	for (size_t i = Array.size() - 1; i >= 0; i--)
//	{
//		vkDestroySemaphore(*GetDeviceHandler()->GetLogicalDeviceHandle(), Array[i], nullptr);
//		Array.erase(Array.begin() + i);
//	}
//}

//void RenderingInstance_SDL2_Vulkan::DestroyFenceArray(std::vector<VkFence>& Array)
//{
//	for (size_t i = Array.size() - 1; i >= 0; i--)
//	{
//		vkDestroyFence(*GetDeviceHandler()->GetLogicalDeviceHandle(), Array[i], nullptr);
//		Array.erase(Array.begin() + i);
//	}
//}
