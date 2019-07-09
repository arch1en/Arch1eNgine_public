#include "SwapChainHandler.h"

#include <algorithm>
#include <limits>

#include "LogSystem.h"
#include "DeviceHandler.h" // CIRCULAR DEPENDENCY !

void SwapChainHandler::CreateSwapChain(const SwapChainHandlerCreationInfo& CreationInfo)
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(*CreationInfo.PhysicalDevice, *CreationInfo.Surface);

	VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SupportDetails.Formats);
	VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SupportDetails.PresentModes);
	VkExtent2D Extent = ChooseSwapExtent(SupportDetails.Capabilities);

	uint32_t ImageCount = SupportDetails.Capabilities.minImageCount + 1;

	if (SupportDetails.Capabilities.maxImageCount > 0 && ImageCount > SupportDetails.Capabilities.maxImageCount)
	{
		ImageCount = SupportDetails.Capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.surface = *CreationInfo.Surface;
	CreateInfo.minImageCount = ImageCount;
	CreateInfo.imageFormat = SurfaceFormat.format;
	CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	CreateInfo.imageExtent = Extent;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t QueueFamilyIndices[] = { 
		CreationInfo.Families->FamilyIndices[static_cast<size_t>(QueueFamilyIndices::GraphicsFamily)].Index.value(),
		CreationInfo.Families->FamilyIndices[static_cast<size_t>(QueueFamilyIndices::PresentationFamily)].Index.value()
	};

	if (CreationInfo.Families->FamilyIndices[static_cast<size_t>(QueueFamilyIndices::GraphicsFamily)].Index != CreationInfo.Families->FamilyIndices[static_cast<size_t>(QueueFamilyIndices::PresentationFamily)].Index)
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		CreateInfo.queueFamilyIndexCount = 2;
		CreateInfo.pQueueFamilyIndices = QueueFamilyIndices;
	}
	else
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		CreateInfo.queueFamilyIndexCount = 0; // Optional
		CreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	CreateInfo.preTransform = SupportDetails.Capabilities.currentTransform;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.presentMode = PresentMode;
	CreateInfo.clipped = VK_TRUE;
	CreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(*CreationInfo.LogicalDevice, &CreateInfo, nullptr, &SwapChainHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Swap chain creation failure !");
		return;
	}

	RetrieveSwapChainImages(*CreationInfo.LogicalDevice);
}

void SwapChainHandler::CreateSwapChainImageView(const VkDevice& Device)
{
	SwapChainImageViews.resize(SwapChainImages.size());

	for (size_t i = 0; i < SwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo CreateInfo = {};
		
		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.image = SwapChainImages[i];
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.format = SwapChainImageFormat;
		
		CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		CreateInfo.subresourceRange.baseMipLevel = 0;
		CreateInfo.subresourceRange.levelCount = 1;
		CreateInfo.subresourceRange.baseArrayLayer = 0;
		CreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(Device, &CreateInfo, nullptr, &SwapChainImageViews[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Failed to create image view !");
		}
	}
}

void SwapChainHandler::RetrieveSwapChainImages(const VkDevice& Device)
{
	uint32_t ImageCount = 0;
	vkGetSwapchainImagesKHR(Device, SwapChainHandle, &ImageCount, nullptr);
	SwapChainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(Device, SwapChainHandle, &ImageCount, SwapChainImages.data());
}

void SwapChainHandler::Destroy(const VkDevice* Device)
{
	vkDestroySwapchainKHR(*Device, SwapChainHandle, nullptr);
	for (auto ImageView : SwapChainImageViews)
	{
		vkDestroyImageView(*Device, ImageView, nullptr);
	}
}

bool SwapChainHandler::IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(Device, Surface);
	return !SupportDetails.Formats.empty() && !SupportDetails.PresentModes.empty();
}

VkSurfaceFormatKHR SwapChainHandler::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	if (AvailableFormats.size() == 1 && AvailableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& AvailableFormat : AvailableFormats)
	{
		if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return AvailableFormat;
		}
	}

	return AvailableFormats[0];
}

VkPresentModeKHR SwapChainHandler::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> AvailablePresentModes)
{
	VkPresentModeKHR BestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& AvailablePresentMode : AvailablePresentModes)
	{
		if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return AvailablePresentMode;
		}
		else if (AvailablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			BestMode = AvailablePresentMode;
		}
	}
	return BestMode;
}

VkExtent2D SwapChainHandler::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& Capabilities)
{

	// todo: windows.h has max and min macro, there fore using std::min and std::max is colliding with these macros, 
#undef min
#undef max
	if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return Capabilities.currentExtent;
	}
	else
	{
		constexpr uint32_t StandardWidth = 800;
		constexpr uint32_t StandardHeight = 600;

		VkExtent2D ActualExtent = { StandardWidth, StandardHeight };

		ActualExtent.width = std::max(Capabilities.minImageExtent.width, std::min(Capabilities.maxImageExtent.width, ActualExtent.width));
		ActualExtent.height = std::max(Capabilities.minImageExtent.height, std::min(Capabilities.maxImageExtent.height, ActualExtent.height));

		return ActualExtent;
	}
}

const SwapChainSupportDetails SwapChainHandler::QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Details.Capabilities);

	uint32_t FormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.Formats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, Details.Formats.data());
	}

	uint32_t PresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0)
	{
		Details.PresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, Details.PresentModes.data());
	}

	return Details;
}

const VkExtent2D& SwapChainHandler::GetSwapChainExtent() const
{
	return SwapChainExtent;
}