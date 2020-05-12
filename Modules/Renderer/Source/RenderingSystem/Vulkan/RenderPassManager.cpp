#include "RenderPassManager.h"
#include "Debug/LogSystem.h"
#include "VulkanUtilities.h"


RenderPassManager::RenderPassManager()
{
	CreatePipelineSystem();
}

void RenderPassManager::CreatePipelineSystem()
{
	mPipelineSystem = std::make_unique<PipelineSystem>();
}

PipelineSystem* const RenderPassManager::GetPipelineSystem() const
{
	return mPipelineSystem.get();
}

void RenderPassManager::CreateRenderPass
(
	const RenderPassID& ID,
	const VkDevice* LogicalDevice,
	VkRenderPassCreateInfo RenderPassCreateInfo
)
{
	Assert(ID.compare("") != 0, "Render pass ID cannot be empty!");

	RenderPassData NewData;

	if (vkCreateRenderPass(*LogicalDevice, &RenderPassCreateInfo, nullptr, &NewData.mRenderPassHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Render pass creation failed !");
	}


	mRenderPasses.insert({ ID, NewData });
}

void RenderPassManager::CreateFramebuffers
(	
	const RenderPassID& ID,
	const VkDevice* LogicalDevice,
	const VkRenderPass* RenderPassHandle,
	const std::vector<VkImageView>* SwapChainImageViews,
	const VkExtent2D* SwapChainImageExtent
)
{
	// [Todo] This can be done better.
	RenderPassData* Data = &mRenderPasses.find(ID)->second;

	Data->mFramebufferHandles.resize(SwapChainImageViews->size());

	for (size_t i = 0; i < SwapChainImageViews->size(); i++)
	{
		VkImageView Attachments[] = {
			(*SwapChainImageViews)[i]
		};

		VkFramebufferCreateInfo FramebufferCreateInfo = {};
		FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		FramebufferCreateInfo.renderPass = *RenderPassHandle;
		FramebufferCreateInfo.attachmentCount = 1;
		FramebufferCreateInfo.pAttachments = Attachments;
		FramebufferCreateInfo.width = SwapChainImageExtent->width;
		FramebufferCreateInfo.height = SwapChainImageExtent->height;
		FramebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(*LogicalDevice, &FramebufferCreateInfo, nullptr, &Data->mFramebufferHandles[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Framebuffer creation failed.");
		}
	}

}

void RenderPassManager::CreateRenderPassCommandBuffers
(
	const RenderPassID&					ID,
	const VkDevice*						LogicalDevice,
	const VkCommandPool*				CommandPoolHandle,
	const VkPipeline*					PipelineHandle,
	const VkPipelineLayout*				PipelineLayout,
	const std::vector<VkDescriptorSet>* DescriptorSets,
	size_t								BufferSize,
	VkExtent2D							SwapChainExtent,
	const VertexBufferData*				VertexBufferData,
	const IndexBufferData*				IndexBufferData
	)
{
	// [Todo] This can be done better.
	RenderPassData* Data = &mRenderPasses.find(ID)->second;
	std::vector < VkCommandBuffer>& CommandBufferHandles = Data->mCommandBufferHandles;

	CommandBufferHandles.resize(BufferSize);

	VkCommandBufferAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.commandPool = *CommandPoolHandle;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandBufferCount = uint32_t(CommandBufferHandles.size());

	if (vkAllocateCommandBuffers(*LogicalDevice, &AllocInfo, CommandBufferHandles.data()) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Command buffer allocation failed.");
	}

	for (size_t i = 0; i < CommandBufferHandles.size(); i++)
	{
		VkCommandBufferBeginInfo CommandBufferBI = {};
		CommandBufferBI.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		CommandBufferBI.flags = 0; // Optional
		CommandBufferBI.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(CommandBufferHandles[i], &CommandBufferBI) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Error beginning command buffer.");
		}

		VkRenderPassBeginInfo RenderPassBI = {};
		RenderPassBI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		RenderPassBI.renderPass = *GetMainRenderPassHandle();
		RenderPassBI.framebuffer = Data->mFramebufferHandles[i];
		RenderPassBI.renderArea.offset = { 0,0 };
		RenderPassBI.renderArea.extent = SwapChainExtent;

		VkClearValue ClearColor = { 0.1f, 0.1f, 0.1f, 1.f };

		RenderPassBI.clearValueCount = 1;
		RenderPassBI.pClearValues = &ClearColor;

		auto& CommandBuffer = CommandBufferHandles[i];

		vkCmdBeginRenderPass(CommandBuffer, &RenderPassBI, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PipelineHandle);

			VkBuffer VertexBuffers[] = { VertexBufferData->mBufferData.mBuffer };
			VkDeviceSize Offsets[] = { 0 };
			vkCmdBindVertexBuffers(CommandBuffer, 0, 1, VertexBuffers, Offsets);

			// [TODO] Needs change to distinct UINT16 and UINT32 index buffer type.
			VkBuffer IndexBuffers[] = { IndexBufferData->mBufferData.mBuffer };
			vkCmdBindIndexBuffer(CommandBuffer, *IndexBuffers, 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *PipelineLayout, 0, 1, &(*DescriptorSets)[i], 0, nullptr);

			vkCmdDrawIndexed(CommandBuffer, static_cast<uint32_t>(IndexBufferData->mIndices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(CommandBuffer);

		if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Failed to record command buffer!");
		}
	}
}

const VkRenderPass* const RenderPassManager::GetMainRenderPassHandle()
{
	return GetRenderPassHandle("main");
}

const RenderPassData* const RenderPassManager::GetRenderPassData(const RenderPassID ID)
{
	return &mRenderPasses[ID];
}

const std::map<RenderPassID, RenderPassData>& RenderPassManager::GetAllRenderPasses() const
{
	return mRenderPasses;
}

const VkRenderPass* const RenderPassManager::GetRenderPassHandle(const RenderPassID ID)
{
	return &mRenderPasses[ID].mRenderPassHandle;
}

void RenderPassManager::CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool)
{
	auto& RenderPassesLocalRef = this->mRenderPasses;
	std::vector<VkCommandBuffer> CommandBuffers;
	for (auto RenderPass : RenderPassesLocalRef)
	{
		for (auto CommandBuffer : RenderPass.second.mCommandBufferHandles)
		{
			CommandBuffers.push_back(CommandBuffer);
		}
	}

	vkFreeCommandBuffers(Device, *CommandPool, static_cast<uint32_t>(CommandBuffers.size()), CommandBuffers.data());

	for (auto iter = mRenderPasses.rbegin(); iter != mRenderPasses.rend(); ++iter)
	{
		auto& RenderPassHandle = iter->second.mRenderPassHandle;
		if (RenderPassHandle != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(Device, RenderPassHandle, nullptr);
		}

		auto& FrameBufferHandles = iter->second.mFramebufferHandles;
		for (auto FrameBufferIter = FrameBufferHandles.rbegin(); FrameBufferIter != FrameBufferHandles.rend(); ++FrameBufferIter)
		{
			auto& FrameBufferHandle = *FrameBufferIter;
			if (FrameBufferHandle != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(Device, FrameBufferHandle, nullptr);
			}
		}

	}

	mRenderPasses.clear();

	GetPipelineSystem()->CleanUp(Device);
}

void RenderPassManager::Destroy(const VkDevice& Device, const VkCommandPool* const CommandPool)
{
	CleanUp(Device, CommandPool);

	for (auto i : mRenderPasses)
	{
		auto& CommandBufferHandles = i.second.mCommandBufferHandles;
		CommandBufferHandles.erase(CommandBufferHandles.begin(), CommandBufferHandles.end());

		auto& FramebufferHandles = i.second.mFramebufferHandles;
		FramebufferHandles.erase(FramebufferHandles.begin(), FramebufferHandles.end());
	}

	GetPipelineSystem()->Destroy(Device);
}
