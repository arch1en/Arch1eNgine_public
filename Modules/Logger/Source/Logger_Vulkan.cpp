#include "Logger_Vulkan.h"


void Logger_Vulkan::InitiateDebugMessenger(const VkInstance& Instance)
{
	VkDebugUtilsMessengerCreateInfoEXT CreateInfo = {};

	CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	CreateInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT 
		|	VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT 
		|	VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	CreateInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT 
		|	VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT 
		|	VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	CreateInfo.pfnUserCallback = Logger_Vulkan::DebugCallback;
	CreateInfo.pUserData = nullptr;

	if (CreateDebugUtilsMessenger(Instance, &CreateInfo, nullptr, &DebugMessengerHandle) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Debug Utils Messenger creation failed !");
	}
}

VkResult Logger_Vulkan::CreateDebugUtilsMessenger(VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo, const VkAllocationCallbacks* Allocator, VkDebugUtilsMessengerEXT* DebugMessenger)
{
	auto Func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkCreateDebugUtilsMessengerEXT");
	if (Func != nullptr)
	{
		return Func(Instance, CreateInfo, Allocator, DebugMessenger);
	}
	
	return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void Logger_Vulkan::DestroyDebugUtilsMessenger(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* Allocator)
{
	auto Func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(Instance, "vkDestroyDebugUtilsMessengerEXT");
	if (Func != nullptr)
	{
		Func(Instance, DebugMessenger, Allocator);
	}
}

VkBool32 Logger_Vulkan::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData)
{
	char LogDomain[256] = LOGDOMAIN_RENDERER_VULKAN;
	char Delimiter[2] = "/";
	strcat_s(LogDomain, strlen(LogDomain), (Delimiter + GetPartLogDomainByMessageTypeFlag(MessageType)).c_str());
	LogV(MapSeverityFlagBitToLogType(MessageSeverity), LogDomain, 0, CallbackData->pMessage);

	return VK_FALSE;
}

void Logger_Vulkan::Cleanup(const VkInstance& Instance)
{
	DestroyDebugUtilsMessenger(Instance, DebugMessengerHandle, nullptr);
}

std::string Logger_Vulkan::GetPartLogDomainByMessageTypeFlag(VkDebugUtilsMessageTypeFlagsEXT MessageType)
{
	if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) return "General";
	if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) return "Validation";
	if (MessageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) return "Performance";

	return "Unknown";
}

LogType Logger_Vulkan::MapSeverityFlagBitToLogType(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity)
{
	if (MessageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) return LogType::Notice;
	if (MessageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) return LogType::Warning;
	if (MessageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) return LogType::Error;

	return LogType::Notice;
}