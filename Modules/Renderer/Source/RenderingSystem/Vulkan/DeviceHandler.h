#ifndef PHYSICALDEVICEHANDLER_H
#define PHYSICALDEVICEHANDLER_H

#include <vector>
#include <optional>

#include <vulkan/vulkan.h>

class SwapChainHandler;

struct DeviceHandlerCreationInfo
{
	const VkInstance* pInstanceHandle;
	const VkSurfaceKHR* pSurfaceHandle;
	const SwapChainHandler* pSwapChainHandler;
	std::vector<const char*> DesiredDeviceExtensions;
};

struct PhysicalDeviceProperties
{
    VkPhysicalDevice DeviceHandle;
    int Rating;
};

enum class QueueFamilyIndices
{
	GraphicsFamily,
	PresentationFamily,
	TOTAL
};

struct QueueFamilyProperties
{
	QueueFamilyProperties()
		: QueuePriority{ 1.f }
	{}

	std::optional<uint32_t> Index;
	float QueuePriority;
};

struct QueueFamilies
{
	QueueFamilies()
		: QueuePriority{ 1.f }
	{
		FamilyIndices.resize(static_cast<size_t>(QueueFamilyIndices::TOTAL));
	}

	std::vector<QueueFamilyProperties> FamilyIndices;

	float QueuePriority;
	bool IsComplete()
	{
		for (const auto& Index : FamilyIndices)
		{
			if (Index.Index.has_value() == false)
			{
				return false;
			}
		}

		return true;
	}
};

class DeviceHandler
{
public:
	void Initiate(const DeviceHandlerCreationInfo* CreationInfo);

	const std::vector<PhysicalDeviceProperties>* GetPhysicalDevicesProperties() const;
	const VkDevice* GetLogicalDeviceHandle() const;
	const QueueFamilies RetrieveQueueFamilies(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const;

private:

    // Physical Devices
    bool RetrievePhysicalDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices);
    bool FilterSuitableDevices(const VkInstance& InstanceHandle, const VkSurfaceKHR& Surface, const SwapChainHandler* aSwapChainHandler, std::vector<VkPhysicalDevice>& Devices);
	bool IsDeviceSuitable(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device, const SwapChainHandler* aSwapChainHandler) const;
	bool CheckDeviceExtensionSupport(const VkPhysicalDevice& aDevice, const std::vector<const char*> aDesiredDeviceExtensions) const;
    void CacheDevices(std::vector<VkPhysicalDevice>& Devices);
	int GetDeviceSuitabilityRating(const VkPhysicalDevice& Device) const;
	void SetDesiredDeviceExtensions(const std::vector<const char*> aDesiredDeviceExtensions);
    // ~Physical Devices

	// Queue Families
	bool FilterSuitableQueueFamilies(const VkSurfaceKHR& Surface, std::vector<VkQueueFamilyProperties>& QueueFamiliesProperties, const VkPhysicalDevice& Device, QueueFamilies& Indices) const;
	bool IsGraphicsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties) const;
	bool IsPresentationQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties, const VkPhysicalDevice& Device, const VkSurfaceKHR& SurfaceHandle, int Index) const;
	// ~Queue Families

	// Logical Devices
	void CreateLogicalDevice(const VkSurfaceKHR& Surface, const PhysicalDeviceProperties& PhysicalDevice);
	// ~Logical Devices

	void Cleanup();

	VkDevice LogicalDevice;
    std::vector<PhysicalDeviceProperties> DeviceProperties;
	std::vector<const char*> mDesiredDeviceExtensions;
};

#endif
