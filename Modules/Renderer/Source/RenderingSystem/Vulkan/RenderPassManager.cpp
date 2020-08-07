#include "RenderPassManager.h"
#include "Debug/LogSystem.h"
#include "VulkanUtilities.h"

RenderPassManager::RenderPassManager(const VkDevice* LogicalDevice)
{
	CreatePipelineSystem(LogicalDevice);
}

void RenderPassManager::CreatePipelineSystem(const VkDevice* LogicalDevice)
{
	mPipelineSystem = std::make_unique<PipelineSystem>(LogicalDevice);
}

PipelineSystem* const RenderPassManager::GetPipelineSystem() const
{
	return mPipelineSystem.get();
}

void RenderPassManager::CreateRenderPass
(
	const RenderPassID& ID,
	const VkDevice* LogicalDevice,
	uint32_t NumCommandBuffersToMake,
	const std::vector<VkImageView>& SwapChainImageViews,
	const VkExtent2D& SwapChainImageExtent,
	uint32_t PresentationSuitableQueueFamilyIndex,
	const VkRenderPassCreateInfo& RenderPassCreateInfo,
	RecordingRenderPassDelegate&& RecordingRenderPassFunc
)
{
	Assert(ID.compare("") != 0, "Render pass ID cannot be empty!");

	RenderPassData NewData;

	if (vkCreateRenderPass(*LogicalDevice, &RenderPassCreateInfo, nullptr, &NewData.mRenderPassHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Render pass creation failed !");
	}

	NewData.mRecordingRenderPassFunc = std::move(RecordingRenderPassFunc);

	NewData.mFrameData.resize(SwapChainImageViews.size());

	for (uint32_t i = 0; i < SwapChainImageViews.size(); i++)
	{
		FrameData NewFrameData = {};
		NewFrameData.mFrameBufferHandle = CreateFramebuffer(LogicalDevice, &NewData.mRenderPassHandle, SwapChainImageViews[i], SwapChainImageExtent);
		NewFrameData.mFinishedFrameRenderingSemaphoreHandle = CreateSemaphore(LogicalDevice);
		NewFrameData.mFrameCommandPool = CreateCommandPool(LogicalDevice, PresentationSuitableQueueFamilyIndex);

		CreateAndAllocateCommandBuffers(LogicalDevice, NewFrameData.mCommandBufferHandles, &NewFrameData.mFrameCommandPool, NumCommandBuffersToMake);

		NewData.mFrameData[i] = std::move(NewFrameData);
	}

	mRenderPasses.insert({ID, std::move(NewData)});
}

// [Test]
VkFramebuffer RenderPassManager::CreateFramebuffer
(
	const VkDevice* LogicalDevice,
	const VkRenderPass* RenderPassHandle,
	const VkImageView& SwapChainImageView,
	const VkExtent2D& SwapChainImageExtent
)
{
	VkFramebuffer NewFrameBuffer;

	VkImageView Attachments[] = {
		SwapChainImageView
	};
	
	VkFramebufferCreateInfo FramebufferCreateInfo = {};
	FramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	FramebufferCreateInfo.renderPass = *RenderPassHandle;
	FramebufferCreateInfo.attachmentCount = 1;
	FramebufferCreateInfo.pAttachments = Attachments;
	FramebufferCreateInfo.width = SwapChainImageExtent.width;
	FramebufferCreateInfo.height = SwapChainImageExtent.height;
	FramebufferCreateInfo.layers = 1;
	
	if(vkCreateFramebuffer(*LogicalDevice, &FramebufferCreateInfo, nullptr, &NewFrameBuffer) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Framebuffer creation failed.");
	}

	return NewFrameBuffer;
}


const VkRenderPass* const RenderPassManager::GetMainRenderPassHandle()
{
	return GetRenderPassHandle("main");
}

RenderPassData* const RenderPassManager::GetRenderPassData(const RenderPassID& ID)
{
	return &mRenderPasses[ID];
}

const std::map<RenderPassID, RenderPassData> RenderPassManager::GetAllRenderPasses() const
{
	return mRenderPasses;
}

const VkRenderPass* const RenderPassManager::GetRenderPassHandle(const RenderPassID& ID)
{
	return &mRenderPasses[ID].mRenderPassHandle;
}

void RenderPassManager::CleanUp(const VkDevice& Device, const VkCommandPool* const CommandPool)
{
	for (auto iter = mRenderPasses.rbegin(); iter != mRenderPasses.rend(); ++iter)
	{
		for (auto FrameDataIter : iter->second.mFrameData)
		{
			if (FrameDataIter.mFrameBufferHandle != VK_NULL_HANDLE)
			{
				vkDestroyFramebuffer(Device, FrameDataIter.mFrameBufferHandle, nullptr);
				FrameDataIter.mFrameBufferHandle = VK_NULL_HANDLE;
			}

			vkFreeCommandBuffers(Device, FrameDataIter.mFrameCommandPool, uint32_t(FrameDataIter.mCommandBufferHandles.size()), FrameDataIter.mCommandBufferHandles.data());
			auto& CommandBufferHandles = FrameDataIter.mCommandBufferHandles;
			CommandBufferHandles.erase(CommandBufferHandles.begin(), CommandBufferHandles.end());

			vkDestroySemaphore(Device, FrameDataIter.mFinishedFrameRenderingSemaphoreHandle, nullptr);
			FrameDataIter.mFinishedFrameRenderingSemaphoreHandle = VK_NULL_HANDLE;

			vkDestroyCommandPool(Device, FrameDataIter.mFrameCommandPool, nullptr);

		}

		auto& RenderPassHandle = iter->second.mRenderPassHandle;
		if (RenderPassHandle != VK_NULL_HANDLE)
		{
			vkDestroyRenderPass(Device, RenderPassHandle, nullptr);
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



	}

	GetPipelineSystem()->Destroy(Device);
}
