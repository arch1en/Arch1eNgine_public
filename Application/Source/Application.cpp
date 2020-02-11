
#include "Application.h"

#include "Configurators/Window_RenderingInstance_Configurator.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
#include "RenderingSystem/Vulkan/SwapChainHandler.h"
#include "RenderingSystem/Vulkan/RenderPassManager.h"
#include "examples/imgui_impl_vulkan.h"
#include "examples/imgui_impl_sdl.h"

#include "SDL.h"

bool Application::Initiate()
{
	mWindowSystem = std::make_unique<WindowSystem>();
	mRenderingSystem = std::make_unique<RenderingSystem>();
	mEngineEditor = std::make_unique<EngineEditor>();

	// Renderer
	RenderingInstanceType Type = RenderingInstanceType::Vulkan;

	CreateRenderer(Type);
	CreateApplicationWindow(Type);

	Configurator::Window_RenderingInstance WRIConfigurator(mWindowSystem->GetMainWindow(), mRenderingSystem->GetRenderingInstance());
	WRIConfigurator.Configure();

	// EngineEditor
	Configurator::Renderer_EngineEditor REEConfigurator(mWindowSystem.get(), mRenderingSystem.get(), mEngineEditor.get());
	REEConfigurator.Configure();

	MainLoop();

	return false;
}

void Application::CreateApplicationWindow(RenderingInstanceType Type)
{
	WindowProperties Properties;

	Properties.Title = "Arch1eNgine";
	Properties.WindowPosition.Mode = WindowPositionMode::Custom;
	Properties.WindowPosition.PositionX = 100;
	Properties.WindowPosition.PositionY = 100;
	Properties.Width = 640;
	Properties.Height = 480;
	Properties.RendererType = static_cast<WindowRendererType>(Type);
	Properties.Resizeable = true;

	mWindowSystem->CreateNewWindow(Properties);
}

void Application::CreateRenderer(RenderingInstanceType Type)
{
	RenderingInstanceProperties Properties;

	Properties.Type = RenderingInstanceType::Vulkan;

	mRenderingSystem->CreateInstance(Properties);
}

void Application::MainLoop()
{
	while (mWindowSystem->IsMainWindowAvailable())
	{
		LogicLoop();
		RenderingLoop();
	}
}

void Application::LogicLoop()
{
	SDL_Event Event;
	
	while(SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
		{
			mWindowSystem->DestroyAllWindows();
			break;
		}
		case SDL_WINDOWEVENT_RESIZED:
		{
			int Width, Height;
			SDL_GetWindowSize(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()), &Width, &Height);
			mRenderingSystem->GetRenderingInstance()->ResizeCanvas(Width,Height);
			break;
		}
		}
	}
	// [todo] Made for SDL for now. Some kind of API binding system would be good to do here.

}

void Application::RenderingLoop()
{
	int Width, Height;
	SDL_GetWindowSize(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()), &Width, &Height);
	mEngineEditor->Render(Vector2<>(Width, Height));
	mRenderingSystem->GetRenderingInstance()->RenderLoop();

	//mRenderingSystem->GetRenderingInstance()->SetClearColor(Vector4<float>(0.f, 0.f, 0.f, 1.f));

//	I::RenderingInstanceProperties_ClearColor_Impl Properties;

//	Properties.ClearColorBuffer = true;

//	mRenderingSystem->GetRenderingInstance()->ClearInstance(Properties);

	//mWindowSystem->GetMainWindow()->SwapBuffers();
}

void Configurator::Renderer_EngineEditor::Configure()
{
	mEngineEditor->Initiate();

	ImGui_ImplSDL2_InitForVulkan(static_cast<SDL_Window*>(mWindowSystem->GetMainWindow()->GetWindowHandle()));

	if (mRenderingSystem->GetRenderingInstance()->GetImplementationType().compare("SDL2/Vulkan") == 0)
	{
		RenderingInstance_SDL2_Vulkan* RenderingInstance = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingSystem->GetRenderingInstance());
		
		// Pass Vulkan data to ImGUI.

		const VkDevice* LogicalDeviceHandle = RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle();

		// Adding Descriptor Pool.
		{
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

			const DescriptorPoolID ID = "engine_editor";

			DescriptorPoolCI.mDescriptorPoolID = ID;

			RenderingInstance->GetSwapChainHandler()->CreateDescriptorPool(DescriptorPoolCI);
		}
		// ~Adding Descriptor Pool.

		// Adding Render Pass.
		{
			RenderPassCreateInfo RenderPassCI = {};

			RenderPassCI.mLogicalDevice = RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle();
			RenderPassCI.mRenderPassID = "engine_editor";

			RenderPassCI.mAttachmentDescriptrion.format = RenderingInstance->GetSwapChainHandler()->GetSwapChainImageFormat();
			RenderPassCI.mAttachmentDescriptrion.samples = VK_SAMPLE_COUNT_1_BIT;
			RenderPassCI.mAttachmentDescriptrion.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			RenderPassCI.mAttachmentDescriptrion.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			RenderPassCI.mAttachmentDescriptrion.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			RenderPassCI.mAttachmentDescriptrion.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			RenderPassCI.mAttachmentDescriptrion.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			RenderPassCI.mAttachmentDescriptrion.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			RenderPassCI.mAttachmentReference.attachment = 0;
			RenderPassCI.mAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			RenderPassCI.mSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			RenderPassCI.mSubpassDescription.colorAttachmentCount = 1;
			RenderPassCI.mSubpassDescription.pColorAttachments = &RenderPassCI.mAttachmentReference;

			RenderPassCI.mSubpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			RenderPassCI.mSubpassDependency.dstSubpass = 0;
			RenderPassCI.mSubpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			RenderPassCI.mSubpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			RenderPassCI.mSubpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			RenderPassCI.mSubpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

			RenderPassCI.mRenderPassCreateInfo = {};
			RenderPassCI.mRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			RenderPassCI.mRenderPassCreateInfo.attachmentCount = 1;
			RenderPassCI.mRenderPassCreateInfo.pAttachments = &RenderPassCI.mAttachmentDescriptrion;
			RenderPassCI.mRenderPassCreateInfo.subpassCount = 1;
			RenderPassCI.mRenderPassCreateInfo.pSubpasses = &RenderPassCI.mSubpassDescription;
			RenderPassCI.mRenderPassCreateInfo.dependencyCount = 1;
			RenderPassCI.mRenderPassCreateInfo.pDependencies = &RenderPassCI.mSubpassDependency;

			RenderingInstance->GetSwapChainHandler()->GetRenderPassManager()->CreateRenderPass(RenderPassCI);
		}
		// ~Adding Render Pass.

		ImGui_ImplVulkan_InitInfo Info = {};
		Info.Instance = *static_cast<VkInstance*>(RenderingInstance->GetRenderingInstanceHandle());
		Info.PhysicalDevice = RenderingInstance->GetDeviceHandler()->GetPhysicalDevicesProperties()->at(0).DeviceHandle;
		Info.Device = *RenderingInstance->GetDeviceHandler()->GetLogicalDeviceHandle();
		Info.QueueFamily = RenderingInstance->GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData()->FamilyIndex;
		Info.Queue = RenderingInstance->GetDeviceHandler()->GetQueueFamilyHandler()->GetPresentationSuitableQueueFamilyData()->QueueHandle;
		Info.PipelineCache = nullptr;
		Info.DescriptorPool = *RenderingInstance->GetSwapChainHandler()->GetDescriptorPool("engine_editor");
		Info.MinImageCount = 2;
		Info.ImageCount = Info.MinImageCount;
		Info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		Info.Allocator = nullptr;

		ImGui_ImplVulkan_Init(&Info, *RenderingInstance->GetSwapChainHandler()->GetRenderPassManager()->GetMainRenderPassHandle());

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
