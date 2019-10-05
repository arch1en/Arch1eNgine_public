#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>

#include "GeometrySystem/Vertex.h"
#include "VulkanUtilities.h"

struct BufferCreationInfo
{
	const VkDevice* mLogicalDevice = nullptr;
	const VkPhysicalDevice* mPhysicalDevice = nullptr;
};

class MemoryManager
{
public:

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

	void CreateBuffer(const BufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);

	const std::vector<std::unique_ptr<BufferData>>* const GetBufferData() const;

	void Destroy(const VkDevice& mLogicalDevice);

private:

	void AllocateMemory(const VkDevice& LogicalDevice, const VkPhysicalDevice& PhysicalDevice, const VkBuffer& VertexBuffer, VkDeviceMemory& BufferMemory);
	void MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkBufferCreateInfo& BufferCreateInfo, VkDeviceMemory& BufferMemory);
	uint32_t FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties);

	std::vector<VkVertexInputBindingDescription> mBindingDescriptions;
	std::vector< VkVertexInputAttributeDescription> mAttributeDescriptions;

	std::vector<std::unique_ptr<BufferData>> mBufferData;

};

#endif
