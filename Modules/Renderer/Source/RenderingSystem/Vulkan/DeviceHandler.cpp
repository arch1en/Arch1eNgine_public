#include "DeviceHandler.h"

#include <algorithm>
#include <set>

#include "Debug/LogSystem.h"
#include "SwapChainHandler.h"

// Queue Family Handler
void QueueFamilyHandler::ResetQueueFamilyData(const std::vector<QueueFamilyData>& Data)
{
	mCachedQueueFamilyData.erase(mCachedQueueFamilyData.begin(), mCachedQueueFamilyData.end());
	mCachedQueueFamilyData.reserve(Data.size());
	mCachedQueueFamilyData = Data;

	// It is easier to pass family indices when they are aligned, one after another.
	mAlignedQueueFamilyIndices.erase(mAlignedQueueFamilyIndices.begin(), mAlignedQueueFamilyIndices.end());
	for (auto& i : mCachedQueueFamilyData)
	{
		mAlignedQueueFamilyIndices.push_back(i.FamilyIndex);
	}
}

const std::vector<uint32_t>* QueueFamilyHandler::GetAlignedQueueFamiliesIndices() const
{
	return &mAlignedQueueFamilyIndices;
}

const size_t QueueFamilyHandler::GetNumberOfQueueFamilies() const
{
	return mCachedQueueFamilyData.size();
}

const std::vector<QueueFamilyData>* QueueFamilyHandler::GetQueueFamilyData() const
{
	return &mCachedQueueFamilyData;
}

const QueueFamilyData* QueueFamilyHandler::GetPresentationSuitableQueueFamilyData() const
{
	for (auto& QueueFamily : *GetQueueFamilyData())
	{
		if (QueueFamily.IsPresentationSuitable)
		{
			return &QueueFamily;
		}
	}

	return nullptr;
}

void QueueFamilyHandler::RetrieveDeviceQueueHandles(const VkDevice* LogicalDevice)
{
	for (QueueFamilyData& FamilyData : mCachedQueueFamilyData)
	{
		vkGetDeviceQueue(*LogicalDevice, FamilyData.FamilyIndex, 0, &FamilyData.QueueHandle);
	}
}

// ~Queue Family Handler

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

	CreateLogicalDevice(*CreationInfo->pSurfaceHandle, mDeviceProperties[0]);
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
    for(int i = int(Devices.size()) - 1; i >= 0; i--)
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
	std::vector<QueueFamilyData> Indices = RetrieveQueueFamiliesData(Surface, Device);
	bool AreExtensionsSupported = CheckDeviceExtensionSupport(Device, mDesiredDeviceExtensions);
	bool SwapChainAdequate = aSwapChainHandler->IsAdequate(Device, Surface);

    return Rating != 0 && !Indices.empty() && AreExtensionsSupported && SwapChainAdequate;
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
    mDeviceProperties.clear();
    mDeviceProperties.resize(Devices.size());

    for(size_t i = 0; i < mDeviceProperties.size(); i++)
    {
        mDeviceProperties[i].DeviceHandle = Devices[i];
        mDeviceProperties[i].Rating = GetDeviceSuitabilityRating(Devices[i]);
    }

	std::sort(mDeviceProperties.begin(), mDeviceProperties.end(), [](const PhysicalDeviceProperties& LHS, const PhysicalDeviceProperties& RHS)
	{
		return LHS.Rating < RHS.Rating;
	});
}

const std::vector<QueueFamilyData>  DeviceHandler::RetrieveQueueFamiliesData(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const
{
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;

	uint32_t QueueFamilyCount = 0;

	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, nullptr);
	if (QueueFamilyCount == 0)
	{
		LogVk(LogType::Error, 0, "No Queue Family properties for physical devices were found.");
		return std::vector<QueueFamilyData>();
	}

	QueueFamilyProperties.resize(QueueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(Device, &QueueFamilyCount, QueueFamilyProperties.data());

	return FilterSuitableQueueFamilies(Surface, QueueFamilyProperties, Device);
}

std::vector<QueueFamilyData> DeviceHandler::FilterSuitableQueueFamilies(const VkSurfaceKHR& Surface, std::vector<VkQueueFamilyProperties>& QueueFamiliesProperties, const VkPhysicalDevice& Device) const
{
	if (QueueFamiliesProperties.size() == 0)
	{
		LogVk(LogType::Error, 0, "No queue families properties to filter.");
		return 	std::vector<QueueFamilyData>();
	}

	std::vector<QueueFamilyData> QueueFamilyIndices;

	int i = 0;
	for (const auto& QueueFamily : QueueFamiliesProperties)
	{
		if (QueueFamily.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))
		{
			QueueFamilyData NewProperties = {};
			NewProperties.FamilyIndex = i;
			NewProperties.IsPresentationSuitable = IsPresentationQueueFamilySuitable(QueueFamily, Device, Surface, i);;
			QueueFamilyIndices.push_back(NewProperties);
		}

		i++;
	}

	return QueueFamilyIndices;
}

bool DeviceHandler::IsGraphicsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties) const
{
	return QueueFamilyProperties.queueCount > 0 && QueueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
}

// Presentation for queue family needs to be checked before creating swapchain, otherwise Vulkan will complain.
bool DeviceHandler::IsPresentationQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties, const VkPhysicalDevice& Device, const VkSurfaceKHR& SurfaceHandle, int Index) const
{
	VkBool32 SurfaceSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(Device, Index, SurfaceHandle, &SurfaceSupport);
	return QueueFamilyProperties.queueCount > 0 && SurfaceSupport;
}

void DeviceHandler::CreateLogicalDevice(const VkSurfaceKHR& Surface, const PhysicalDeviceProperties& PhysicalDevice)
{
	std::vector<QueueFamilyData> Families = RetrieveQueueFamiliesData(Surface, PhysicalDevice.DeviceHandle);

	std::vector<VkDeviceQueueCreateInfo> QueueCreateInfos;

	for (const auto& Family : Families)
	{
		VkDeviceQueueCreateInfo QueueCreateInfo = {};
		QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueCreateInfo.queueFamilyIndex = Family.FamilyIndex;
		QueueCreateInfo.queueCount = 1;
		QueueCreateInfo.pQueuePriorities = &Family.Priority;
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

	mQueueFamilyHandler.ResetQueueFamilyData(Families);
	mQueueFamilyHandler.RetrieveDeviceQueueHandles(&LogicalDevice);
}

const std::vector<PhysicalDeviceProperties>* DeviceHandler::GetPhysicalDevicesProperties() const
{
	return &mDeviceProperties;
}

const PhysicalDeviceProperties* DeviceHandler::GetStrongestPhysicalDeviceProperties() const
{
	return &mDeviceProperties[0];
}

const VkDevice * DeviceHandler::GetLogicalDeviceHandle() const
{
	return &LogicalDevice;
}

void DeviceHandler::Destroy()
{
	vkDestroyDevice(LogicalDevice, nullptr);
}

const QueueFamilyHandler* DeviceHandler::GetQueueFamilyHandler() const
{
	return &mQueueFamilyHandler;
}