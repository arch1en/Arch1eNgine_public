#ifndef PHYSICALDEVICEHANDLER_H
#define PHYSICALDEVICEHANDLER_H

#include <vector>
#include <optional>

#include <vulkan/vulkan.h>

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
	void Initiate(const VkInstance& InstanceHandle, const VkSurfaceKHR& Surface);

private:

    // Physical Devices
    bool RetrievePhysicalDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices);
    bool FilterSuitableDevices(const VkInstance& InstanceHandle, const VkSurfaceKHR& Surface, std::vector<VkPhysicalDevice>& Devices);
	bool IsDeviceSuitable(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const;
    void CacheDevices(std::vector<VkPhysicalDevice>& Devices);
	int GetDeviceSuitabilityRating(const VkPhysicalDevice& Device) const;
    // ~Physical Devices

	// Queue Families
	QueueFamilies RetrieveQueueFamilies(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const;
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

};

#endif
