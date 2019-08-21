#include "SwapChainHandler.h"

#include <algorithm>
#include <limits>

#include "LogSystem.h"
#include "DeviceHandler.h" // CIRCULAR DEPENDENCY !

void SwapChainHandler::CreateSwapChain(const SwapChainHandlerCreationInfo& CreationInfo)
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(*CreationInfo.mPhysicalDevice, *CreationInfo.mSurface);

	VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SupportDetails.mFormats);
	VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SupportDetails.mPresentModes);
	VkExtent2D Extent = ChooseSwapExtent(SupportDetails.mCapabilities);

	uint32_t ImageCount = SupportDetails.mCapabilities.minImageCount + 1;

	if (SupportDetails.mCapabilities.maxImageCount > 0 && ImageCount > SupportDetails.mCapabilities.maxImageCount)
	{
		ImageCount = SupportDetails.mCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.surface = *CreationInfo.mSurface;
	CreateInfo.minImageCount = ImageCount;
	CreateInfo.imageFormat = SurfaceFormat.format;
	CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	CreateInfo.imageExtent = Extent;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (CreationInfo.mQueueFamilyHandler->GetNumberOfQueueFamilies() > 1)
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		CreateInfo.queueFamilyIndexCount = CreationInfo.mQueueFamilyHandler->GetNumberOfQueueFamilies();
		CreateInfo.pQueueFamilyIndices = CreationInfo.mQueueFamilyHandler->GetQueueFamiliesIndices().data();
	}
	else
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		CreateInfo.queueFamilyIndexCount = 0; // Optional
		CreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	CreateInfo.preTransform = SupportDetails.mCapabilities.currentTransform;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.presentMode = PresentMode;
	CreateInfo.clipped = VK_TRUE;
	CreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(*CreationInfo.mLogicalDevice, &CreateInfo, nullptr, &mSwapChainHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Swap chain creation failure !");
		return;
	}

	// Cache for image view creation
	mSwapChainImageFormat = SurfaceFormat.format;
	mSwapChainExtent = Extent;

	RetrieveSwapChainImages(*CreationInfo.mLogicalDevice, mSwapChainImages);

	CreateSwapChainImageView(*CreationInfo.mLogicalDevice);
}

void SwapChainHandler::CreateSwapChainImageView(const VkDevice& Device)
{
	mSwapChainImageViews.resize(mSwapChainImages.size());

	for (size_t i = 0; i < mSwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo CreateInfo = {};
		
		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.image = mSwapChainImages[i];
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.format = mSwapChainImageFormat;
		
		CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		CreateInfo.subresourceRange.baseMipLevel = 0;
		CreateInfo.subresourceRange.levelCount = 1;
		CreateInfo.subresourceRange.baseArrayLayer = 0;
		CreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(Device, &CreateInfo, nullptr, &mSwapChainImageViews[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Failed to create image view !");
		}
	}
}

void SwapChainHandler::RetrieveSwapChainImages(const VkDevice& Device, std::vector<VkImage>& SwapChainImages)
{
	uint32_t ImageCount = 0;
	vkGetSwapchainImagesKHR(Device, mSwapChainHandle, &ImageCount, nullptr);
	SwapChainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(Device, mSwapChainHandle, &ImageCount, SwapChainImages.data());
}

void SwapChainHandler::Destroy(const VkDevice* Device)
{
	vkDestroySwapchainKHR(*Device, mSwapChainHandle, nullptr);
	for (auto ImageView : mSwapChainImageViews)
	{
		vkDestroyImageView(*Device, ImageView, nullptr);
	}
}

bool SwapChainHandler::IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(Device, Surface);
	return !SupportDetails.mFormats.empty() && !SupportDetails.mPresentModes.empty();
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

VkExtent2D SwapChainHandler::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& mCapabilities)
{

	// todo: windows.h has max and min macro, there fore using std::min and std::max is colliding with these macros, 
#undef min
#undef max
	if (mCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return mCapabilities.currentExtent;
	}
	else
	{
		constexpr uint32_t StandardWidth = 800;
		constexpr uint32_t StandardHeight = 600;

		VkExtent2D ActualExtent = { StandardWidth, StandardHeight };

		ActualExtent.width = std::max(mCapabilities.minImageExtent.width, std::min(mCapabilities.maxImageExtent.width, ActualExtent.width));
		ActualExtent.height = std::max(mCapabilities.minImageExtent.height, std::min(mCapabilities.maxImageExtent.height, ActualExtent.height));

		return ActualExtent;
	}
}

const SwapChainSupportDetails SwapChainHandler::QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Details.mCapabilities);

	uint32_t FormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.mFormats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, Details.mFormats.data());
	}

	uint32_t PresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0)
	{
		Details.mPresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, Details.mPresentModes.data());
	}

	return Details;
}

const VkFormat SwapChainHandler::GetSwapChainImageFormat() const
{
	return mSwapChainImageFormat;
}

const VkExtent2D SwapChainHandler::GetSwapChainExtent() const
{
	return mSwapChainExtent;
}

const std::vector<VkImageView>* SwapChainHandler::GetSwapChainImageViews() const
{
	return &mSwapChainImageViews;
}