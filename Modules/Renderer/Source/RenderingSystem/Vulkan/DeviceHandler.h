// Handling Device related things.
// LogicalDevice, QueueFamilies, Queues.
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

struct QueueFamilyData
{
	VkQueue QueueHandle;
	uint32_t FamilyIndex = -1;
	float Priority = 1.f;
	bool IsPresentationSuitable = false;
};

class QueueFamilyHandler
{
public:
	void ResetQueueFamilyData(const std::vector<QueueFamilyData>& Data);
	void RetrieveDeviceQueueHandles(const VkDevice* LogicalDevice);

	const std::vector<uint32_t> GetQueueFamiliesIndices() const;
	const size_t GetNumberOfQueueFamilies() const;
	const std::vector<QueueFamilyData>* GetQueueFamilyData() const;
	const QueueFamilyData* GetPresentationSuitableQueueFamilyData() const;
private:

	std::vector<QueueFamilyData> mCachedQueueFamilyData;

};

class DeviceHandler
{
public:
	void Initiate(const DeviceHandlerCreationInfo* CreationInfo);

	const std::vector<PhysicalDeviceProperties>* GetPhysicalDevicesProperties() const;
	const VkDevice* GetLogicalDeviceHandle() const;
	const std::vector<QueueFamilyData> RetrieveQueueFamiliesData(const VkSurfaceKHR& Surface, const VkPhysicalDevice& Device) const;

	const QueueFamilyHandler* GetQueueFamilyHandler() const;

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
	std::vector<QueueFamilyData> FilterSuitableQueueFamilies(const VkSurfaceKHR& Surface, std::vector<VkQueueFamilyProperties>& QueueFamiliesProperties, const VkPhysicalDevice& Device) const;
	bool IsGraphicsQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties) const;
	bool IsPresentationQueueFamilySuitable(const VkQueueFamilyProperties& QueueFamilyProperties, const VkPhysicalDevice& Device, const VkSurfaceKHR& SurfaceHandle, int Index) const;
	// ~Queue Families

	// Logical Devices
	void CreateLogicalDevice(const VkSurfaceKHR& Surface, const PhysicalDeviceProperties& PhysicalDevice);
	// ~Logical Devices

	void Cleanup();
	QueueFamilyHandler mQueueFamilyHandler;
	VkDevice LogicalDevice;
    std::vector<PhysicalDeviceProperties> DeviceProperties;
	std::vector<const char*> mDesiredDeviceExtensions;
};

#endif
