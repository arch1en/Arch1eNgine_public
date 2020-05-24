#ifndef SWAPCHAINHANDLER_H
#define SWAPCHAINHANDLER_H

#include <map>
#include <vector>
#include <vulkan/vulkan.h>

#include "RenderingSystem/Vulkan/RenderPassManager.h"
#include "MemoryManager.h"
#include "ShaderSystem.h"

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
	void CreateMainRenderPass(const VkDevice* LogicalDevice, const VkPhysicalDevice* PhysicalDevice, const QueueFamilyHandler* QueueFamilyHandler);
	void ReCreateSwapChain(const SwapChainCreationInfo& CreationInfo);
	void CreateSwapChainImageView(const VkDevice& Device);

	void CreateShaderSystem();
	void CreateRenderPassManager();
	void CreateMemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	void CreateSemaphores(const VkDevice* Device);
	void CreateFences(const VkDevice* Device);

	void CreateCommandPool(const CommandPoolCreateInfo& CreateInfo);

	EDrawFrameErrorCode DrawFrame(const VkDevice& Device, const VkQueue& PresentQueueHandle);

	void RequestFrameBufferResizing();

	// We need to make sure, that the VK_KHR_SWAPCHAIN_EXTENSION_NAME extension is available before checking SwapChain adequatibility.
	bool IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> AvailablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);

	void Cleanup(const VkDevice* Device);
	void Destroy(const VkDevice* Device);

	const VkSwapchainKHR* GetSwapChainHandle();
	const VkFormat GetSwapChainImageFormat() const;
	void SetActualSwapChainExtent(VkExtent2D Extent);
	const VkExtent2D GetActualSwapChainExtent() const;
	const VkExtent2D GetSwapChainExtent() const;
	const std::vector<VkImageView> GetSwapChainImageViews() const;

	RenderPassManager*	const	GetRenderPassManager() const;
	MemoryManager*		const	GetMemoryManager() const;

	const VkCommandPool*		const	GetCommandPool() const;

	const SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;

	std::vector<VkImage>* GetSwapChainImages();

private:

	bool mRequestFrameBufferResizing = false;
	size_t mCurrentFrameIndex = 0;


	VkSwapchainKHR mSwapChainHandle;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent; // Cached computed extent.
	VkExtent2D mSwapChainActualExtent; // Proposed extent (eg. for window resizing).
	VkCommandPool mCommandPool;

	std::unique_ptr<RenderPassManager> mRenderPassManager;
	std::unique_ptr<MemoryManager> mMemoryManager;
	std::unique_ptr<ShaderSystem> mShaderSystem;

	std::vector<VkImage> mSwapChainImages;			// SwapChain holds images that can be then retrieved by render passes, and drawn onto.
	std::vector<VkImageView> mSwapChainImageViews;	// 

	std::vector<VkSemaphore>	mImageAvailableSemaphores;
	std::vector<VkFence>		mInFlightFences;

	// SwapChain automatically creates images (thats, 
	void RetrieveSwapChainImages(const VkDevice& Device, std::vector<VkImage>& SwapChainImages);

	void DestroyFenceArray(const VkDevice& Device, std::vector<VkFence>& Array);

};

#endif