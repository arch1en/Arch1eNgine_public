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

struct QueueFamilyIndices
{
	QueueFamilyIndices()
		: QueuePriority{ 1.f }
	{}

	std::optional<uint32_t> GraphicsFamilyIndex;
	float QueuePriority;
	bool IsComplete()
	{
		return GraphicsFamilyIndex.has_value();
	}
};

class DeviceHandler
{
public:
    DeviceHandler(const VkInstance& InstanceHandle);

private:

    // Physical Devices
    bool RetrievePhysicalDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices);
    bool FilterSuitableDevices(const VkInstance& InstanceHandle, std::vector<VkPhysicalDevice>& Devices);
	bool IsDeviceSuitable(const VkPhysicalDevice& Device) const;
    void CacheDevices(std::vector<VkPhysicalDevice>& Devices);
	int GetDeviceSuitabilityRating(const VkPhysicalDevice& Device) const;
    // ~Physical Devices

	// Queue Families
	QueueFamilyIndices RetrieveQueueFamilies(const VkPhysicalDevice& Device) const;
	bool FilterSuitableQueueFamilies(std::vector<VkQueueFamilyProperties>& QueueFamilies, QueueFamilyIndices& Indices) const;
	bool IsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamily) const;
	// ~Queue Families

	// Logical Devices
	void CreateLogicalDevice(const PhysicalDeviceProperties& PhysicalDevice);
	// ~Logical Devices

	void Cleanup();

	VkDevice LogicalDevice;
    std::vector<PhysicalDeviceProperties> DeviceProperties;

};

#endif
