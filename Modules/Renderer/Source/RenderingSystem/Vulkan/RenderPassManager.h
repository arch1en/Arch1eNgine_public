#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.h>

#include "RenderingSystem/Vulkan/PipelineSystem.h"

using RenderPassID = std::string;

struct VertexBufferData;
struct IndexBufferData;

struct RenderPassData
{
	VkRenderPass mRenderPassHandle = VK_NULL_HANDLE;
	std::vector<VkFramebuffer> mFramebufferHandles;
	std::vector<VkCommandBuffer> mCommandBufferHandles;
};

struct RenderPassCreateInfo
{
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

	RenderPassManager();
	
	void CreateRenderPass(const RenderPassID& ID, const RenderPassCreateInfo& CreateInfo);
	void CreateFramebuffers(const RenderPassID& ID, const FramebufferCreateInfo& CreateInfo);
	void CreateRenderPassCommandBuffers(const RenderPassID& ID, const RenderPassCommandBufferCreateInfo& CreateInfo);

	const VkRenderPass* const GetMainRenderPassHandle();
	const RenderPassData* const GetRenderPassData(const RenderPassID ID);
	const std::map<RenderPassID, RenderPassData>& GetAllRenderPasses() const;
	const VkRenderPass* const GetRenderPassHandle(const RenderPassID ID);
	
	// Pipeline
	void CreatePipelineSystem();
	PipelineSystem* const	GetPipelineSystem() const;
	// ~Pipeline

	void CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool);
	void Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool);
private:

	std::unique_ptr<PipelineSystem> mPipelineSystem;

	std::map<RenderPassID, RenderPassData> mRenderPasses;

};

#endif