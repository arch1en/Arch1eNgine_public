#ifndef SWAPCHAINHANDLER_H
#define SWAPCHAINHANDLER_H

#include <vector>
#include <vulkan/vulkan.h>

struct QueueFamilies;
struct QueueFamilyProperties;

struct SwapChainHandlerCreationInfo
{
	const VkPhysicalDevice* PhysicalDevice;
	const VkDevice*	LogicalDevice;
	const VkSurfaceKHR* Surface;
	const QueueFamilies* Families;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
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
	const VkExtent2D& GetSwapChainExtent() const;
	void RetrieveSwapChainImages(const VkDevice& Device);

	void Destroy(const VkDevice* Device);
private:

	const SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const;

	VkSwapchainKHR SwapChainHandle;
	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;

	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
};

#endif