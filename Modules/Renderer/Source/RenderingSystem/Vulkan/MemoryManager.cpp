#include "MemoryManager.h"

#include "DeviceHandler.h"

#include <glm/gtx/transform.hpp>

#include "Debug/LogSystem.h"

void BufferFactory::Initiate(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH)
{
	VkCommandPoolCreateInfo CommandPoolCI = {};
	CommandPoolCI.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	CommandPoolCI.queueFamilyIndex = QFH->GetPresentationSuitableQueueFamilyData()->FamilyIndex;

	vkCreateCommandPool(LogicalDevice, &CommandPoolCI, nullptr, &mMemoryOperationsCommandPool);
}

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

	VkBufferCreateInfo BufferCI = {};

	BufferCI.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferCI.pNext = nullptr;
	BufferCI.size = DataSize;
	BufferCI.usage = UsageFlags;
	BufferCI.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*LogicalDevice, &BufferCI, nullptr, &NewBufferData->mBuffer) != VK_SUCCESS)
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

void BufferFactory::Destroy(const VkDevice& LogicalDevice)
{
	vkDestroyCommandPool(LogicalDevice, mMemoryOperationsCommandPool, nullptr);
}

std::unique_ptr<VertexBufferData> BufferFactory::CreateVertexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices)
{
	std::unique_ptr<BufferData> StagingBuffer = CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	MemoryManagementMethods::MapMemory(*CreationInfo.mBufferCreationInfo.mLogicalDevice, Vertices.data(), StagingBuffer->mBuffer, CreationInfo.mBufferCreationInfo.mDataSize, StagingBuffer->mBufferMemory);

	std::unique_ptr<BufferData> VertexBuffer = CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	CopyBuffer(CreationInfo.mBufferCreationInfo.mLogicalDevice, CreationInfo.mQueueFamilyHandler, StagingBuffer->mBuffer, VertexBuffer->mBuffer, CreationInfo.mBufferCreationInfo.mDataSize);

	vkDestroyBuffer(*CreationInfo.mBufferCreationInfo.mLogicalDevice, StagingBuffer->mBuffer, nullptr);
	vkFreeMemory(*CreationInfo.mBufferCreationInfo.mLogicalDevice, StagingBuffer->mBufferMemory, nullptr);

	VertexBufferData* NewVertexBufferData = new VertexBufferData;

	NewVertexBufferData->mBufferData = *VertexBuffer;
	NewVertexBufferData->mVertices = Vertices;

	return std::unique_ptr<VertexBufferData>(NewVertexBufferData);
}

std::unique_ptr<IndexBufferData> BufferFactory::CreateIndexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Vertices)
{
	std::unique_ptr<BufferData> StagingBuffer = CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	MemoryManagementMethods::MapMemory(*CreationInfo.mBufferCreationInfo.mLogicalDevice, Vertices.data(), StagingBuffer->mBuffer, CreationInfo.mBufferCreationInfo.mDataSize, StagingBuffer->mBufferMemory);

	std::unique_ptr<BufferData> IndexBuffer = CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);

	CopyBuffer(CreationInfo.mBufferCreationInfo.mLogicalDevice, CreationInfo.mQueueFamilyHandler, StagingBuffer->mBuffer, IndexBuffer->mBuffer, CreationInfo.mBufferCreationInfo.mDataSize);

	vkDestroyBuffer(*CreationInfo.mBufferCreationInfo.mLogicalDevice, StagingBuffer->mBuffer, nullptr);
	vkFreeMemory(*CreationInfo.mBufferCreationInfo.mLogicalDevice, StagingBuffer->mBufferMemory, nullptr);

	IndexBufferData* NewIndexBufferData = new IndexBufferData;

	NewIndexBufferData->mBufferData = *IndexBuffer;
	NewIndexBufferData->mIndices = Vertices;

	return std::unique_ptr<IndexBufferData>(NewIndexBufferData);
}

std::unique_ptr<BufferData>	BufferFactory::CreateGeneralBuffer(const GeneralBufferCreationInfo& CreationInfo)
{
	return CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

}

std::unique_ptr<BufferData>	BufferFactory::CreateUniformBuffer(const GeneralBufferCreationInfo& CreationInfo)
{
	return CreateBufferInternal
	(
		CreationInfo.mBufferCreationInfo.mLogicalDevice,
		CreationInfo.mBufferCreationInfo.mPhysicalDevice,
		CreationInfo.mBufferCreationInfo.mDataSize,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

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
	return 0;
}

void MemoryManagementMethods::MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkDeviceSize& MemorySize, VkDeviceMemory& BufferMemory)
{
	void* Data = nullptr;

	vkMapMemory(LogicalDevice, BufferMemory, 0, MemorySize, 0, &Data);
		memcpy(Data, BufferData, size_t(MemorySize));
	vkUnmapMemory(LogicalDevice, BufferMemory);
}

// Memory Manager

MemoryManager::MemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH)
	: mBufferFactory{ new BufferFactory }
{
	mBufferFactory->Initiate(LogicalDevice, QFH);
}

void MemoryManager::CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices)
{
	std::unique_ptr<VertexBufferData> Data = GetBufferFactory()->CreateVertexBuffer(CreationInfo, Vertices);
	mVertexBufferData.push_back(std::move(Data));
}

void MemoryManager::CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Indices)
{
	std::unique_ptr<IndexBufferData> Data = GetBufferFactory()->CreateIndexBuffer(CreationInfo, Indices);
	mIndexBufferData.push_back(std::move(Data));
}

void MemoryManager::CreateUniformBuffers(const GeneralBufferCreationInfo& CreationInfo, uint8_t SwapChainImagesNum)
{
	mUniformBufferData.resize(SwapChainImagesNum);

	for (int i = 0; i < SwapChainImagesNum; i++)
	{
		std::unique_ptr<BufferData> Data = GetBufferFactory()->CreateUniformBuffer(CreationInfo);
		mUniformBufferData[i] = std::move(Data);
	}

}

void MemoryManager::UpdateUniformBuffer(const VkDevice* LogicalDevice, float DeltaTime, uint32_t ImageIndex, VkExtent2D ViewportExtent)
{
	// @temp
	const float FOV = 45.f;

	UniformBufferObject UBO = {};
	UBO.Model = glm::rotate(glm::mat4(1.f), DeltaTime * glm::radians(90.f), glm::vec3(0.f, 0.f, 1.f));
	UBO.View = glm::lookAt(glm::vec3(2.f, 2.f, 2.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f));
	UBO.Projection = glm::perspective(glm::radians(FOV), ViewportExtent.width / float(ViewportExtent.height), 0.1f, 10.f);

	UBO.Projection[1][1] *= -1;

	MemoryManagementMethods::MapMemory(*LogicalDevice, &UBO, mUniformBufferData[ImageIndex]->mBuffer, sizeof(UBO), mUniformBufferData[ImageIndex]->mBufferMemory);
}

void MemoryManager::CleanUp(const VkDevice& mLogicalDevice)
{
	for (int i = int(mUniformBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mUniformBufferData[i]->mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mUniformBufferData[i]->mBufferMemory, nullptr);
	}

	mUniformBufferData.erase(mUniformBufferData.begin(), mUniformBufferData.end());
}

void MemoryManager::Destroy(const VkDevice& mLogicalDevice)
{
	CleanUp(mLogicalDevice);

	// [TODO] Merge above and below code.
	for (int i = int(mVertexBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mVertexBufferData[i]->mBufferData.mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mVertexBufferData[i]->mBufferData.mBufferMemory, nullptr);
	}

	mVertexBufferData.erase(mVertexBufferData.begin(), mVertexBufferData.end());

	for (int i = int(mIndexBufferData.size()) - 1; i >= 0; i--)
	{
		vkDestroyBuffer(mLogicalDevice, mIndexBufferData[i]->mBufferData.mBuffer, nullptr);
		vkFreeMemory(mLogicalDevice, mIndexBufferData[i]->mBufferData.mBufferMemory, nullptr);
	}

	mIndexBufferData.erase(mIndexBufferData.begin(), mIndexBufferData.end());

	GetBufferFactory()->Destroy(mLogicalDevice);
}

const std::vector<std::unique_ptr<VertexBufferData>>* const MemoryManager::GetVertexBufferData() const
{
	return &mVertexBufferData;
}

const std::vector<std::unique_ptr<IndexBufferData>>* const MemoryManager::GetIndexBufferData() const
{
	return &mIndexBufferData;
}

const std::vector<std::unique_ptr<BufferData>>* const MemoryManager::GetUniformBufferData() const
{
	return &mUniformBufferData;
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

void BufferFactory::CopyBuffer(const VkDevice* LogicalDevice, const QueueFamilyHandler* QueueFamilyHandler, VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size)
{
	const VkQueue& GraphicsQueue = QueueFamilyHandler->GetPresentationSuitableQueueFamilyData()->QueueHandle;

	VkCommandBufferAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandPool = mMemoryOperationsCommandPool;
	AllocInfo.commandBufferCount = 1;

	VkCommandBuffer CommandBuffer;
	vkAllocateCommandBuffers(*LogicalDevice, &AllocInfo, &CommandBuffer);

	VkCommandBufferBeginInfo BeginInfo = {};
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(CommandBuffer, &BeginInfo);

		VkBufferCopy CopyRegion = {};
		CopyRegion.srcOffset = 0; // Optional
		CopyRegion.dstOffset = 0; // Optional
		CopyRegion.size = Size;

		vkCmdCopyBuffer(CommandBuffer, SrcBuffer, DstBuffer, 1, &CopyRegion);

	vkEndCommandBuffer(CommandBuffer);

	VkSubmitInfo SubmitInfo = {};

	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &CommandBuffer;

	vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(GraphicsQueue);

	vkFreeCommandBuffers(*LogicalDevice, mMemoryOperationsCommandPool, 1, &CommandBuffer);
}
