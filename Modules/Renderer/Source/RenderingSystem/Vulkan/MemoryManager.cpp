#include "MemoryManager.h"

#include "LogSystem.h"

void MemoryManager::CreateBuffer(const BufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices)
{
	std::unique_ptr<BufferData> NewBufferData(new BufferData);

	NewBufferData->mBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	NewBufferData->mBufferCreateInfo.pNext = nullptr;
	NewBufferData->mBufferCreateInfo.size = sizeof(Vertices[0]) * Vertices.size();
	NewBufferData->mBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	NewBufferData->mBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	NewBufferData->mVertices = Vertices;

	if (vkCreateBuffer(*CreationInfo.mLogicalDevice, &NewBufferData->mBufferCreateInfo, nullptr, &NewBufferData->mBuffer) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Failed to create vertex buffer!");
	}

	AllocateMemory(*CreationInfo.mLogicalDevice, *CreationInfo.mPhysicalDevice, NewBufferData->mBuffer, NewBufferData->mBufferMemory);
	MapMemory(*CreationInfo.mLogicalDevice, Vertices.data(), NewBufferData->mBuffer, NewBufferData->mBufferCreateInfo, NewBufferData->mBufferMemory);

	mBufferData.push_back(std::move(NewBufferData));
}

void MemoryManager::AllocateMemory(const VkDevice& LogicalDevice, const VkPhysicalDevice& PhysicalDevice, const VkBuffer& Buffer, VkDeviceMemory& BufferMemory)
{
	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(LogicalDevice, Buffer, &MemoryRequirements);

	VkMemoryAllocateInfo AllocateInfo = {};
	AllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	AllocateInfo.allocationSize = MemoryRequirements.size;
	AllocateInfo.memoryTypeIndex = FindMemoryType(PhysicalDevice, MemoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(LogicalDevice, &AllocateInfo, nullptr, &BufferMemory) == VK_SUCCESS)
	{
		// 0 is the offset within the region of memory.
		// Since this memory is allocated specifically for this the vertex buffer, the offset is simply 0. 
		// If the offset is non-zero, then it is required to be divisible by memRequirements.alignment.
		vkBindBufferMemory(LogicalDevice, Buffer, BufferMemory, 0);
	}
	else
	{
		LogVk(LogType::Error, 0, "Buffer memory allocation failed!");
	}
}

void MemoryManager::MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkBufferCreateInfo& BufferCreateInfo, VkDeviceMemory& BufferMemory)
{
	void* Data = nullptr;

	vkMapMemory(LogicalDevice, BufferMemory, 0, BufferCreateInfo.size, 0, &Data);
		memcpy(Data, BufferData, size_t(BufferCreateInfo.size));
	vkUnmapMemory(LogicalDevice, BufferMemory);
}

uint32_t MemoryManager::FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties)
{
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &MemoryProperties);

	for (uint32_t i = 0; i < MemoryProperties.memoryTypeCount; i++)
	{
		if ((TypeFilter & (1 << i)) && (MemoryProperties.memoryTypes[i].propertyFlags & Properties) == Properties)
		{
			return i;
		}
	}

	LogVk(LogType::Error, 0, "Suitable memory type finding failure!");
}

void MemoryManager::Destroy(const VkDevice& mLogicalDevice)
{
	for (int i = int(mBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mBufferData[i]->mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mBufferData[i]->mBufferMemory, nullptr);
	}

	mBufferData.erase(mBufferData.begin(), mBufferData.end());
}

const std::vector<std::unique_ptr<BufferData>>* const MemoryManager::GetBufferData() const
{
	return &mBufferData;
}

template<>
const std::vector<VkVertexInputAttributeDescription> MemoryManager::GetAttributeDescription<Vertex>() const
{
	std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

	// Position.
	VkVertexInputAttributeDescription PositionAttributeDescription = {};

	PositionAttributeDescription.binding = 0;
	PositionAttributeDescription.location = 0;
	PositionAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	PositionAttributeDescription.offset = offsetof(Vertex, Position);

	AttributeDescriptions.push_back(PositionAttributeDescription);

	// Color.
	VkVertexInputAttributeDescription ColorAttributeDescription = {};

	ColorAttributeDescription.binding = 0;
	ColorAttributeDescription.location = 1;
	ColorAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
	ColorAttributeDescription.offset = offsetof(Vertex, Color);

	AttributeDescriptions.push_back(ColorAttributeDescription);

	return AttributeDescriptions;
}
