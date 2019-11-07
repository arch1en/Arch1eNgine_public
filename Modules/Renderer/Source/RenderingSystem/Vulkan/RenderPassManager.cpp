#include "RenderPassManager.h"
#include "LogSystem.h"
#include "VulkanUtilities.h"

void RenderPassManager::CreateRenderPass(const VkDevice& Device, const VkFormat& ImageFormat)
{
	VkAttachmentDescription ColorAttachment = {};
	ColorAttachment.format = ImageFormat;
	ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference ColorAttachmentRef = {};
	ColorAttachmentRef.attachment = 0;
	ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription Subpass = {};
	Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	Subpass.colorAttachmentCount = 1;
	Subpass.pColorAttachments = &ColorAttachmentRef;

	VkSubpassDependency Dependency = {};
	Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	Dependency.dstSubpass = 0;
	Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	Dependency.srcAccessMask = 0;
	Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo RenderPassInfo = {};
	RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassInfo.attachmentCount = 1;
	RenderPassInfo.pAttachments = &ColorAttachment;
	RenderPassInfo.subpassCount = 1;
	RenderPassInfo.pSubpasses = &Subpass;
	RenderPassInfo.dependencyCount = 1;
	RenderPassInfo.pDependencies = &Dependency;

	if (vkCreateRenderPass(Device, &RenderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Render pass creation failed !");
	}

}

void RenderPassManager::CreateFramebuffers(const FramebufferCreateInfo& CreateInfo)
{
	mFramebuffers.resize(CreateInfo.mSwapChainImageViews->size());

	for (size_t i = 0; i < CreateInfo.mSwapChainImageViews->size(); i++)
	{
		VkImageView Attachments[] = {
			(*CreateInfo.mSwapChainImageViews)[i]
		};

		VkFramebufferCreateInfo FramebufferCreateInfo = {};
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.renderPass = *CreateInfo.mRenderPassHandle;
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.pAttachments = Attachments;
		FramebufferCreateInfo.width = CreateInfo.mSwapChainImageExtent->width;
		FramebufferCreateInfo.height = CreateInfo.mSwapChainImageExtent->height;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(*CreateInfo.mLogicalDevice, &FramebufferCreateInfo, nullptr, &mFramebuffers[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Framebuffer creation failed.");
		}

	}

}

void RenderPassManager::CreateRenderPassCommandBuffers(const RenderPassCommandBufferCreateInfo& CreateInfo)
{
	mRenderPassCommandBuffers.resize(CreateInfo.mBufferSize);

	VkCommandBufferAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.commandPool = *CreateInfo.mCommandPool;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandBufferCount = uint32_t(mRenderPassCommandBuffers.size());

	if (vkAllocateCommandBuffers(*CreateInfo.mLogicalDevice, &AllocInfo, mRenderPassCommandBuffers.data()) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Command buffer allocation failed.");
	}

	for (size_t i = 0; i < GetRenderPassCommandBuffers()->size(); i++)
	{
		VkCommandBufferBeginInfo CommandBufferBI = {};
		CommandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CommandBufferBI.flags = 0; // Optional
		CommandBufferBI.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer((*GetRenderPassCommandBuffers())[i], &CommandBufferBI) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Error beginning command buffer.");
		}

		VkRenderPassBeginInfo RenderPassBI = {};
		RenderPassBI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBI.renderPass = *GetRenderPassHandle();
		RenderPassBI.framebuffer = (*GetFramebuffers())[i];
		RenderPassBI.renderArea.offset = { 0,0 };
		RenderPassBI.renderArea.extent = CreateInfo.mSwapChainExtent;

		VkClearValue ClearColor = { 0.1f, 0.1f, 0.1f, 1.f };

		RenderPassBI.clearValueCount = 1;
		RenderPassBI.pClearValues = &ClearColor;

		vkCmdBeginRenderPass(mRenderPassCommandBuffers[i], &RenderPassBI, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(mRenderPassCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *CreateInfo.mPipelineHandle);

			VkBuffer VertexBuffers[] = { CreateInfo.mVertexBufferData->mBufferData.mBuffer };
			VkDeviceSize Offsets[] = { 0 };
			vkCmdBindVertexBuffers(mRenderPassCommandBuffers[i], 0, 1, VertexBuffers, Offsets);

			// [TODO] Needs change to distinct UINT16 and UINT32 index buffer type.
			VkBuffer IndexBuffers[] = { CreateInfo.mIndexBufferData->mBufferData.mBuffer };
			vkCmdBindIndexBuffer(mRenderPassCommandBuffers[i], *IndexBuffers, 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(mRenderPassCommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, *CreateInfo.mPipelineLayout, 0, 1, &(*CreateInfo.mDescriptorSets)[i], 0, nullptr);

			vkCmdDrawIndexed(mRenderPassCommandBuffers[i], static_cast<uint32_t>(CreateInfo.mIndexBufferData->mIndices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(mRenderPassCommandBuffers[i]);

		if (vkEndCommandBuffer(mRenderPassCommandBuffers[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Failed to record command buffer!");
		}
	}
}

const VkRenderPass* const RenderPassManager::GetRenderPassHandle() const
{
	return &mRenderPass;
}

const std::vector<VkFramebuffer>* RenderPassManager::GetFramebuffers() const
{
	return &mFramebuffers;
}

void RenderPassManager::CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool)
{
	vkFreeCommandBuffers(Device, *CommandPool, static_cast<uint32_t>(GetRenderPassCommandBuffers()->size()), GetRenderPassCommandBuffers()->data());
	for (int i = int(mFramebuffers.size() - 1); i >= 0 ; i--)
	{
		vkDestroyFramebuffer(Device, mFramebuffers[i], nullptr);
		mFramebuffers.erase(mFramebuffers.begin() + i);
	}
	if (mRenderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(Device, mRenderPass, nullptr);
		mRenderPass = VK_NULL_HANDLE;
	}
}

void RenderPassManager::Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool)
{
	// [Arch1eN] Upon destruction, render pass command buffers must be erased
	mRenderPassCommandBuffers.erase(mRenderPassCommandBuffers.begin(), mRenderPassCommandBuffers.end());

	CleanUp(Device, CommandPool);

	mFramebuffers.erase(mFramebuffers.begin(), mFramebuffers.end());
}

const std::vector<VkCommandBuffer>* RenderPassManager::GetRenderPassCommandBuffers() const
{
	return &mRenderPassCommandBuffers;
}
