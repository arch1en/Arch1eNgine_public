#ifndef RENDERPASSMANAGER_H
#define RENDERPASSMANAGER_H

#include <vector>
#include <vulkan/vulkan.h>

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
	size_t mBufferSize;
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
	
	void CleanUp(const VkDevice& Device);
private:

	VkRenderPass mRenderPass;
	std::vector<VkFramebuffer> mFramebuffers;
	std::vector<VkCommandBuffer> mRenderPassCommandBuffers;

};

#endif