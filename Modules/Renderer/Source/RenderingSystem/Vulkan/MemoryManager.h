#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>

#include "GeometrySystem/Vertex.h"
#include "VulkanUtilities.h"

class QueueFamilyHandler;

struct BufferCreationInfo
{
	const VkDevice* mLogicalDevice = nullptr;
	const VkPhysicalDevice* mPhysicalDevice = nullptr;
	VkDeviceSize mDataSize;
};

struct GeneralBufferCreationInfo
{
	BufferCreationInfo mBufferCreationInfo;
	const QueueFamilyHandler* mQueueFamilyHandler = nullptr;
};

class BufferFactory
{
public:

	void Initiate(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	std::unique_ptr<BufferData>			CreateGeneralBuffer(const GeneralBufferCreationInfo& CreationInfo);
	std::unique_ptr<VertexBufferData>	CreateVertexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);
	std::unique_ptr<IndexBufferData>	CreateIndexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Indices);
	std::unique_ptr<BufferData>			CreateUniformBuffer(const GeneralBufferCreationInfo& CreationInfo);

	void Destroy(const VkDevice& LogicalDevice);

private:

	VkCommandPool mMemoryOperationsCommandPool;

	uint32_t FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties);
	
	std::unique_ptr<BufferData>	 CreateBufferInternal
	(
		const VkDevice* LogicalDevice,
		const VkPhysicalDevice* PhysicalDevice,
		VkDeviceSize DataSize,
		VkBufferUsageFlags UsageFlags,
		VkMemoryPropertyFlags MemoryPropertyFlagBits
	);

	void CopyBuffer(const VkDevice* LogicalDevice, const QueueFamilyHandler* GraphicsQueue, VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size);

};

namespace MemoryManagementMethods
{
	void MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkDeviceSize& MemorySize, VkDeviceMemory& BufferMemory);
}

class MemoryManager
{
public:

	MemoryManager() = delete;
	MemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	// Describes at which rate to load data from memory throughout the vertices.
	template <class T>
	const VkVertexInputBindingDescription GetBindingDescription() const
	{
		VkVertexInputBindingDescription BindingDescription = {};

		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(T);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return BindingDescription;
	}

	// Describes how to handle vertex input.
	template <class T>
	const std::vector<VkVertexInputAttributeDescription> GetAttributeDescription() const;

	// @todo Names must be matched with actual buffers, they are now ambiguous.
	void CreateBuffer(const BufferCreationInfo& CreationInfo) {}
	void CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);
	void CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Indices);
	void CreateUniformBuffers(const GeneralBufferCreationInfo& CreationInfo, uint8_t SwapChainImagesNum);

	void UpdateUniformBuffer(const VkDevice* LogicalDevice, float DeltaTime, uint32_t ImageIndex, VkExtent2D ViewportExtent);

	const std::vector<std::unique_ptr<VertexBufferData>>* const GetVertexBufferData() const;
	const std::vector<std::unique_ptr<IndexBufferData>>* const GetIndexBufferData() const;
	const std::vector<std::unique_ptr<BufferData>>* const GetUniformBufferData() const;

	void CleanUp(const VkDevice& mLogicalDevice);
	void Destroy(const VkDevice& mLogicalDevice);

private:

	BufferFactory* const GetBufferFactory() const;
	std::unique_ptr<BufferFactory> mBufferFactory;

	std::vector<VkVertexInputBindingDescription> mBindingDescriptions;
	std::vector< VkVertexInputAttributeDescription> mAttributeDescriptions;

	// Specialized Buffers.
	std::vector<std::unique_ptr<VertexBufferData>> mVertexBufferData;
	std::vector<std::unique_ptr<IndexBufferData>> mIndexBufferData;
	std::vector<std::unique_ptr<BufferData>> mUniformBufferData;

};

#endif
