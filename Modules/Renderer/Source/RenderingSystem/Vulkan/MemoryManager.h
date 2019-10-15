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

struct VertexBufferCreationInfo
{
	BufferCreationInfo mBufferCreationInfo;
	const QueueFamilyHandler* mQueueFamilyHandler = nullptr;
};

class BufferFactory
{
public:

	std::unique_ptr<VertexBufferData>	CreateVertexBuffer(const VertexBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);

private:

	VkCommandPool mMemoryOperationsCommandPool;

	void MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkBufferCreateInfo& BufferCreateInfo, VkDeviceMemory& BufferMemory);
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

class MemoryManager
{
public:

	MemoryManager();

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

	void CreateBuffer(const BufferCreationInfo& CreationInfo);
	void CreateBuffer(const VertexBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertice);

	const std::vector<std::unique_ptr<BufferData>>* const GetBufferData() const;
	const std::vector<std::unique_ptr<VertexBufferData>>* const GetVertexBufferData() const;

	void Destroy(const VkDevice& mLogicalDevice);

private:

	BufferFactory* const GetBufferFactory() const;
	std::unique_ptr<BufferFactory> mBufferFactory;

	std::vector<VkVertexInputBindingDescription> mBindingDescriptions;
	std::vector< VkVertexInputAttributeDescription> mAttributeDescriptions;

	std::vector<std::unique_ptr<BufferData>> mBufferData;

	// Specialized Buffers.
	std::vector<std::unique_ptr<VertexBufferData>> mVertexBufferData;

};

#endif
