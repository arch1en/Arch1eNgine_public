#include "RenderingInstance_SDL2_Vulkan.h"

#include <vector>

#include "SDL.h"
#include "SDL_vulkan.h"

#include "RenderingSystem/RenderingSystemUtilities.h"
#include "Logger_Vulkan.h"

RenderingInstance_SDL2_Vulkan::RenderingInstance_SDL2_Vulkan()
#ifdef DEBUG
	: EnableValidationLayers{ true }
#elif
	: EnableValidationLayers{ false }
#endif
{

}

RenderingInstance_SDL2_Vulkan::~RenderingInstance_SDL2_Vulkan()
{
	vkDestroyInstance(InstanceHandle, nullptr);
}

bool RenderingInstance_SDL2_Vulkan::CreateVulkanInstance(void* WindowHandle)
{
	VkApplicationInfo ApplicationInfo = {};
	ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	ApplicationInfo.pApplicationName = "Arch1eNgine";
	ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.pEngineName = "No Engine";
	ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	ApplicationInfo.apiVersion = VK_API_VERSION_1_0;

	// Extensions
	std::vector<const char*> Extensions =
	{
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME
	};

	if (!AddExtensions(WindowHandle, Extensions))
	{
		return false;
	}
	// ~Extensions

	// Validation Layers
	const std::vector<const char*> ValidationLayers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	if (EnableValidationLayers)
	{
		std::vector<VkLayerProperties> AvailableDesiredLayers = CheckValidationLayersAvailability(ValidationLayers);

		if (AvailableDesiredLayers.size() != ValidationLayers.size())
		{
			LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Avaliable validation layers count do not match with desired validation layers.");
			return false;
		}
	}

	// ~Validation Layers

	VkInstanceCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	CreateInfo.pApplicationInfo = &ApplicationInfo;
	CreateInfo.enabledExtensionCount = static_cast<uint32_t>(Extensions.size());
	CreateInfo.ppEnabledExtensionNames = Extensions.data();
	if (EnableValidationLayers)
	{
		CreateInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
		CreateInfo.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		CreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateInstance(&CreateInfo, nullptr, &InstanceHandle) != VK_SUCCESS)
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance creation failed.");
		return false;
	}

	// @todo [Urgent] Decide what to do with the logger class (maybe make it classless ?)
	Logger_Vulkan::GetInstance().InitiateDebugMessenger(InstanceHandle,)

	return true;
}

bool RenderingInstance_SDL2_Vulkan::AddExtensions(void* WindowHandle, std::vector<const char*>& Extensions)
{
	unsigned int ExtensionsCount = 0;

	SDL_Window* Window = static_cast<SDL_Window*>(WindowHandle);

	if (!SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionsCount, nullptr))
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance extensions receivement failed.");
		return false;
	}

	size_t AdditionalExtensionsCount = Extensions.size();

	Extensions.resize(AdditionalExtensionsCount + ExtensionsCount);

	if (!SDL_Vulkan_GetInstanceExtensions(Window, &ExtensionsCount, Extensions.data() + AdditionalExtensionsCount))
	{
		LogV(LogType::Error, LOGDOMAIN_RENDERER_VULKAN, 0, "Vulkan instance extensions addition failed.");
		return false;
	}

	return true;
}

std::vector<VkExtensionProperties> RenderingInstance_SDL2_Vulkan::GetAvailableExtensions()
{
	uint32_t ExtensionCount = 0;

	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, nullptr);
	std::vector<VkExtensionProperties> Extensions(ExtensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &ExtensionCount, Extensions.data());

	return Extensions;
}

std::vector<VkLayerProperties> RenderingInstance_SDL2_Vulkan::CheckValidationLayersAvailability(const std::vector<const char*> DesiredLayers)
{
	uint32_t LayerCount = 0;
	vkEnumerateInstanceLayerProperties(&LayerCount, nullptr);
	std::vector<VkLayerProperties> AvailableLayers(LayerCount);
	vkEnumerateInstanceLayerProperties(&LayerCount, AvailableLayers.data());

	std::vector<VkLayerProperties> AvailableDesiredLayers;

	for (auto LayerName : DesiredLayers)
	{
		for (const auto& LayerProperties : AvailableLayers)
		{
			if (strcmp(LayerName, LayerProperties.layerName) == 0)
			{
				AvailableDesiredLayers.push_back(LayerProperties);
			}
		}
	}

	return AvailableDesiredLayers;
}

void RenderingInstance_SDL2_Vulkan::Initialize()
{
}

const std::string RenderingInstance_SDL2_Vulkan::GetImplementationType() const
{
	return "SDL2/Vulkan";
}

void * RenderingInstance_SDL2_Vulkan::GetRenderingInstanceHandle() const
{
	return nullptr;
}

void RenderingInstance_SDL2_Vulkan::SetRenderingInstanceHandle(void * Handle)
{
}

void RenderingInstance_SDL2_Vulkan::SetSwapInterval(int Interval)
{
}

void RenderingInstance_SDL2_Vulkan::SetClearColor(Vector4<float> ClearColor)
{
}

void RenderingInstance_SDL2_Vulkan::ClearInstance(I::RenderingInstanceProperties_ClearColor_Impl Properties)
{
}