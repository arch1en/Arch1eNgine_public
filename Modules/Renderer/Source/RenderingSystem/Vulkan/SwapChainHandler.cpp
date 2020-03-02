#include "SwapChainHandler.h"

#include <algorithm>
#include <limits>
#include <chrono>

#include <glm/glm.hpp>

#include "Debug/LogSystem.h"
#include "DeviceHandler.h" // CIRCULAR DEPENDENCY !
#include "VulkanUtilities.h"

#include "RenderingSystem/Vulkan/PipelineSystem.h"

void SwapChainHandler::Initiate(const SwapChainHandlerInitiationInfo& InitiationInfo)
{
	CreateRenderPassManager();
	CreateMemoryManager(*InitiationInfo.mLogicalDevice, InitiationInfo.mQueueFamilyHandler);

	CreateSemaphores(InitiationInfo.mLogicalDevice);
	CreateFences(InitiationInfo.mLogicalDevice);

	CommandPoolCreateInfo CommandPoolCI = {};
	CommandPoolCI.mLogicalDevice = InitiationInfo.mLogicalDevice;
	CommandPoolCI.mQueueFamilyData = InitiationInfo.mQueueFamilyHandler->GetQueueFamilyData();

	CreateCommandPool(CommandPoolCI);
}

void SwapChainHandler::PrepareVertexMemory(const GeneralBufferCreationInfo& BufferCreationInfo, std::vector<Vertex> Vertices)
{
	GetMemoryManager()->CreateBuffer(BufferCreationInfo, Vertices);
}

void SwapChainHandler::PrepareIndexMemory(const GeneralBufferCreationInfo& BufferCreationInfo, std::vector<uint16_t> Indices)
{
	GetMemoryManager()->CreateBuffer(BufferCreationInfo, Indices);
}

void SwapChainHandler::CreateRenderPassManager()
{
	mRenderPassManager = std::make_unique<RenderPassManager>();
}

//void SwapChainHandler::CreatePipelineSystem()
//{
//	mPipelineSystem = std::make_unique<PipelineSystem>();
//}

void SwapChainHandler::CreateMemoryManager(const VkDevice& LogicalDevice, const QueueFamilyHandler* QFH)
{
	mMemoryManager = std::make_unique<MemoryManager>(LogicalDevice, QFH);
}

void SwapChainHandler::CreateSemaphores(const VkDevice* Device)
{
	Assert(mImageAvailableSemaphores.size() == 0, "Array must be empty at this point.");
	Assert(mRenderFinishedSemaphores.size() == 0, "Array must be empty at this point.");

	mImageAvailableSemaphores.resize(MaxFramesInFlight);
	mRenderFinishedSemaphores.resize(MaxFramesInFlight);

	VkSemaphoreCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for (int i = 0; i < mImageAvailableSemaphores.size(); i++)
	{
		if (vkCreateSemaphore(*Device, &CreateInfo, nullptr, &mImageAvailableSemaphores[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Image Available Semaphores Creation failed!");
		}
	}

	for (int i = 0; i < mRenderFinishedSemaphores.size(); i++)
	{
		if (vkCreateSemaphore(*Device, &CreateInfo, nullptr, &mRenderFinishedSemaphores[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Rendering Finished Semaphores Creation failed!");
		}
	}
}

void SwapChainHandler::CreateFences(const VkDevice* Device)
{
	Assert(mInFlightFences.size() == 0, "Array must be empty at this point.");

	mInFlightFences.resize(MaxFramesInFlight);

	VkFenceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	// By default Fences are created in an unsignaled state. That means vkWaitForFences will wait forever for them.
	// We can change its state on the creation time, so that vkWaitForFences will catch it the first time before rendering.
	CreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < mInFlightFences.size(); i++)
	{
		if (vkCreateFence(*Device, &CreateInfo, nullptr, &mInFlightFences[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "In Flight Fences Creation failed!");
		}
	}
}

void SwapChainHandler::CreateSwapChain(const SwapChainCreationInfo& CreationInfo)
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(*CreationInfo.mPhysicalDevice, *CreationInfo.mSurface);

	VkSurfaceFormatKHR SurfaceFormat = ChooseSwapSurfaceFormat(SupportDetails.mFormats);
	VkPresentModeKHR PresentMode = ChooseSwapPresentMode(SupportDetails.mPresentModes);
	VkExtent2D Extent = ChooseSwapExtent(SupportDetails.mCapabilities);

	uint32_t ImageCount = SupportDetails.mCapabilities.minImageCount + 1;

	if (SupportDetails.mCapabilities.maxImageCount > 0 && ImageCount > SupportDetails.mCapabilities.maxImageCount)
	{
		ImageCount = SupportDetails.mCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	CreateInfo.surface = *CreationInfo.mSurface;
	CreateInfo.minImageCount = ImageCount;
	CreateInfo.imageFormat = SurfaceFormat.format;
	CreateInfo.imageColorSpace = SurfaceFormat.colorSpace;
	CreateInfo.imageExtent = Extent;
	CreateInfo.imageArrayLayers = 1;
	CreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (CreationInfo.mQueueFamilyHandler->GetNumberOfQueueFamilies() > 1)
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		CreateInfo.queueFamilyIndexCount = uint32_t(CreationInfo.mQueueFamilyHandler->GetNumberOfQueueFamilies());
		CreateInfo.pQueueFamilyIndices = (*CreationInfo.mQueueFamilyHandler->GetAlignedQueueFamiliesIndices()).data();
	}
	else
	{
		CreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		CreateInfo.queueFamilyIndexCount = 0; // Optional
		CreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	CreateInfo.preTransform = SupportDetails.mCapabilities.currentTransform;
	CreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	CreateInfo.presentMode = PresentMode;
	CreateInfo.clipped = VK_TRUE;
	CreateInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(*CreationInfo.mLogicalDevice, &CreateInfo, nullptr, &mSwapChainHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Swap chain creation failure !");
		return;
	}

	// Cache for image view creation
	mSwapChainImageFormat = SurfaceFormat.format;
	mSwapChainExtent = Extent;

	RetrieveSwapChainImages(*CreationInfo.mLogicalDevice, mSwapChainImages);

	CreateSwapChainImageView(*CreationInfo.mLogicalDevice);

	CreateMainRenderPass(CreationInfo.mLogicalDevice, CreationInfo.mPhysicalDevice, CreationInfo.mQueueFamilyHandler);
}

void SwapChainHandler::CreateMainRenderPass(const VkDevice* LogicalDevice, const VkPhysicalDevice* PhysicalDevice, const QueueFamilyHandler* QueueFamilyHandler)
{
	// Render Pass Creation

	RenderPassCreateInfo RenderPassCI = {};

	// Color Attachment
	RenderPassCI.mAttachmentDescriptrion.format = mSwapChainImageFormat;
	RenderPassCI.mAttachmentDescriptrion.samples = VK_SAMPLE_COUNT_1_BIT;
	RenderPassCI.mAttachmentDescriptrion.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	RenderPassCI.mAttachmentDescriptrion.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	RenderPassCI.mAttachmentDescriptrion.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	RenderPassCI.mAttachmentDescriptrion.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	RenderPassCI.mAttachmentDescriptrion.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	RenderPassCI.mAttachmentDescriptrion.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// Color Attachment Reference.
	RenderPassCI.mAttachmentReference.attachment = 0;
	RenderPassCI.mAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	RenderPassCI.mSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	RenderPassCI.mSubpassDescription.colorAttachmentCount = 1;
	RenderPassCI.mSubpassDescription.pColorAttachments = &RenderPassCI.mAttachmentReference;

	RenderPassCI.mSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	RenderPassCI.mSubpassDependency.dstSubpass = 0;
	RenderPassCI.mSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	RenderPassCI.mSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	RenderPassCI.mSubpassDependency.srcAccessMask = 0;
	RenderPassCI.mSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	RenderPassCI.mRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	RenderPassCI.mRenderPassCreateInfo.attachmentCount = 1;
	RenderPassCI.mRenderPassCreateInfo.pAttachments = &RenderPassCI.mAttachmentDescriptrion;
	RenderPassCI.mRenderPassCreateInfo.subpassCount = 1;
	RenderPassCI.mRenderPassCreateInfo.pSubpasses = &RenderPassCI.mSubpassDescription;
	RenderPassCI.mRenderPassCreateInfo.dependencyCount = 1;
	RenderPassCI.mRenderPassCreateInfo.pDependencies = &RenderPassCI.mSubpassDependency;

	RenderPassCI.mLogicalDevice = LogicalDevice;
	// RenderPass. (Needs to be created before pipeline. Needs to be created after swap chain.)
	GetRenderPassManager()->CreateRenderPass("main", RenderPassCI);

	// ~Render Pass Creation

	// Framebuffer.

	FramebufferCreateInfo FramebufferCreationInfo = {};

	FramebufferCreationInfo.mLogicalDevice = LogicalDevice;
	FramebufferCreationInfo.mRenderPassHandle = GetRenderPassManager()->GetMainRenderPassHandle();
	FramebufferCreationInfo.mSwapChainImageExtent = &GetSwapChainExtent();
	FramebufferCreationInfo.mSwapChainImageViews = GetSwapChainImageViews();

	GetRenderPassManager()->CreateFramebuffers("main", FramebufferCreationInfo);

	GeneralBufferCreationInfo UniformBufferCI = {};
	UniformBufferCI.mBufferCreationInfo.mLogicalDevice = LogicalDevice;
	UniformBufferCI.mBufferCreationInfo.mPhysicalDevice = PhysicalDevice;
	UniformBufferCI.mBufferCreationInfo.mDataSize = sizeof(UniformBufferObject);
	UniformBufferCI.mQueueFamilyHandler = QueueFamilyHandler;

	GetMemoryManager()->CreateUniformBuffers(UniformBufferCI, static_cast<uint8_t>(mSwapChainImages.size()));

	PipelineSystemCreationInfo PipelineCreationInfo = {};

	PipelineCreationInfo.mLogicalDevice = LogicalDevice;
	PipelineCreationInfo.mImageFormat = GetSwapChainImageFormat();
	PipelineCreationInfo.mViewportExtent = GetSwapChainExtent();
	PipelineCreationInfo.mRenderPassHandle = GetRenderPassManager()->GetMainRenderPassHandle();
	PipelineCreationInfo.mMemoryManager = GetMemoryManager();

	GetRenderPassManager()->GetPipelineSystem()->CreateGraphicsPipeline(PipelineCreationInfo);

	// Descriptor Pool Creation and Descriptor Sets Update.
	DescriptorPoolCreateInfo DescriptorPoolCI = {};

	DescriptorPoolCI.mLogicalDevice = LogicalDevice;

	DescriptorPoolCI.mPoolSizes = { {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,  static_cast<uint32_t>(mSwapChainImages.size())} };

	DescriptorPoolCI.mPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	DescriptorPoolCI.mPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolCI.mPoolSizes.size());
	DescriptorPoolCI.mPoolCreateInfo.pPoolSizes = DescriptorPoolCI.mPoolSizes.data();
	DescriptorPoolCI.mPoolCreateInfo.maxSets = static_cast<uint32_t>(mSwapChainImages.size());
	DescriptorPoolCI.mPoolCreateInfo.flags = 0;
	DescriptorPoolCI.mDescriptorPoolID = "main";

	GetRenderPassManager()->GetPipelineSystem()->CreateDescriptorPoolAndUpdateDescriptorSets(DescriptorPoolCI, GetMemoryManager(), mSwapChainImages.size());

	RenderPassCommandBufferCreateInfo RenderPassCommandBufferCI = {};

	RenderPassCommandBufferCI.mLogicalDevice = LogicalDevice;
	RenderPassCommandBufferCI.mBufferSize = GetRenderPassManager()->GetRenderPassData("main")->mFramebufferHandles.size();
	RenderPassCommandBufferCI.mCommandPool = GetCommandPool();
	RenderPassCommandBufferCI.mPipelineHandle = GetRenderPassManager()->GetPipelineSystem()->GetPipelineHandle();
	// [TODO] This will need a refactoring.
	RenderPassCommandBufferCI.mPipelineLayout = GetRenderPassManager()->GetPipelineSystem()->GetPipelineLayout();
	RenderPassCommandBufferCI.mDescriptorSets = GetRenderPassManager()->GetPipelineSystem()->GetDescriptorSets();
	RenderPassCommandBufferCI.mSwapChainExtent = GetSwapChainExtent();
	// [TODO] This will need a refactoring.
	RenderPassCommandBufferCI.mVertexBufferData = (*GetMemoryManager()->GetVertexBufferData())[0].get();
	RenderPassCommandBufferCI.mIndexBufferData = (*GetMemoryManager()->GetIndexBufferData())[0].get();

	GetRenderPassManager()->CreateRenderPassCommandBuffers("main", RenderPassCommandBufferCI);
}

//void SwapChainHandler::CreateDescriptorPoolAndUpdateDescriptorSets(const DescriptorPoolCreateInfo& CreateInfo)
//{
//	CreateDescriptorPool(CreateInfo);
//	UpdateDescriptorSets(CreateInfo.mLogicalDevice);
//}

//void SwapChainHandler::CreateDescriptorPool(const DescriptorPoolCreateInfo& CreateInfo)
//{
//	Assert(CreateInfo.mDescriptorPoolID.compare("") != 0, "Descriptor pool ID cannot be empty!");
//
//	VkDescriptorPool Pool;
//
//	if (vkCreateDescriptorPool(*CreateInfo.mLogicalDevice, &CreateInfo.mPoolCreateInfo, nullptr, &Pool) != VK_SUCCESS)
//	{
//		LogVk(LogType::Error, 0, "Descriptor pool creation failed!");
//	}
//
//	mDescriptorPools.insert({ CreateInfo.mDescriptorPoolID, Pool });
//}

//void SwapChainHandler::UpdateDescriptorSets(const VkDevice* Device)
//{
//	std::vector<VkDescriptorSetLayout> LayoutsToSet(mSwapChainImages.size(), GetPipelineSystem()->GetDescriptorSetLayouts()[0]);
//
//	VkDescriptorSetAllocateInfo AllocInfo = {};
//
//	AllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//	AllocInfo.descriptorPool = *GetMainDescriptorPool();
//	AllocInfo.descriptorSetCount = static_cast<uint32_t>(mSwapChainImages.size());
//	AllocInfo.pSetLayouts = LayoutsToSet.data();
//
//	mDescriptorSets.resize(mSwapChainImages.size());
//	if (vkAllocateDescriptorSets(*Device, &AllocInfo, mDescriptorSets.data()) != VK_SUCCESS)
//	{
//		LogVk(LogType::Error, 0, "Descriptor sets allocation failed!");
//	}
//
//	for (size_t i = 0; i < mSwapChainImages.size(); i++)
//	{
//		VkDescriptorBufferInfo BufferInfo = {};
//		BufferInfo.buffer = (*GetMemoryManager()->GetUniformBufferData())[i]->mBuffer;
//		BufferInfo.offset = 0;
//		BufferInfo.range = sizeof(UniformBufferObject);
//
//		VkWriteDescriptorSet DescriptorWrite = {};
//		DescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//		DescriptorWrite.dstSet = mDescriptorSets[i];
//		DescriptorWrite.dstBinding = 0;
//		DescriptorWrite.dstArrayElement = 0;
//		DescriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//		DescriptorWrite.descriptorCount = 1;
//		DescriptorWrite.pBufferInfo = &BufferInfo;
//		DescriptorWrite.pImageInfo = nullptr;	// Optional
//		DescriptorWrite.pTexelBufferView = nullptr; // Optional
//
//		vkUpdateDescriptorSets(*Device, 1, &DescriptorWrite, 0, nullptr);
//	}
//}

void SwapChainHandler::CreateCommandPool(const CommandPoolCreateInfo& CreateInfo)
{
	VkCommandPoolCreateInfo PoolInfo = {};

	uint32_t QueueFamilyIndex = -1;

	for (int i = 0; i < CreateInfo.mQueueFamilyData->size(); i++)
	{
		if ((*CreateInfo.mQueueFamilyData)[i].IsPresentationSuitable)
		{
			QueueFamilyIndex = (*CreateInfo.mQueueFamilyData)[i].FamilyIndex;
			break;
		}
	}

	PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	PoolInfo.queueFamilyIndex = QueueFamilyIndex;
	PoolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(*CreateInfo.mLogicalDevice, &PoolInfo, nullptr, &mCommandPool) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Command pool creation failed.");
	}
}

void SwapChainHandler::ReCreateSwapChain(const SwapChainCreationInfo& CreationInfo)
{
	vkDeviceWaitIdle(*CreationInfo.mLogicalDevice);

	Cleanup(CreationInfo.mLogicalDevice);

	CreateSwapChain(CreationInfo);
}

void SwapChainHandler::CreateSwapChainImageView(const VkDevice& Device)
{
	mSwapChainImageViews.resize(mSwapChainImages.size());

	for (size_t i = 0; i < mSwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo CreateInfo = {};
		
		CreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		CreateInfo.image = mSwapChainImages[i];
		CreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		CreateInfo.format = mSwapChainImageFormat;
		
		CreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		CreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		CreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		CreateInfo.subresourceRange.baseMipLevel = 0;
		CreateInfo.subresourceRange.levelCount = 1;
		CreateInfo.subresourceRange.baseArrayLayer = 0;
		CreateInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(Device, &CreateInfo, nullptr, &mSwapChainImageViews[i]) != VK_SUCCESS)
		{
			LogVk(LogType::Error, 0, "Failed to create image view !");
		}
	}
}

void SwapChainHandler::RetrieveSwapChainImages(const VkDevice& Device, std::vector<VkImage>& SwapChainImages)
{
	uint32_t ImageCount = 0;
	vkGetSwapchainImagesKHR(Device, mSwapChainHandle, &ImageCount, nullptr);
	SwapChainImages.resize(ImageCount);
	vkGetSwapchainImagesKHR(Device, mSwapChainHandle, &ImageCount, SwapChainImages.data());
}

void SwapChainHandler::RequestFrameBufferResizing()
{
	mRequestFrameBufferResizing = true;
}

EDrawFrameErrorCode SwapChainHandler::DrawFrame(const VkDevice& Device, const VkQueue& PresentQueueHandle)
{
	EDrawFrameErrorCode ErrorCode = EDrawFrameErrorCode::OK;

	std::vector<VkCommandBuffer> RenderPassesCommandBuffers;

	for (auto RenderPass : GetRenderPassManager()->GetAllRenderPasses())
	{
		for (auto CommandBuffer : RenderPass.second.mCommandBufferHandles)
		{
			RenderPassesCommandBuffers.push_back(CommandBuffer);
		}
	}

	Assert(Device, "Device must be valid at this point!");

	uint32_t ImageIndex;

	uint64_t Timeout = std::numeric_limits <uint64_t>::max(); // Timeout in nanoseconds. Using the maximum value of a 64bit unsigned integer disables the timeout.

	vkWaitForFences(Device, 1, &mInFlightFences[mCurrentFrameIndex], VK_TRUE, UINT64_MAX);
	vkResetFences(Device, 1, &mInFlightFences[mCurrentFrameIndex]);

	VkResult ImageAcquisitionResult = vkAcquireNextImageKHR(Device, *GetSwapChainHandle(), Timeout, mImageAvailableSemaphores[mCurrentFrameIndex], VK_NULL_HANDLE, &ImageIndex);

	if (ImageAcquisitionResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		return EDrawFrameErrorCode::SwapChainRecreationRequired;
	}
	else if (ImageAcquisitionResult != VK_SUCCESS && ImageAcquisitionResult != VK_SUBOPTIMAL_KHR)
	{
		LogVk(LogType::Error, 0, "Swap Chain Image Acquisition Failed!");
		return EDrawFrameErrorCode::SwapChainImageAcquisitionFailed;
	}

	// @todo : This is temporary. Time should be taken from the rendering loop.
	static auto StartTime = std::chrono::high_resolution_clock::now();
	auto CurrentTime = std::chrono::high_resolution_clock::now();
	float Time = std::chrono::duration<float, std::chrono::seconds::period>(CurrentTime - StartTime).count();

	GetMemoryManager()->UpdateUniformBuffer(&Device, Time, ImageIndex, mSwapChainExtent);

	VkPipelineStageFlags WaitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	VkSemaphore WaitSemaphores[] = { mImageAvailableSemaphores[mCurrentFrameIndex] };
	VkSemaphore SignalSemaphores[] = { mRenderFinishedSemaphores[mCurrentFrameIndex] };

	VkSubmitInfo SubmitInfo = {};
	SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	SubmitInfo.waitSemaphoreCount = 1;
	SubmitInfo.pWaitSemaphores = WaitSemaphores;
	SubmitInfo.pWaitDstStageMask = WaitStages;
	SubmitInfo.commandBufferCount = 1;
	SubmitInfo.pCommandBuffers = &RenderPassesCommandBuffers[ImageIndex];
	SubmitInfo.pSignalSemaphores = SignalSemaphores;
	SubmitInfo.signalSemaphoreCount = 1;

	if (vkQueueSubmit(PresentQueueHandle, 1, &SubmitInfo, mInFlightFences[mCurrentFrameIndex]) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Queue submission failed!");
		return EDrawFrameErrorCode::QueueSubmissionFailed;
	}

	VkPresentInfoKHR PresentInfo = {};
	PresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	PresentInfo.waitSemaphoreCount = 1;
	PresentInfo.pWaitSemaphores = SignalSemaphores;
	PresentInfo.swapchainCount = 1;
	PresentInfo.pSwapchains = { GetSwapChainHandle() };
	PresentInfo.pImageIndices = &ImageIndex;
	PresentInfo.pResults = nullptr;

	VkResult QueuePresentResult = vkQueuePresentKHR(PresentQueueHandle, &PresentInfo);

	if (QueuePresentResult == VK_ERROR_OUT_OF_DATE_KHR || QueuePresentResult == VK_SUBOPTIMAL_KHR || mRequestFrameBufferResizing)
	{
		mRequestFrameBufferResizing = false;
		ErrorCode = EDrawFrameErrorCode::SwapChainRecreationRequired;
	}

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % MaxFramesInFlight;

	return ErrorCode;
}

void SwapChainHandler::Cleanup(const VkDevice* Device)
{
	GetRenderPassManager()->CleanUp(*Device, &mCommandPool);
	//GetPipelineSystem()->CleanUp(*Device);
	GetMemoryManager()->CleanUp(*Device);

	for (int i = int(GetSwapChainImageViews()->size()) - 1; i >= 0; i--)
	{
		vkDestroyImageView(*Device, (*GetSwapChainImageViews())[i], nullptr);
	}

	//for (auto iter = mDescriptorPools.rbegin(); iter != mDescriptorPools.rend(); ++iter)
	//{
	//	vkDestroyDescriptorPool(*Device, iter->second, nullptr);
	//	mDescriptorPools.erase(iter->first);
	//}
	//mDescriptorPools.clear();

	vkDestroySwapchainKHR(*Device, mSwapChainHandle, nullptr);
}

void SwapChainHandler::Destroy(const VkDevice* Device)
{
	Cleanup(Device);

	DestroySemaphoreArray(*Device, mImageAvailableSemaphores);
	DestroySemaphoreArray(*Device, mRenderFinishedSemaphores);
	DestroyFenceArray(*Device, mInFlightFences);

	GetRenderPassManager()->Destroy(*Device, GetCommandPool());
	//GetPipelineSystem()->Destroy(*Device);
	GetMemoryManager()->Destroy(*Device);

	vkDestroyCommandPool(*Device, *GetCommandPool(), nullptr);
	mCommandPool = VK_NULL_HANDLE;
}

void SwapChainHandler::DestroySemaphoreArray(const VkDevice& Device, std::vector<VkSemaphore>& Array)
{
	for (int i = static_cast<int>(Array.size()) - 1; i >= 0; i--)
	{
		vkDestroySemaphore(Device, Array[i], nullptr);
	}
	Array.erase(Array.begin(), Array.end());
}

void SwapChainHandler::DestroyFenceArray(const VkDevice& Device, std::vector<VkFence>& Array)
{
	for (int i = static_cast<int>(Array.size()) - 1; i >= 0; i--)
	{
		vkDestroyFence(Device, Array[i], nullptr);
	}

	Array.erase(Array.begin(), Array.end());
}


bool SwapChainHandler::IsAdequate(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails SupportDetails = QuerySwapChainSupport(Device, Surface);
	return !SupportDetails.mFormats.empty() && !SupportDetails.mPresentModes.empty();
}

VkSurfaceFormatKHR SwapChainHandler::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats)
{
	if (AvailableFormats.size() == 1 && AvailableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& AvailableFormat : AvailableFormats)
	{
		if (AvailableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && AvailableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return AvailableFormat;
		}
	}

	return AvailableFormats[0];
}

VkPresentModeKHR SwapChainHandler::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> AvailablePresentModes)
{
	VkPresentModeKHR BestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto& AvailablePresentMode : AvailablePresentModes)
	{
		if (AvailablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return AvailablePresentMode;
		}
		else if (AvailablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			BestMode = AvailablePresentMode;
		}
	}
	return BestMode;
}

VkExtent2D SwapChainHandler::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& mCapabilities)
{

	// todo: windows.h has max and min macro, there fore using std::min and std::max is colliding with these macros, 
#undef min
#undef max
	if (mCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return mCapabilities.currentExtent;
	}
	else
	{
		VkExtent2D ActualExtent;

		ActualExtent.width = std::max(mCapabilities.minImageExtent.width, std::min(mCapabilities.maxImageExtent.width, GetActualSwapChainExtent().width));
		ActualExtent.height = std::max(mCapabilities.minImageExtent.height, std::min(mCapabilities.maxImageExtent.height, GetActualSwapChainExtent().height));

		return ActualExtent;
	}
}

const SwapChainSupportDetails SwapChainHandler::QuerySwapChainSupport(const VkPhysicalDevice& Device, const VkSurfaceKHR& Surface) const
{
	SwapChainSupportDetails Details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device, Surface, &Details.mCapabilities);

	uint32_t FormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, nullptr);

	if (FormatCount != 0)
	{
		Details.mFormats.resize(FormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(Device, Surface, &FormatCount, Details.mFormats.data());
	}

	uint32_t PresentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, nullptr);

	if (PresentModeCount != 0)
	{
		Details.mPresentModes.resize(PresentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(Device, Surface, &PresentModeCount, Details.mPresentModes.data());
	}

	return Details;
}

std::vector<VkImage>* SwapChainHandler::GetSwapChainImages()
{
	return &mSwapChainImages;
}

const VkSwapchainKHR* SwapChainHandler::GetSwapChainHandle()
{
	return &mSwapChainHandle;
}

const VkFormat SwapChainHandler::GetSwapChainImageFormat() const
{
	return mSwapChainImageFormat;
}

void SwapChainHandler::SetActualSwapChainExtent(VkExtent2D Extent)
{
	mSwapChainActualExtent = Extent;
}

const VkExtent2D SwapChainHandler::GetActualSwapChainExtent() const
{
	return mSwapChainActualExtent;
}

const VkExtent2D SwapChainHandler::GetSwapChainExtent() const
{
	return mSwapChainExtent;
}

const std::vector<VkImageView>* SwapChainHandler::GetSwapChainImageViews() const
{
	return &mSwapChainImageViews;
}

RenderPassManager* const SwapChainHandler::GetRenderPassManager() const
{
	return mRenderPassManager.get();
}

//PipelineSystem* const SwapChainHandler::GetPipelineSystem() const
//{
//	return mPipelineSystem.get();
//}

MemoryManager* const SwapChainHandler::GetMemoryManager() const
{
	return mMemoryManager.get();
}

//const VkDescriptorPool* const SwapChainHandler::GetMainDescriptorPool()
//{
//	return GetDescriptorPool("main");
//}

//const VkDescriptorPool* const SwapChainHandler::GetDescriptorPool(DescriptorPoolID ID)
//{
//	return &mDescriptorPools[ID];
//}

const VkCommandPool* const SwapChainHandler::GetCommandPool() const
{
	return &mCommandPool;
}