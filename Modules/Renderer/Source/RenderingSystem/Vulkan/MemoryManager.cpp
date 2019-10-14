#include "MemoryManager.h"

#include "LogSystem.h"

std::unique_ptr<BufferData>	BufferFactory::CreateBufferInternal
(
	const VkDevice* LogicalDevice,
	const VkPhysicalDevice* PhysicalDevice,
	VkDeviceSize DataSize,
	VkBufferUsageFlags UsageFlags,
	VkMemoryPropertyFlags MemoryPropertyFlagBits
)
{
	BufferData* NewBufferData = new BufferData;

	NewBufferData->mBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	NewBufferData->mBufferCreateInfo.pNext = nullptr;
	NewBufferData->mBufferCreateInfo.size = DataSize;
	NewBufferData->mBufferCreateInfo.usage = UsageFlags;
	NewBufferData->mBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*LogicalDevice, &NewBufferData->mBufferCreateInfo, nullptr, &NewBufferData->mBuffer) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Failed to create vertex buffer!");
	}

	// Allocate memory

	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(*LogicalDevice, NewBufferData->mBuffer, &MemoryRequirements);

	VkMemoryAllocateInfo AllocateInfo = {};
	AllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	AllocateInfo.allocationSize = MemoryRequirements.size;
	AllocateInfo.memoryTypeIndex = FindMemoryType(*PhysicalDevice, MemoryRequirements.memoryTypeBits, MemoryPropertyFlagBits); //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(*LogicalDevice, &AllocateInfo, nullptr, &NewBufferData->mBufferMemory) == VK_SUCCESS)
	{
		// 0 is the offset within the region of memory.
		// Since this memory is allocated specifically for this the vertex buffer, the offset is simply 0. 
		// If the offset is non-zero, then it is required to be divisible by memRequirements.alignment.
		vkBindBufferMemory(*LogicalDevice, NewBufferData->mBuffer, NewBufferData->mBufferMemory, 0);
	}
	else
	{
		LogVk(LogType::Error, 0, "Buffer memory allocation failed!");
	}

	return std::unique_ptr<BufferData>(NewBufferData);
}

std::unique_ptr<VertexBufferData> BufferFactory::CreateVertexBuffer(const BufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices)
{
	std::unique_ptr<BufferData> NewBufferData = CreateBufferInternal
	(
		CreationInfo.mLogicalDevice,
		CreationInfo.mPhysicalDevice,
		CreationInfo.mDataSize,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	MapMemory(*CreationInfo.mLogicalDevice, Vertices.data(), NewBufferData->mBuffer, NewBufferData->mBufferCreateInfo, NewBufferData->mBufferMemory);

	VertexBufferData* NewVertexBufferData = new VertexBufferData;

	NewVertexBufferData->mBufferData = *NewBufferData;
	NewVertexBufferData->mVertices = Vertices;

	return std::unique_ptr<VertexBufferData>(NewVertexBufferData);
}

uint32_t BufferFactory::FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties)
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

void BufferFactory::MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkBufferCreateInfo& BufferCreateInfo, VkDeviceMemory& BufferMemory)
{
	void* Data = nullptr;

	vkMapMemory(LogicalDevice, BufferMemory, 0, BufferCreateInfo.size, 0, &Data);
		memcpy(Data, BufferData, size_t(BufferCreateInfo.size));
	vkUnmapMemory(LogicalDevice, BufferMemory);
}

// Memory Manager

MemoryManager::MemoryManager()
	: mBufferFactory{ new BufferFactory }
{}

void MemoryManager::CreateBuffer(const BufferCreationInfo& CreationInfo)
{
	//GetBufferFactory()->CreateBuffer(CreationInfo);

	
}

void MemoryManager::CreateBuffer(const BufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices)
{
	std::unique_ptr<VertexBufferData> Data = GetBufferFactory()->CreateVertexBuffer(CreationInfo, Vertices);
	mVertexBufferData.push_back(std::move(Data));
}

void MemoryManager::Destroy(const VkDevice& mLogicalDevice)
{
	for (int i = int(mBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mBufferData[i]->mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mBufferData[i]->mBufferMemory, nullptr);
	}

	mBufferData.erase(mBufferData.begin(), mBufferData.end());

	// [TODO] Merge above and below code.
	for (int i = int(mVertexBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mVertexBufferData[i]->mBufferData.mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mVertexBufferData[i]->mBufferData.mBufferMemory, nullptr);
	}

	mVertexBufferData.erase(mVertexBufferData.begin(), mVertexBufferData.end());
}

const std::vector<std::unique_ptr<BufferData>>* const MemoryManager::GetBufferData() const
{
	return &mBufferData;
}

const std::vector<std::unique_ptr<VertexBufferData>>* const MemoryManager::GetVertexBufferData() const
{
	return &mVertexBufferData;
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

BufferFactory* const MemoryManager::GetBufferFactory() const
{
	return mBufferFactory.get();
}
