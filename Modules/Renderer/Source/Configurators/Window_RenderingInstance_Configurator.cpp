#include "Window_RenderingInstance_Configurator.h"

#include <SDL.h>

#include "LogSystem.h"
#include "WindowSystem/Implementations/Window_SDL2.h"
#include "WindowSystem/Implementations/Window_SDL2_Vulkan.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_OpenGL.h"
#include "RenderingSystem/Instance/RenderingInstance_SDL2_Vulkan.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceUtilities.h"
#include "RenderingSystem/Vulkan/Surface/SurfaceHandler.h"
#include "RenderingSystem/Vulkan/DeviceHandler.h"
#include "RenderingSystem/Vulkan/Surface/Implementations/Surface_Base.h"
#include "RenderingSystem/Vulkan/PipelineSystem/RenderPassManager.h"
#include "vulkan/vulkan.h"


Configurator::Window_RenderingInstance::Window_RenderingInstance(I::Window_Impl* Window, I::RenderingInstance_Impl* RenderingInstance)
	: mWindow{Window}
	, mRenderingInstance{RenderingInstance}
{}

void Configurator::Window_RenderingInstance::ConfigureImplementations()
{
	if (mWindow->GetImplementationType().compare("SDL2/Vulkan") == 0)
	{
		// todo: This whole block of code needs to have these method chains shrinked to variable.
		RenderingInstance_SDL2_Vulkan* InstanceVkSDL2 = static_cast<RenderingInstance_SDL2_Vulkan*>(mRenderingInstance);
		Window_SDL2_Vulkan* WindowVkSDL2 = static_cast<Window_SDL2_Vulkan*>(mWindow);

		InstanceVkSDL2->CreateVulkanInstance(WindowVkSDL2->GetWindowHandle());
		InstanceVkSDL2->CreateSurfaceHandler();
		InstanceVkSDL2->CreateDeviceHandler();

		SurfaceHandlerCreationData_SDL SurfaceData = {};
		
		// Creating Window.
		SurfaceData.VulkanInstanceRef = static_cast<VkInstance*>(InstanceVkSDL2->GetRenderingInstanceHandle());

#ifdef ARCHITECTURE_X86
		SurfaceData.TargetSurfaceEnum = TargetSurface::Win32;
#elif ARCHITECTURE_X86_64
		SurfaceData.TargetSurfaceEnum = TargetSurface::Win64;
#endif

		SurfaceData.WindowHandle = static_cast<SDL_Window*>(WindowVkSDL2->GetWindowHandle());
		SurfaceData.WindowInfo = WindowVkSDL2->GetWindowInfo();

		// Surface.
		InstanceVkSDL2->GetSurfaceHandler()->CreateSurface(&SurfaceData);

		// SwapChain.
		InstanceVkSDL2->CreateSwapChainHandler();

		DeviceHandlerCreationInfo CreationInfo;

		CreationInfo.pInstanceHandle = static_cast<VkInstance*>(InstanceVkSDL2->GetRenderingInstanceHandle());
		CreationInfo.pSurfaceHandle = InstanceVkSDL2->GetSurfaceHandler()->GetMainSurface()->GetHandle();
		CreationInfo.pSwapChainHandler = InstanceVkSDL2->GetSwapChainHandler();
		CreationInfo.DesiredDeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		DeviceHandler* pDeviceHandler = InstanceVkSDL2->GetDeviceHandler();
		pDeviceHandler->Initiate(&CreationInfo);

		SwapChainHandlerCreationInfo SwapChainCreationInfo = {};

		const VkDevice* LogicalDevice = pDeviceHandler->GetLogicalDeviceHandle();

		SwapChainCreationInfo.mLogicalDevice = LogicalDevice;
		SwapChainCreationInfo.mPhysicalDevice = &pDeviceHandler->GetPhysicalDevicesProperties()->at(0).DeviceHandle;
		SwapChainCreationInfo.mSurface = InstanceVkSDL2->GetSurfaceHandler()->GetMainSurface()->GetHandle();
		SwapChainCreationInfo.mQueueFamilyHandler = pDeviceHandler->GetQueueFamilyHandler();

		InstanceVkSDL2->GetSwapChainHandler()->CreateSwapChain(SwapChainCreationInfo);

		// RenderPass. (Needs to be created before pipeline. Needs to be created after swap chain.)
		InstanceVkSDL2->CreateRenderPassManager();
		InstanceVkSDL2->GetRenderPassManager()->CreateRenderPass(*LogicalDevice, InstanceVkSDL2->GetSwapChainHandler()->GetSwapChainImageFormat());

		FramebufferCreateInfo FramebufferCreationInfo = {};

		FramebufferCreationInfo.mLogicalDevice = LogicalDevice;
		FramebufferCreationInfo.mRenderPassHandle = InstanceVkSDL2->GetRenderPassManager()->GetRenderPassHandle();
		FramebufferCreationInfo.mSwapChainImageExtent = &InstanceVkSDL2->GetSwapChainHandler()->GetSwapChainExtent();
		FramebufferCreationInfo.mSwapChainImageViews = InstanceVkSDL2->GetSwapChainHandler()->GetSwapChainImageViews();

		InstanceVkSDL2->GetRenderPassManager()->CreateFramebuffers(FramebufferCreationInfo);

		// Pipeline.
		InstanceVkSDL2->CreatePipelineSystem();
		
		PipelineSystemCreationInfo PipelineCreationInfo = {};

		PipelineCreationInfo.mLogicalDevice = LogicalDevice;
		PipelineCreationInfo.mImageFormat = InstanceVkSDL2->GetSwapChainHandler()->GetSwapChainImageFormat();
		PipelineCreationInfo.mViewportExtent = InstanceVkSDL2->GetSwapChainHandler()->GetSwapChainExtent();
		PipelineCreationInfo.mRenderPassHandle = InstanceVkSDL2->GetRenderPassManager()->GetRenderPassHandle();

		InstanceVkSDL2->GetPipelineSystem()->CreateGraphicsPipeline(PipelineCreationInfo);

		// Commands (Command Pool and Command Buffers).

		InstanceVkSDL2->CreateCommandsHandler();

		CommandPoolCreateInfo CommandPoolCI = {};

		CommandPoolCI.mLogicalDevice = LogicalDevice;
		CommandPoolCI.mQueueFamilyData = InstanceVkSDL2->GetDeviceHandler()->GetQueueFamilyHandler()->GetQueueFamilyData();

		InstanceVkSDL2->GetCommandsHandler()->CreateCommandPool(CommandPoolCI);

		RenderPassCommandBufferCreateInfo RenderPassCommandBufferCI = {};

		RenderPassCommandBufferCI.mLogicalDevice = LogicalDevice;
		RenderPassCommandBufferCI.mBufferSize = InstanceVkSDL2->GetRenderPassManager()->GetFramebuffers()->size();
		RenderPassCommandBufferCI.mCommandPool = InstanceVkSDL2->GetCommandsHandler()->GetCommandPool();

		InstanceVkSDL2->GetRenderPassManager()->CreateRenderPassCommandBuffers(RenderPassCommandBufferCI);
		InstanceVkSDL2->CreateSemaphores();
		InstanceVkSDL2->CreateFences();

		return;
	}
	else if (mRenderingInstance->GetImplementationType().compare("SDL2/OpenGL") == 0)
	{
		RenderingInstance_SDL2_OpenGL* InstanceOGLSDL2 = static_cast<RenderingInstance_SDL2_OpenGL*>(mRenderingInstance);

		InstanceOGLSDL2->CreateOpenGLContext(static_cast<SDL_Window*>(mWindow->GetWindowHandle()));
		InstanceOGLSDL2->InitializeGLAD();
		return;
	}

	LogV(LogType::Warning, "Renderer", 0, "%s function is not implemented for Window {%s} and Instance {%s} pair.", __FUNCTION__, mWindow->GetImplementationType().c_str(), mRenderingInstance->GetImplementationType().c_str());
}

void Configurator::Window_RenderingInstance::Invalidate()
{
	mWindow = nullptr;
	mRenderingInstance = nullptr;
}