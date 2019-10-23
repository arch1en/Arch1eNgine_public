#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>

struct VertexBufferData;
struct IndexBufferData;

struct FramebufferCreateInfo
{
	const VkDevice* mLogicalDevice;
	const VkRenderPass* mRenderPassHandle;
	const std::vector<VkImageView>* mSwapChainImageViews;
	const VkExtent2D* mSwapChainImageExtent;
};

struct RenderPassCommandBufferCreateInfo
{
	const VkDevice* mLogicalDevice;
	const VkCommandPool* mCommandPool;
	const VkPipeline* mPipelineHandle;
	size_t mBufferSize;
	VkExtent2D mSwapChainExtent;
	const VertexBufferData* mVertexBufferData;
	const IndexBufferData* mIndexBufferData;
};

class RenderPassManager
{
public:

	void CreateRenderPass(const VkDevice& Device, const VkFormat& ImageFormat);
	void CreateFramebuffers(const FramebufferCreateInfo& CreateInfo);
	const VkRenderPass* const GetRenderPassHandle() const;
	const std::vector<VkCommandBuffer>* GetRenderPassCommandBuffers() const;

	const std::vector<VkFramebuffer>* GetFramebuffers() const;

	void CreateRenderPassCommandBuffers(const RenderPassCommandBufferCreateInfo& CreateInfo);
	
	void CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool);
	void Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool);
private:

	VkRenderPass mRenderPass;
	std::vector<VkFramebuffer> mFramebuffers;
	std::vector<VkCommandBuffer> mRenderPassCommandBuffers;

};

#endif