#include "EngineEditor.h"
#include "examples/imgui_impl_vulkan.h"
#include "examples/imgui_impl_sdl.h"
#include "WindowSystem/Implementations/Window_SDL2.h"
#include "WindowSystem/WindowSystem.h"
#include "RenderingSystem/RenderingSystem.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
#include "FileSystem/FileSystem.h"

EngineEditor::EngineEditor(WindowSystem* const WindowSystem, RenderingSystem* const RenderingSystem)
	: mWindowSystem{WindowSystem}
	, mRenderingSystem{RenderingSystem}
	, mContext{ nullptr }
{}

void EngineEditor::Initiate()
{
	mContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(mContext);
	ImGui::StyleColorsDark();

	ConfigureVulkan();
	CreateVulkanRenderPass();
}

void EngineEditor::ConfigureVulkan()
{

}

void EngineEditor::CreateVulkanRenderPass()
{
	ImGui_ImplSDL2_InitForVulkan(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()));

	if (mRenderingSystem->GetRenderingInstance()->GetImplementationType().compare("SDL2/Vulkan") == 0)
	{
		RenderingInstance_SDL2_Vulkan* RenderingInstance = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingSystem->GetRenderingInstance());

		const VkDevice* LogicalDeviceHandle = RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle();
		SwapChainHandler* SwapChainHandler = RenderingInstance->GetSwapChainHandler();
		
		const DescriptorPoolID ID = "engine_editor";

		{
			// Color Attachment
			VkAttachmentDescription AttachmentDescription = {};

			AttachmentDescription.format = SwapChainHandler->GetSwapChainImageFormat();
			AttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
			AttachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			AttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			AttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			AttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			AttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			AttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference AttachmentReference = {};

			AttachmentReference.attachment = 0;
			AttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkSubpassDescription SubpassDescription = {};

			SubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			SubpassDescription.colorAttachmentCount = 1;
			SubpassDescription.pColorAttachments = &AttachmentReference;

			VkSubpassDependency SubpassDependency = {};

			SubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			SubpassDependency.dstSubpass = 0;
			SubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			SubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			SubpassDependency.srcAccessMask = 0;
			SubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			VkRenderPassCreateInfo RenderPassCreateInfo = {};

			RenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassCreateInfo.attachmentCount = 1;
			RenderPassCreateInfo.pAttachments = &AttachmentDescription;
			RenderPassCreateInfo.subpassCount = 1;
			RenderPassCreateInfo.pSubpasses = &SubpassDescription;
			RenderPassCreateInfo.dependencyCount = 1;
			RenderPassCreateInfo.pDependencies = &SubpassDependency;

			// RenderPass. (Needs to be created before pipeline. Needs to be created after swap chain.)
			SwapChainHandler->GetRenderPassManager()->CreateRenderPass
			(
				ID,
				LogicalDeviceHandle,
				RenderPassCreateInfo
			);

		}

		const VkRenderPass& RenderPassHandle = SwapChainHandler->GetRenderPassManager()->GetRenderPassData(ID)->mRenderPassHandle;

		// Framebuffer.

		SwapChainHandler->GetRenderPassManager()->CreateFramebuffers
		(
			ID,
			LogicalDeviceHandle,
			&RenderPassHandle,
			SwapChainHandler->GetSwapChainImageViews(),
			&SwapChainHandler->GetSwapChainExtent()
		);
		// No uniform buffers ?

		// Pipeline creation.
		{
			PipelineSystemCreationInfo PipelineCreationInfo = {};

			PipelineCreationInfo.mLogicalDevice = LogicalDeviceHandle;
			PipelineCreationInfo.mImageFormat = SwapChainHandler->GetSwapChainImageFormat();
			PipelineCreationInfo.mViewportExtent = SwapChainHandler->GetSwapChainExtent();
			PipelineCreationInfo.mRenderPassHandle = &RenderPassHandle;
			PipelineCreationInfo.mMemoryManager = SwapChainHandler->GetMemoryManager();

			PipelineCreationInfo.mShaderCode_Vertex = FileSystem::RetrieveBinaryDataFromFile("EngineEditor", "Shaders/Main.vert.spv");
			PipelineCreationInfo.mShaderCode_Fragment = FileSystem::RetrieveBinaryDataFromFile("EngineEditor", "Shaders/Main.frag.spv");

			SwapChainHandler->GetRenderPassManager()->GetPipelineSystem()->CreateGraphicsPipeline(PipelineCreationInfo);

			// Adding Descriptor Pool. Descriptor pool can be done during vulkan initialization process (before rendering and swapchain even).
			DescriptorPoolCreateInfo DescriptorPoolCI = {};

			DescriptorPoolCI.mLogicalDevice = LogicalDeviceHandle;

			DescriptorPoolCI.mPoolSizes =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			DescriptorPoolCI.mPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			DescriptorPoolCI.mPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			DescriptorPoolCI.mPoolCreateInfo.maxSets = 1000 * static_cast<uint32_t>(DescriptorPoolCI.mPoolSizes.size());
			DescriptorPoolCI.mPoolCreateInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolCI.mPoolSizes.size());
			DescriptorPoolCI.mPoolCreateInfo.pPoolSizes = DescriptorPoolCI.mPoolSizes.data();

			DescriptorPoolCI.mDescriptorPoolID = ID;

			SwapChainHandler->GetRenderPassManager()->GetPipelineSystem()->CreateDescriptorPool(DescriptorPoolCI);
		}


		// Fonts
		{

		}

		// Adding Image View
		//{
		//	VkImageViewCreateInfo info = {};
		//	info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		//	info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		//	info.format = SwapChainHandler->GetSwapChainImageFormat();
		//	info.components.r = VK_COMPONENT_SWIZZLE_R;
		//	info.components.g = VK_COMPONENT_SWIZZLE_G;
		//	info.components.b = VK_COMPONENT_SWIZZLE_B;
		//	info.components.a = VK_COMPONENT_SWIZZLE_A;
		//	VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		//	info.subresourceRange = image_range;
		//	for (uint32_t i = 0; i < SwapChainHandler->GetSwapChainImages()->size(); i++)
		//	{
		//		ImGui_ImplVulkanH_Frame* fd = &wd->Frames[i];
		//		info.image = fd->Backbuffer;
		//		err = vkCreateImageView(device, &info, allocator, &fd->BackbufferView);
		//		check_vk_result(err);
		//	}
		//}
		// ~Adding Image View

		RenderingInstance->GetOnResizeCanvasEvent().Bind<EngineEditor, &EngineEditor::OnResizeCanvasVulkan>(this);

		// Allocating Fonts
		{
			const VkCommandPool* CommandPool = RenderingInstance->GetSwapChainHandler()->GetCommandPool();
			VkCommandBuffer CommandBuffer;

			VkCommandBufferAllocateInfo AllocInfo = {};
			AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			AllocInfo.commandPool = *CommandPool;
			AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			AllocInfo.commandBufferCount = 1;

			// [Todo] Needs deallocation.
			if (vkAllocateCommandBuffers(*LogicalDeviceHandle, &AllocInfo, &CommandBuffer) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Command buffer allocation failed.");
			}

			if (vkResetCommandPool(*RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle(), *CommandPool, 0) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Command pool resetting failed!");
				return;
			}

			VkCommandBufferBeginInfo BeginInfo = {};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			if (vkBeginCommandBuffer(CommandBuffer, &BeginInfo) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Command buffer beginning failed!");
				return;
			}

			ImGui_ImplVulkan_CreateFontsTexture(CommandBuffer);

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &CommandBuffer;

			if (vkEndCommandBuffer(CommandBuffer) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Command buffer ending failed!");
				return;
			}

			if (vkQueueSubmit(RenderingInstance->GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData()->QueueHandle, 1, &end_info, VK_NULL_HANDLE) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Queue submission failed!");
				return;
			}

			if (vkDeviceWaitIdle(*LogicalDeviceHandle) != VK_SUCCESS)
			{
				LogVk(LogType::Error, 0, "Device wait idle failed!");
				return;
			}
			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

		// ~Allocating Fonts


	}
}

void EngineEditor::OnResizeCanvasVulkan(int Width, int Height)
{
	RenderingInstance_SDL2_Vulkan* RenderingInstance = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingSystem->GetRenderingInstance());

	// Pass Vulkan data to ImGUI.

	const VkInstance* Instance = static_cast<VkInstance*>(RenderingInstance->GetRenderingInstanceHandle());
	const VkDevice* LogicalDeviceHandle = RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle();
	const VkPhysicalDevice& PhysicalDeviceHandle = RenderingInstance->GetDeviceHandler()->GetStrongestPhysicalDeviceProperties()->DeviceHandle;
	const VkSurfaceKHR Surface = *RenderingInstance->GetSurfaceHandler()->GetMainSurface()->GetHandle();
	const QueueFamilyData* QFData = RenderingInstance->GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData();

	SwapChainSupportDetails SurfaceDetails = RenderingInstance->GetSwapChainHandler()->QuerySwapChainSupport(PhysicalDeviceHandle, Surface);

	ImGui_ImplVulkanH_Window WindowData = {};

	WindowData.Width = Width;
	WindowData.Height = Height;
	WindowData.Swapchain = *RenderingInstance->GetSwapChainHandler()->GetSwapChainHandle();
	WindowData.Surface = Surface;
	WindowData.SurfaceFormat = SurfaceDetails.mFormats[0];
	WindowData.PresentMode = SurfaceDetails.mPresentModes[0];
	WindowData.RenderPass = *RenderingInstance->GetSwapChainHandler()->GetRenderPassManager()->GetRenderPassHandle("engine_editor");
	WindowData.FrameIndex;             // Curren
	WindowData.ImageCount;             // Number
	WindowData.SemaphoreIndex;         // Curren
	WindowData.Frames;
	WindowData.FrameSemaphores;

	ImGui_ImplVulkan_SetMinImageCount(2);
	ImGui_ImplVulkanH_CreateWindow(*Instance, PhysicalDeviceHandle, *LogicalDeviceHandle, &WindowData, QFData->FamilyIndex, nullptr, Width, Height, 2);
}

void EngineEditor::InitiateInput()
{
	mContext->IO = ImGui::GetIO();
}

void EngineEditor::Render(Vector2<> DisplaySize)
{
	mContext->IO.DisplaySize = { DisplaySize.x, DisplaySize.y };

	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(static_cast<Window_SDL2*>(mWindowSystem->GetMainWindow())->GetWindowHandle()));

	ImGui::NewFrame();
	ImGui::Render();

	FrameRender();
	FramePresent();
}

void EngineEditor::FrameRender()
{
	//VkResult err;

	//VkSemaphore image_acquired_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
	//VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	//err = vkAcquireNextImageKHR(g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
	//check_vk_result(err);

	//ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
	//{
	//	err = vkWaitForFences(g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
	//	check_vk_result(err);

	//	err = vkResetFences(g_Device, 1, &fd->Fence);
	//	check_vk_result(err);
	//}
	//{
	//	err = vkResetCommandPool(g_Device, fd->CommandPool, 0);
	//	check_vk_result(err);
	//	VkCommandBufferBeginInfo info = {};
	//	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//	info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	//	err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
	//	check_vk_result(err);
	//}
	//{
	//	VkRenderPassBeginInfo info = {};
	//	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	//	info.renderPass = wd->RenderPass;
	//	info.framebuffer = fd->Framebuffer;
	//	info.renderArea.extent.width = wd->Width;
	//	info.renderArea.extent.height = wd->Height;
	//	info.clearValueCount = 1;
	//	info.pClearValues = &wd->ClearValue;
	//	vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
	//}

	//// Record Imgui Draw Data and draw funcs into command buffer
	//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

	//// Submit command buffer
	//vkCmdEndRenderPass(fd->CommandBuffer);
	//{
	//	VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	//	VkSubmitInfo info = {};
	//	info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	//	info.waitSemaphoreCount = 1;
	//	info.pWaitSemaphores = &image_acquired_semaphore;
	//	info.pWaitDstStageMask = &wait_stage;
	//	info.commandBufferCount = 1;
	//	info.pCommandBuffers = &fd->CommandBuffer;
	//	info.signalSemaphoreCount = 1;
	//	info.pSignalSemaphores = &render_complete_semaphore;

	//	err = vkEndCommandBuffer(fd->CommandBuffer);
	//	check_vk_result(err);
	//	err = vkQueueSubmit(g_Queue, 1, &info, fd->Fence);
	//	check_vk_result(err);
	//}

}

void EngineEditor::FramePresent()
{
	//VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
	//VkPresentInfoKHR info = {};
	//info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	//info.waitSemaphoreCount = 1;
	//info.pWaitSemaphores = &render_complete_semaphore;
	//info.swapchainCount = 1;
	//info.pSwapchains = &wd->Swapchain;
	//info.pImageIndices = &wd->FrameIndex;
	//VkResult err = vkQueuePresentKHR(g_Queue, &info);
	//check_vk_result(err);
	//wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores

}