#include "DeviceHandler.h"

#include <algorithm>

#include "LogSystem.h"

DeviceHandler::DeviceHandler(const VkInstance& InstanceHandle)
{
    std::vector<VkPhysicalDevice> RetrievedDevices;

    if(!RetrievePhysicalDevices(InstanceHandle, RetrievedDevices))
    {
        return;
    }

    if(!FilterSuitableDevices(InstanceHandle, RetrievedDevices))
    {
        return;
    }

    CacheDevices(RetrievedDevices);

	CreateLogicalDevice(DeviceProperties[0]);
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

bool DeviceHandler::FilterSuitableDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices)
{
    for(int i = Devices.size() - 1; i >= 0; i--)
    {
        if(!IsDeviceSuitable(Devices[i]))
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

bool DeviceHandler::IsDeviceSuitable(const VkPhysicalDevice& Device) const
{
	int Rating = GetDeviceSuitabilityRating(Device);
	QueueFamilyIndices Indices = RetrieveQueueFamilies(Device);

    return Rating != 0 && Indices.IsComplete();
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

QueueFamilyIndices DeviceHandler::RetrieveQueueFamilies(const VkPhysicalDevice& Device) const
{
	QueueFamilyIndices Indices;
	std::vector<VkQueueFamilyProperties> QueueFamilies;

	uint32_t QueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);
	if (QueueFamilyCount == 0)
	{
		LogVk(LogType::Error, 0, "No Queue Family properties for physical devices were found.");
		return QueueFamilyIndices();
	}

	QueueFamilies.resize(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilies.data());

	if (!FilterSuitableQueueFamilies(QueueFamilies, Indices))
	{
		return QueueFamilyIndices();
	}

	return Indices;
}

bool DeviceHandler::FilterSuitableQueueFamilies(std::vector<VkQueueFamilyProperties>& QueueFamilies, QueueFamilyIndices& Indices) const
{
	if (QueueFamilies.size() == 0)
	{
		LogVk(LogType::Error, 0, "No queue families to filter.");
		return false;
	}

	int i = 0;
	for (const auto& QueueFamily : QueueFamilies)
	{
		if (!IsQueueFamilySuitable(QueueFamily))
		{
			Indices.GraphicsFamilyIndex = i;
		}

		if (Indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return true;
}

bool DeviceHandler::IsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamily) const
{
	return QueueFamily.queueCount > 0 && QueueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

void DeviceHandler::CreateLogicalDevice(const PhysicalDeviceProperties& PhysicalDevice)
{
	QueueFamilyIndices Indices = RetrieveQueueFamilies(PhysicalDevice.DeviceHandle);

	VkDeviceQueueCreateInfo QueueCreateInfo = {};
	QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	QueueCreateInfo.queueFamilyIndex = Indices.GraphicsFamilyIndex.value();
	QueueCreateInfo.queueCount = 1;
	QueueCreateInfo.pQueuePriorities = &Indices.QueuePriority;

	VkPhysicalDeviceFeatures DeviceFeatures = {};

	VkDeviceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	CreateInfo.pQueueCreateInfos = &QueueCreateInfo;
	CreateInfo.queueCreateInfoCount = 1;
	CreateInfo.pEnabledFeatures = &DeviceFeatures;
	CreateInfo.enabledExtensionCount = 0;
	
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

void DeviceHandler::Cleanup()
{
	vkDestroyDevice(LogicalDevice, nullptr);
}