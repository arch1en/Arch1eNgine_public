#include "DeviceHandler.h"

#include <algorithm>
#include <set>

#include "LogSystem.h"
#include "SwapChainHandler.h"

void DeviceHandler::Initiate(const DeviceHandlerCreationInfo* CreationInfo)
{
	std::vector<VkPhysicalDevice> RetrievedDevices;

	SetDesiredDeviceExtensions(CreationInfo->DesiredDeviceExtensions);

	if (!RetrievePhysicalDevices(*CreationInfo->pInstanceHandle, RetrievedDevices))
	{
		return;
	}

	if (!FilterSuitableDevices(*CreationInfo->pInstanceHandle, *CreationInfo->pSurfaceHandle, CreationInfo->pSwapChainHandler, RetrievedDevices))
	{
		return;
	}

	CacheDevices(RetrievedDevices);

	CreateLogicalDevice(*CreationInfo->pSurfaceHandle, DeviceProperties[0]);
}

bool DeviceHandler::RetrievePhysicalDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices)
{
    Devices.clear();
    uint32_t DeviceCount = 0;
    vkEnumeratePhysicalDevices(InstanceHandle, &DeviceCount, nullptr);
    if(DeviceCount == 0)
    {
        LogVk(LogType::Error, 0, "No Vulkan supported devices found.");
        return false;
    }
	Devices.resize(DeviceCount);
    vkEnumeratePhysicalDevices(InstanceHandle, &DeviceCount, Devices.data());
    return true;
}

bool DeviceHandler::FilterSuitableDevices(const VkInstance& InstanceHandle, const VkSurfaceKHR& Surface, const SwapChainHandler* aSwapChainHandler, std::vector<VkPhysicalDevice>& Devices)
{
    for(int i = Devices.size() - 1; i >= 0; i--)
    {
        if(!IsDeviceSuitable(Surface, Devices[i], aSwapChainHandler))
        {
            Devices.erase(Devices.begin() + i);
        }
    }

    if(Devices.size() == 0)
    {
        LogVk(LogType::Error, 0, "No Vulkan supported devices that are suitable.");
        return false;
    }

    return true;
}

bool DeviceHandler::IsDeviceSuitable(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device, const SwapChainHandler* aSwapChainHandler) const
{
	int Rating = GetDeviceSuitabilityRating(Device);
	QueueFamilies Indices = RetrieveQueueFamilies(Surface, Device);
	bool AreExtensionsSupported = CheckDeviceExtensionSupport(Device, mDesiredDeviceExtensions);
	bool SwapChainAdequate = aSwapChainHandler->IsAdequate(Device, Surface);

    return Rating != 0 && Indices.IsComplete() && AreExtensionsSupported && SwapChainAdequate;
}

bool DeviceHandler::CheckDeviceExtensionSupport(const VkPhysicalDevice & aDevice, const std::vector<const char*> aDesiredDeviceExtensions) const
{
	uint32_t ExtensionsCount = 0;

	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &ExtensionsCount, nullptr);

	std::vector<VkExtensionProperties> AvailableExtensions(ExtensionsCount);
	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &ExtensionsCount, AvailableExtensions.data());

	std::set<std::string> RequiredExtensions(aDesiredDeviceExtensions.begin(), aDesiredDeviceExtensions.end());

	for (const auto& Extension : AvailableExtensions)
	{
		RequiredExtensions.erase(Extension.extensionName);
	}

	return RequiredExtensions.empty() == true;
}

int DeviceHandler::GetDeviceSuitabilityRating(const VkPhysicalDevice& Device) const
{
	if (Device == VK_NULL_HANDLE)
	{
		return 0;
	}

    int Rating = 0;

    VkPhysicalDeviceProperties DeviceProperties;
    VkPhysicalDeviceFeatures DeviceFeatures;

    vkGetPhysicalDeviceProperties(Device, &DeviceProperties);
    vkGetPhysicalDeviceFeatures(Device, &DeviceFeatures);

    if(DeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) Rating += 100;

    if(!DeviceFeatures.geometryShader)
    {
        return 0;
    }

    return Rating;
}

void DeviceHandler::SetDesiredDeviceExtensions(const std::vector<const char*> aDesiredDeviceExtensions)
{
	mDesiredDeviceExtensions = aDesiredDeviceExtensions;
}

void DeviceHandler::CacheDevices(std::vector<VkPhysicalDevice>& Devices)
{
    DeviceProperties.clear();
    DeviceProperties.resize(Devices.size());

    for(size_t i = 0; i < DeviceProperties.size(); i++)
    {
        DeviceProperties[i].DeviceHandle = Devices[i];
        DeviceProperties[i].Rating = GetDeviceSuitabilityRating(Devices[i]);
    }

	std::sort(DeviceProperties.begin(), DeviceProperties.end(), [](const PhysicalDeviceProperties& LHS, const PhysicalDeviceProperties& RHS)
	{
		return LHS.Rating < RHS.Rating;
	});
}

const QueueFamilies DeviceHandler::RetrieveQueueFamilies(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const
{
	QueueFamilies Indices;
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;

	uint32_t QueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);
	if (QueueFamilyCount == 0)
	{
		LogVk(LogType::Error, 0, "No Queue Family properties for physical devices were found.");
		return QueueFamilies();
	}

	QueueFamilyProperties.resize(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilyProperties.data());

	if (!FilterSuitableQueueFamilies(Surface, QueueFamilyProperties, Device, Indices))
	{
		return QueueFamilies();
	}

	return Indices;
}

bool DeviceHandler::FilterSuitableQueueFamilies(const VkSurfaceKHR& Surface, std::vector<VkQueueFamilyProperties>& QueueFamiliesProperties, const VkPhysicalDevice& Device, QueueFamilies& Indices) const
{
	if (QueueFamiliesProperties.size() == 0)
	{
		LogVk(LogType::Error, 0, "No queue families properties to filter.");
		return false;
	}

	int i = 0;
	for (const auto& QueueFamily : QueueFamiliesProperties)
	{
		if (IsGraphicsQueueFamilySuitable(QueueFamily))
		{
			Indices.FamilyIndices[static_cast<size_t>(QueueFamilyIndices::GraphicsFamily)].Index = i;
		}
		else if(IsPresentationQueueFamilySuitable(QueueFamily, Device, Surface, i))
		{
			Indices.FamilyIndices[static_cast<size_t>(QueueFamilyIndices::PresentationFamily)].Index = i;
		}
		
		if (Indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return true;
}

bool DeviceHandler::IsGraphicsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties) const
{
	return QueueFamilyProperties.queueCount > 0 && QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

bool DeviceHandler::IsPresentationQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties, const VkPhysicalDevice& Device, const VkSurfaceKHR& SurfaceHandle, int Index) const
{
	VkBool32 SurfaceSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, SurfaceHandle, &SurfaceSupport);
	return QueueFamilyProperties.queueCount > 0 && SurfaceSupport;
}

void DeviceHandler::CreateLogicalDevice(const VkSurfaceKHR& Surface, const PhysicalDeviceProperties& PhysicalDevice)
{
	QueueFamilies Families = RetrieveQueueFamilies(Surface, PhysicalDevice.DeviceHandle);

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

	for (const auto& Family : Families.FamilyIndices)
	{
		VkDeviceQueueCreateInfo QueueCreateInfo = {};
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = Family.Index.value();
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &Family.QueuePriority;
		QueueCreateInfos.push_back(QueueCreateInfo);
	}

	VkPhysicalDeviceFeatures DeviceFeatures = {};

	VkDeviceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.queueCreateInfoCount = static_cast<uint32_t>(QueueCreateInfos.size());
	CreateInfo.pQueueCreateInfos = QueueCreateInfos.data();
	CreateInfo.pEnabledFeatures = &DeviceFeatures;
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(mDesiredDeviceExtensions.size());
	CreateInfo.ppEnabledExtensionNames = mDesiredDeviceExtensions.data();
	
	const std::vector<const char*>& ValidationLayers = LogSystem::GetInstance()->GetVulkanLogger()->GetValidationLayers();
	bool EnableValidationLayers = LogSystem::GetInstance()->GetVulkanLogger()->AreValidationLayersEnabled();
	if (EnableValidationLayers)
	{
		CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		CreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(PhysicalDevice.DeviceHandle, &CreateInfo, nullptr, &LogicalDevice))
	{
		LogVk(LogType::Error, 0, "Logical device creation failed.");
		return;
	}
}

const std::vector<PhysicalDeviceProperties>* DeviceHandler::GetPhysicalDevicesProperties() const
{
	return &DeviceProperties;
}

const VkDevice * DeviceHandler::GetLogicalDeviceHandle() const
{
	return &LogicalDevice;
}

void DeviceHandler::Cleanup()
{
	vkDestroyDevice(LogicalDevice, nullptr);
}