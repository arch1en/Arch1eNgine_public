#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <list>
#include <vector>
#include <vulkan/vulkan.h>

#include "GeometrySystem/Mesh.h"
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
	BufferCreationInfo mBufferCreationInfo = {};
	const QueueFamilyHandler* mQueueFamilyHandler = nullptr;
};

class BufferFactory
{
public:

	void Initiate(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	uptr<BufferData>		CreateGeneralBuffer(const GeneralBufferCreationInfo& CreationInfo);
	uptr<VertexBufferData>	CreateVertexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);
	uptr<IndexBufferData>	CreateIndexBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Indices);
	uptr<BufferData>		CreateUniformBuffer(const GeneralBufferCreationInfo& CreationInfo);

	void Destroy(const VkDevice& LogicalDevice);

	VkCommandPool& GetMemoryOperationsCommandPool();

private:

	// [Todo] This should remain private, or somehow moved.
	VkCommandPool mMemoryOperationsCommandPool;

	
	uptr<BufferData>	 CreateBufferInternal
	(
		const VkDevice* LogicalDevice,
		const VkPhysicalDevice* PhysicalDevice,
		VkDeviceSize DataSize,
		VkBufferUsageFlags UsageFlags,
		VkMemoryPropertyFlags MemoryPropertyFlagBits
	);

	void CopyBuffer(const VkDevice* LogicalDevice, const QueueFamilyHandler* GraphicsQueue, VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size);

};

// Image Structs
struct ImageData
{
	std::string mTextureID = "";
	VkImage mTextureImage;
	VkDeviceMemory mTextureImageMemory;
	VkImageView mTextureImageView;
	VkSampler mTextureSampler;
};

struct ImageCreationInfo
{
	const VkDevice* mLogicalDevice;
	const VkPhysicalDevice* mPhysicalDevice;
	uint32_t mWidth;
	uint32_t mHeight;
	VkFormat mFormat;
	VkImageTiling mTiling;
	VkImageUsageFlags mUsage;
	VkMemoryPropertyFlags mProperties;
	ImageData* mImageData;
};

struct ImageTransitionInfo
{
	const VkCommandPool* mSingleTimeCommandsPool = nullptr;
	const VkDevice* mLogicalDevice;
	const VkQueue* mGraphicsQueue;
	VkImage mImage;
	VkFormat mFormat;
	VkImageLayout mOldLayout;
	VkImageLayout mNewLayout;
};

struct CopyBufferToImageInfo
{
	const VkDevice* mLogicalDevice;
	const VkCommandPool* mSingleTimeCommandsPool = nullptr;
	const VkQueue* mGraphicsQueue;
	VkBuffer mBuffer;
	VkImage mImage;
	uint32_t mWidth;
	uint32_t mHeight;
};
// ~Image Structs

// Mesh Structs
struct MeshMemoryData
{
	uptr<VertexBufferData> VertexBuffer;
	uptr<IndexBufferData> IndexBuffer;
};
// ~Mesh Structs
namespace MemoryManagementMethods
{
	void MapMemory(const VkDevice& LogicalDevice, const void* BufferData, const VkBuffer& Buffer, const VkDeviceSize& MemorySize, VkDeviceMemory& BufferMemory);
	uint32_t FindMemoryType(const VkPhysicalDevice& PhysicalDevice, uint32_t TypeFilter, VkMemoryPropertyFlags Properties);
	VkCommandBuffer BeginSingleTimeCommand(const VkDevice* LogicalDevice, VkCommandPool mCommandPool);
	void EndSingleTimeCommand(VkCommandBuffer Buffer, const VkDevice* LogicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue);
}

class MemoryManager
{
public:

	MemoryManager() = delete;
	MemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH);

	// Describes at which rate to load data from memory throughout the vertices.
	template <class T>
	static const VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};

		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(T);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return BindingDescription;
	}

	// Describes how to handle vertex input.
	template <class T>
	static const std::vector<VkVertexInputAttributeDescription> GetAttributeDescription()
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

	void PrepareMeshMemory(const string ID, const GeneralBufferCreationInfo& VertexBufferCreationInfo, const GeneralBufferCreationInfo& IndexBufferCreationInfo, const Mesh& MeshData);

	// @todo Names must be matched with actual buffers, they are now ambiguous.
	void CreateBuffer(const string BufferID, const BufferCreationInfo& CreationInfo) {}
	uptr<VertexBufferData> CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<Vertex>& Vertices);
	uptr<IndexBufferData> CreateBuffer(const GeneralBufferCreationInfo& CreationInfo, const std::vector<uint16_t>& Indices);
	void CreateUniformBuffers(const GeneralBufferCreationInfo& CreationInfo, uint8_t SwapChainImagesNum);

	void UpdateUniformBuffer(const VkDevice* LogicalDevice, float DeltaTime, uint32_t ImageIndex, VkExtent2D ViewportExtent);

	auto GetMeshMemoryData() const->const hashtable<string, uptr<MeshMemoryData>>* const;
	auto GetUniformBufferData() const->const vector<uptr<BufferData>>* const;

	auto GetMeshMemoryDataByID(string ID) const->const MeshMemoryData* const;

	void CleanUp(const VkDevice& mLogicalDevice);
	void Destroy(const VkDevice& mLogicalDevice);

	// Image
	// [Todo] General handles should be packed into read-only singleton object.
	void CreateTextureImage(const VkDevice* LogicalDevice, const VkPhysicalDevice* PhysicalDevice, const VkQueue* GraphicsQueue, const unsigned char* Pixels, const int TexWidth, const int TexHeight, VkFormat ImageFormat, const std::string& TextureID);
	VkImageView CreateImageView(const VkDevice* LogicalDevice, VkImage Image, VkFormat Format);
	void DestroyImage(VkDevice LogicalDevice, ImageData Data);
	const std::list<ImageData>* GetImageDataArray() const;
	const ImageData* GetImageDataByID(std::string ID) const;
private:
	VkSampler CreateTextureSampler(const VkDevice* LogicalDevice);
	void CreateImage(const ImageCreationInfo& CreationInfo);
	void TransitionImageLayout(const ImageTransitionInfo& TransitionInfo);
	void CopyBufferToImage(const CopyBufferToImageInfo& Info);
	// ~Image
private:

	BufferFactory* const GetBufferFactory() const;
	uptr<BufferFactory> mBufferFactory;

	std::vector<VkVertexInputBindingDescription> mBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription> mAttributeDescriptions;

	// Specialized Buffers.
	hashtable<string, uptr<MeshMemoryData>> mMeshMemoryData;

	vector<uptr<BufferData>> mUniformBufferData;

	std::list<ImageData> mImageDataArray;
};

#endif
