#ifndef SWAPCHAINHANDLER_H
#define SWAPCHAINHANDLER_H

#include <vector>
#include <vulkan/vulkan.h>

class QueueFamilyHandler;
struct QueueFamilyProperties;

struct SwapChainHandlerCreationInfo
{
	const VkPhysicalDevice* mPhysicalDevice;
	const VkDevice*	mLogicalDevice;
	const VkSurfaceKHR* mSurface;
	const QueueFamilyHandler* mQueueFamilyHandler;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR mCapabilities;
	std::vector<VkSurfaceFormatKHR> mFormats;
	std::vector<VkPresentModeKHR> mPresentModes;
};

class SwapChainHandler
{
public:

	SwapChainHandler() {}
	virtual ~SwapChainHandler() {}

	void CreateSwapChain(const SwapChainHandlerCreationInfo& CreationInfo);
	void CreateSwapChainImageView(const VkDevice& Device);

	// We need to make sure, that the VK_KHR_SWAPCHAIN_EXTENSION_NAME extension is available before checking SwapChain adequatibility.
	bool IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> AvailablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities);
	void RetrieveSwapChainImages(const VkDevice& Device, std::vector<VkImage>& SwapChainImages);

	void Destroy(const VkDevice* Device);

	const VkSwapchainKHR* GetSwapChainHandle();
	const VkFormat GetSwapChainImageFormat() const;
	const VkExtent2D GetSwapChainExtent() const;
	const std::vector<VkImageView>* GetSwapChainImageViews() const;

private:

	const SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;

	VkSwapchainKHR mSwapChainHandle;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent;

	std::vector<VkImage> mSwapChainImages;
	std::vector<VkImageView> mSwapChainImageViews;
};

#endif