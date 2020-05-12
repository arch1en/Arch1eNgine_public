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
	AllocateInfo.memoryTypeIndex = MemoryManagementMethods::FindMemoryType(*PhysicalDevice, MemoryRequirements.memoryTypeBits, MemoryPropertyFlagBits); //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

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
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,		// Buffer can be used as the source of a transfer command.
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |	// Memory allocated with this type can be mapped for host access using vkMapMemory.
		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT	// The host cache management commands vkFlushMappedMemoryRanges and vkInvalidateMappedMemoryRanges are not needed to flush host writes to the device or make device writes visible to the host, respectively.
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

VkCommandPool& BufferFactory::GetMemoryOperationsCommandPool()
{
	return mMemoryOperationsCommandPool;
}

uint32_t MemoryManagementMethods::FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties)
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

VkCommandBuffer MemoryManagementMethods::BeginSingleTimeCommand(const VkDevice* LogicalDevice, VkCommandPool mCommandPool)
{
	VkCommandBufferAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandPool = mCommandPool;
	AllocInfo.commandBufferCount = 1;

	VkCommandBuffer CommandBuffer;
	vkAllocateCommandBuffers(*LogicalDevice, &AllocInfo, &CommandBuffer);

	VkCommandBufferBeginInfo BeginInfo = {};
	BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	BeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(CommandBuffer, &BeginInfo);

	return CommandBuffer;
}

void MemoryManagementMethods::EndSingleTimeCommand(VkCommandBuffer Buffer, const VkDevice* LogicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue)
{
	vkEndCommandBuffer(Buffer);

	VkSubmitInfo SubmitInfo = {};

	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &Buffer;

	vkQueueSubmit(GraphicsQueue, 1, &SubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(GraphicsQueue);

	vkFreeCommandBuffers(*LogicalDevice, CommandPool, 1, &Buffer);
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

	for (auto i : mImageDataArray)
	{
		DestroyImage(mLogicalDevice, i);
	}

	mImageDataArray.erase(mImageDataArray.begin(), mImageDataArray.end());

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

	// UV.
	VkVertexInputAttributeDescription TextureCoordAttributeDescription = {};

	TextureCoordAttributeDescription.binding = 0;
	TextureCoordAttributeDescription.location = 2;
	TextureCoordAttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
	TextureCoordAttributeDescription.offset = offsetof(Vertex, TexCoord);

	AttributeDescriptions.push_back(TextureCoordAttributeDescription);

	return AttributeDescriptions;
}

BufferFactory* const MemoryManager::GetBufferFactory() const
{
	return mBufferFactory.get();
}

void BufferFactory::CopyBuffer(const VkDevice* LogicalDevice, const QueueFamilyHandler* QueueFamilyHandler, VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size)
{
	const VkQueue& GraphicsQueue = QueueFamilyHandler->GetPresentationSuitableQueueFamilyData()->QueueHandle;

	VkCommandBuffer CommandBuffer = MemoryManagementMethods::BeginSingleTimeCommand(LogicalDevice, mMemoryOperationsCommandPool);

		VkBufferCopy CopyRegion = {};
		CopyRegion.srcOffset = 0; // Optional
		CopyRegion.dstOffset = 0; // Optional
		CopyRegion.size = Size;

		vkCmdCopyBuffer(CommandBuffer, SrcBuffer, DstBuffer, 1, &CopyRegion);

	MemoryManagementMethods::EndSingleTimeCommand(CommandBuffer, LogicalDevice, mMemoryOperationsCommandPool, GraphicsQueue);
}

void MemoryManager::CreateTextureImage(
	const VkDevice* LogicalDevice,
	const VkPhysicalDevice* PhysicalDevice,
	const VkQueue* GraphicsQueue,
	const unsigned char* Pixels,
	const int TexWidth,
	const int TexHeight,
	VkFormat ImageFormat,
	const std::string& TextureID
	)
{
	// [ToDo] Looks suspicious, will require refactoring.
	if (Pixels == nullptr)
	{
		LogVk(LogType::Error, 0, "No pixels passed. Texture image cannot be created!");
		return;
	}
	
	// [ToDo] Why * 4 ?
	VkDeviceSize ImageSize = TexWidth * TexHeight * 4;

	ImageData ImgData;

	ImgData.mTextureID = TextureID;

	GeneralBufferCreationInfo BufferCI = {};

	BufferCI.mBufferCreationInfo.mLogicalDevice = LogicalDevice;
	BufferCI.mBufferCreationInfo.mPhysicalDevice = PhysicalDevice;
	BufferCI.mBufferCreationInfo.mDataSize = ImageSize;

	std::unique_ptr<BufferData> Data = GetBufferFactory()->CreateGeneralBuffer(BufferCI);

	MemoryManagementMethods::MapMemory(*LogicalDevice, Pixels, Data->mBuffer, static_cast<size_t>(ImageSize), Data->mBufferMemory);

	ImageCreationInfo ImageCI = {};
	ImageCI.mLogicalDevice = LogicalDevice;
	ImageCI.mPhysicalDevice = PhysicalDevice;
	ImageCI.mWidth = TexWidth;
	ImageCI.mHeight = TexHeight;
	ImageCI.mFormat = VK_FORMAT_R8G8B8A8_SRGB;
	ImageCI.mTiling = VK_IMAGE_TILING_OPTIMAL;
	ImageCI.mUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	ImageCI.mProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	ImageCI.mImageData = &ImgData;

	CreateImage(ImageCI);

	ImageTransitionInfo ImageUndefinedToDstInfo = {};

	ImageUndefinedToDstInfo.mLogicalDevice = LogicalDevice;
	ImageUndefinedToDstInfo.mSingleTimeCommandsPool = &GetBufferFactory()->GetMemoryOperationsCommandPool();
	ImageUndefinedToDstInfo.mGraphicsQueue = GraphicsQueue;
	ImageUndefinedToDstInfo.mImage = ImgData.mTextureImage;
	ImageUndefinedToDstInfo.mFormat = VK_FORMAT_R8G8B8A8_SRGB;
	ImageUndefinedToDstInfo.mOldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ImageUndefinedToDstInfo.mNewLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

	TransitionImageLayout(ImageUndefinedToDstInfo);

	//CopyBufferToImage
	CopyBufferToImageInfo CbtiInfo;

	CbtiInfo.mLogicalDevice = LogicalDevice;
	CbtiInfo.mSingleTimeCommandsPool = &GetBufferFactory()->GetMemoryOperationsCommandPool();
	CbtiInfo.mGraphicsQueue = GraphicsQueue;
	CbtiInfo.mBuffer = Data->mBuffer;
	CbtiInfo.mImage = ImgData.mTextureImage;
	CbtiInfo.mWidth = static_cast<uint32_t>(TexWidth);
	CbtiInfo.mHeight = static_cast<uint32_t>(TexHeight);


	CopyBufferToImage(CbtiInfo);
	//~CopyBufferToImage

	ImageTransitionInfo ImageDstToShaderInfo = {};

	ImageDstToShaderInfo.mLogicalDevice = LogicalDevice;
	ImageDstToShaderInfo.mSingleTimeCommandsPool = &GetBufferFactory()->GetMemoryOperationsCommandPool();
	ImageDstToShaderInfo.mGraphicsQueue = GraphicsQueue;
	ImageDstToShaderInfo.mImage = ImgData.mTextureImage;
	ImageDstToShaderInfo.mFormat = VK_FORMAT_R8G8B8A8_SRGB;
	ImageDstToShaderInfo.mOldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	ImageDstToShaderInfo.mNewLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	TransitionImageLayout(ImageDstToShaderInfo);

	vkDestroyBuffer(*LogicalDevice, Data->mBuffer, nullptr);
	vkFreeMemory(*LogicalDevice, Data->mBufferMemory, nullptr);

	ImgData.mTextureImageView = CreateImageView(LogicalDevice, ImgData.mTextureImage, VK_FORMAT_R8G8B8A8_SRGB);
	ImgData.mTextureSampler = CreateTextureSampler(LogicalDevice);

	mImageDataArray.push_back(ImgData);
}

VkImageView MemoryManager::CreateImageView(const VkDevice* LogicalDevice, VkImage Image, VkFormat Format)
{
	VkImageViewCreateInfo ViewInfo = {};
	ViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ViewInfo.image = Image;
	ViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ViewInfo.format = Format;

	// Put here implicitly for educational purpose. VK_COMPONENT_SWIZZLE_IDENTITY is a default value for these four.
	ViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	ViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	ViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	ViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	ViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	ViewInfo.subresourceRange.baseMipLevel = 0;
	ViewInfo.subresourceRange.levelCount = 1;
	ViewInfo.subresourceRange.baseArrayLayer = 0;
	ViewInfo.subresourceRange.layerCount = 1;

	VkImageView ImageView;
	if (vkCreateImageView(*LogicalDevice, &ViewInfo, nullptr, &ImageView) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Failed to create image view!");
	}

	return ImageView;
}

void MemoryManager::DestroyImage(VkDevice LogicalDevice, ImageData Data)
{
	vkDestroySampler(LogicalDevice, Data.mTextureSampler, nullptr);
	vkDestroyImageView(LogicalDevice, Data.mTextureImageView, nullptr);
	vkDestroyImage(LogicalDevice, Data.mTextureImage, nullptr);
	vkFreeMemory(LogicalDevice, Data.mTextureImageMemory, nullptr);
}

const std::list<ImageData>* MemoryManager::GetImageDataArray() const
{
	return &mImageDataArray;

}

const ImageData* MemoryManager::GetImageDataByID(std::string ID) const
{
	for (const auto& i : mImageDataArray)
	{
		if (i.mTextureID.compare(ID) == 0)
		{
			return &i;
		}
	}

	return nullptr;
}

VkSampler MemoryManager::CreateTextureSampler(const VkDevice* LogicalDevice)
{
	VkSamplerCreateInfo SamplerInfo = {};
	SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.magFilter = VK_FILTER_LINEAR;
	SamplerInfo.minFilter = VK_FILTER_LINEAR;
	SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.anisotropyEnable = VK_TRUE;
	SamplerInfo.maxAnisotropy = 16;
	SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	SamplerInfo.unnormalizedCoordinates = VK_FALSE;
	SamplerInfo.compareEnable = VK_FALSE;
	SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	VkSampler NewSampler;

	if (vkCreateSampler(*LogicalDevice, &SamplerInfo, nullptr, &NewSampler) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Failed to create texture sampler!");
	}

	return NewSampler;
}

void MemoryManager::CreateImage(const ImageCreationInfo& CreationInfo)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = CreationInfo.mWidth;
	imageInfo.extent.height = CreationInfo.mHeight;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = CreationInfo.mFormat;
	imageInfo.tiling = CreationInfo.mTiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = CreationInfo.mUsage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(*CreationInfo.mLogicalDevice, &imageInfo, nullptr, &CreationInfo.mImageData->mTextureImage) != VK_SUCCESS) 
	{
		LogVk(LogType::Error, 0, "Failed to create image memory!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(*CreationInfo.mLogicalDevice, CreationInfo.mImageData->mTextureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = MemoryManagementMethods::FindMemoryType(*CreationInfo.mPhysicalDevice, memRequirements.memoryTypeBits, CreationInfo.mProperties);

	if (vkAllocateMemory(*CreationInfo.mLogicalDevice, &allocInfo, nullptr, &CreationInfo.mImageData->mTextureImageMemory) != VK_SUCCESS) 
	{
		LogVk(LogType::Error, 0, "Failed to allocate image memory!");
	}

	vkBindImageMemory(*CreationInfo.mLogicalDevice, CreationInfo.mImageData->mTextureImage, CreationInfo.mImageData->mTextureImageMemory, 0);
}

void MemoryManager::TransitionImageLayout(const ImageTransitionInfo& TransitionInfo) {
	VkCommandBuffer commandBuffer = MemoryManagementMethods::BeginSingleTimeCommand(TransitionInfo.mLogicalDevice, *TransitionInfo.mSingleTimeCommandsPool);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = TransitionInfo.mOldLayout;
	barrier.newLayout = TransitionInfo.mNewLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = TransitionInfo.mImage;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (TransitionInfo.mOldLayout == VK_IMAGE_LAYOUT_UNDEFINED && TransitionInfo.mNewLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (TransitionInfo.mOldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && TransitionInfo.mNewLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	// The pipeline barrier specifies an execution dependency,
	// such that all work performed by the set of pipeline stages included in srcStageMask of the first set of commands,
	// completes before any work performed by the set of pipeline stages included in dstStageMask of the second set of commands begins.
	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage,		// Pipeline stage, where operations will be executed.
		destinationStage,	// Pipeline stage, where 
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	MemoryManagementMethods::EndSingleTimeCommand(commandBuffer, TransitionInfo.mLogicalDevice, *TransitionInfo.mSingleTimeCommandsPool, *TransitionInfo.mGraphicsQueue);
}

void MemoryManager::CopyBufferToImage(const CopyBufferToImageInfo& Info) {
	VkCommandBuffer commandBuffer = MemoryManagementMethods::BeginSingleTimeCommand(Info.mLogicalDevice, *Info.mSingleTimeCommandsPool);

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = {
		Info.mWidth,
		Info.mHeight,
		1
	};

	vkCmdCopyBufferToImage(commandBuffer, Info.mBuffer, Info.mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	MemoryManagementMethods::EndSingleTimeCommand(commandBuffer, Info.mLogicalDevice, *Info.mSingleTimeCommandsPool, *Info.mGraphicsQueue);
}
