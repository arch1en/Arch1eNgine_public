#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <vulkan/vulkan.h>

using RenderPassID = std::string;

struct VertexBufferData;
struct IndexBufferData;

struct RenderPassCreateInfo
{
	RenderPassID mRenderPassID = "";
	const VkDevice* mLogicalDevice = nullptr;
	VkAttachmentDescription mAttachmentDescriptrion = {};
	VkAttachmentReference mAttachmentReference = {};
	VkSubpassDescription mSubpassDescription = {};
	VkSubpassDependency mSubpassDependency = {};
	VkRenderPassCreateInfo mRenderPassCreateInfo = {};
};

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

	void CreateRenderPass(const RenderPassCreateInfo& CreateInfo);
	void CreateFramebuffers(const FramebufferCreateInfo& CreateInfo);
	const VkRenderPass* const GetMainRenderPassHandle();
	const VkRenderPass* const GetRenderPassHandle(RenderPassID ID);
	const std::vector<VkCommandBuffer>* GetRenderPassCommandBuffers() const;

	const std::vector<VkFramebuffer>* GetFramebuffers() const;

	void CreateRenderPassCommandBuffers(const RenderPassCommandBufferCreateInfo& CreateInfo);
	
	void CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool);
	void Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool);
private:

	std::map<RenderPassID, VkRenderPass> mRenderPasses;
	std::vector<VkFramebuffer> mFramebuffers;
	std::vector<VkCommandBuffer> mRenderPassCommandBuffers;

};

#endif