// Order of binding : 
// - Renderpass 
// - Pipeline
// - Descriptor sets
// - Vertex buffers 
// - Push constants
// Former should be changed less frequently than the latter.

#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.h>

#include "RenderingSystem/Vulkan/PipelineSystem.h"
#include "Delegate.h"

using RenderPassID = std::string;

class MemoryManager;

struct VertexBufferData;
struct IndexBufferData;
struct FrameData;

using RecordingRenderPassDelegate = Delegate<void
(
	uint32_t						SwapChainImageIndex,
	const VkRenderPass* const		RenderPassHandle,
	const FrameData&				FrameData,
	const MemoryManager* const		MemoryManagerHandle,
	const VkPipeline* const			PipelineHandle,
	const VkPipelineLayout* const	PipelineLayoutHandle,
	const VkDescriptorSet* const	DescriptorSetHandle,
	VkExtent2D						SwapChainExtent,
	VkClearValue					ClearValue
)>;

struct FrameData
{
	VkCommandPool mFrameCommandPool = VK_NULL_HANDLE;
	VkFramebuffer mFrameBufferHandle = VK_NULL_HANDLE;
	std::vector<VkCommandBuffer> mCommandBufferHandles;
	VkSemaphore mFinishedFrameRenderingSemaphoreHandle = VK_NULL_HANDLE;
};

struct RenderPassData
{
	VkRenderPass mRenderPassHandle = VK_NULL_HANDLE;
	std::vector<FrameData> mFrameData;
	RecordingRenderPassDelegate mRecordingRenderPassFunc;
};

struct RenderPassCommandBufferCreateInfo
{
	const VkDevice* mLogicalDevice;
	const VkCommandPool* mCommandPool;
	const VkPipeline* mPipelineHandle;
	const VkPipelineLayout* mPipelineLayout;
	const std::vector<VkDescriptorSet>* mDescriptorSets;
	size_t mBufferSize;
	VkExtent2D mSwapChainExtent;
	const VertexBufferData* mVertexBufferData;
	const IndexBufferData* mIndexBufferData;
};

class RenderPassManager
{
public:

	RenderPassManager() = delete;
	explicit RenderPassManager(const VkDevice* LogicalDevice);
	
	void CreateRenderPass
	(
		const RenderPassID& ID,
		const VkDevice* LogicalDevice,
		uint32_t NumCommandBuffersToMake,
		const std::vector<VkImageView>& SwapChainImageViews,
		const VkExtent2D& SwapChainImageExtent,
		uint32_t PresentationSuitableQueueFamilyIndex,
		const VkRenderPassCreateInfo& RenderPassCreateInfo,
		RecordingRenderPassDelegate&& RecordingRenderPassFunc
	);
	
	VkFramebuffer CreateFramebuffer
	(
		const VkDevice* LogicalDevice,
		const VkRenderPass* RenderPassHandle,
		const VkImageView& SwapChainImageView,
		const VkExtent2D& SwapChainImageExtent
	);

	const VkRenderPass* const GetMainRenderPassHandle();
	RenderPassData* const GetRenderPassData(const RenderPassID& ID);
	const std::map<RenderPassID, RenderPassData> GetAllRenderPasses() const;
	const VkRenderPass* const GetRenderPassHandle(const RenderPassID& ID);
	
	// Pipeline
	void CreatePipelineSystem(const VkDevice* LogicalDevice);
	PipelineSystem* const	GetPipelineSystem() const;
	// ~Pipeline

	void CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool);
	void Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool);
private:

	std::unique_ptr<PipelineSystem> mPipelineSystem;

	std::map<RenderPassID, RenderPassData> mRenderPasses;

};

#endif