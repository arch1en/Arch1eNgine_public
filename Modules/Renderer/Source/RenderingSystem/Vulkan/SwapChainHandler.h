#ifndef SWAPCHAINHANDLER_H
#define SWAPCHAINHANDLER_H

#include <vector>
#include <vulkan/vulkan.h>

#include "RenderingSystem/Vulkan/PipelineSystem.h"
#include "RenderingSystem/Vulkan/RenderPassManager.h"
#include "MemoryManager.h"

class QueueFamilyHandler;
struct QueueFamilyData;
struct QueueFamilyProperties;

enum class EDrawFrameErrorCode
{
	OK,
	SwapChainRecreationRequired,
	SwapChainImageAcquisitionFailed,
	QueueSubmissionFailed
};

struct SwapChainHandlerInitiationInfo
{
	const VkDevice* mLogicalDevice = nullptr;
	const QueueFamilyHandler* mQueueFamilyHandler = nullptr;
};

struct SwapChainCreationInfo
{
	const VkPhysicalDevice* mPhysicalDevice = nullptr;
	const VkDevice*	mLogicalDevice = nullptr;
	const VkSurfaceKHR* mSurface = nullptr;
	const QueueFamilyHandler* mQueueFamilyHandler = nullptr;
	const MemoryManager* mMemoryManager = nullptr;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR> mPresentModes;
};

struct CommandPoolCreateInfo
{
	const VkDevice* mLogicalDevice = nullptr;
	const std::vector<QueueFamilyData>* mQueueFamilyData = nullptr;
};

constexpr int MaxFramesInFlight = 2;

class SwapChainHandler
{
public:

	SwapChainHandler() {}
	virtual ~SwapChainHandler() {}

	void Initiate(const SwapChainHandlerInitiationInfo& InitiationInfo);
	void PrepareVertexMemory(const GeneralBufferCreationInfo& BufferCreationInfo, std::vector<Vertex> Vertices);
	void PrepareIndexMemory(const GeneralBufferCreationInfo& BufferCreationInfo, std::vector<uint16_t> Indices);
	void CreateSwapChain(const SwapChainCreationInfo& CreationInfo);
	void ReCreateSwapChain(const SwapChainCreationInfo& CreationInfo);
	void CreateSwapChainImageView(const VkDevice& Device);

	void CreateRenderPassManager();
	void CreatePipelineSystem();
	void CreateMemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	void CreateSemaphores(const VkDevice* Device);
	void CreateFences(const VkDevice* Device);
	void CreateDescriptorPool(const VkDevice* Device);
	void CreateDescriptorSets(const VkDevice* Device);
	void CreateCommandPool(const CommandPoolCreateInfo& CreateInfo);

	EDrawFrameErrorCode DrawFrame(const VkDevice& Device, const VkQueue& PresentQueueHandle);

	void RequestFrameBufferResizing();

	// We need to make sure, that the VK_KHR_SWAPCHAIN_EXTENSION_NAME extension is available before checking SwapChain adequatibility.
	bool IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> AvailablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);
	void RetrieveSwapChainImages(const VkDevice& Device, std::vector<VkImage>& SwapChainImages);

	void Cleanup(const VkDevice* Device);
	void Destroy(const VkDevice* Device);

	const VkSwapchainKHR* GetSwapChainHandle();
	const VkFormat GetSwapChainImageFormat() const;
	void SetActualSwapChainExtent(VkExtent2D Extent);
	const VkExtent2D GetActualSwapChainExtent() const;
	const VkExtent2D GetSwapChainExtent() const;
	const std::vector<VkImageView>* GetSwapChainImageViews() const;

	RenderPassManager*	const	GetRenderPassManager() const;
	PipelineSystem*		const	GetPipelineSystem() const;
	MemoryManager*		const	GetMemoryManager() const;

	const VkCommandPool*		const	GetCommandPool() const;

private:

	bool mRequestFrameBufferResizing = false;
	size_t mCurrentFrameIndex = 0;

	VkSwapchainKHR mSwapChainHandle;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent; // Cached computed extent.
	VkExtent2D mSwapChainActualExtent; // Proposed extent (eg. for window resizing).
	VkCommandPool mCommandPool;

	// Descriptor Pool/Sets
	VkDescriptorPool mDescriptorPool;
	std::vector<VkDescriptorSet> mDescriptorSets;

	std::unique_ptr<RenderPassManager> mRenderPassManager;
	std::unique_ptr<PipelineSystem> mPipelineSystem;
	std::unique_ptr<MemoryManager> mMemoryManager;

	std::vector<VkImage> mSwapChainImages;
	std::vector<VkImageView> mSwapChainImageViews;

	std::vector<VkSemaphore>	mImageAvailableSemaphores;
	std::vector<VkSemaphore>	mRenderFinishedSemaphores;
	std::vector<VkFence>		mInFlightFences;

	const SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;

	void DestroySemaphoreArray(const VkDevice& Device, std::vector<VkSemaphore>& Array);
	void DestroyFenceArray(const VkDevice& Device, std::vector<VkFence>& Array);

};

#endif