#ifndef LOGGER_VULKAN_H
#define LOGGER_VULKAN_H

#include <string>

#include "Logger.h"
#include "vulkan/vulkan.h"

#define LOGDOMAIN_RENDERER_VULKAN "Renderer/Vulkan"

class Logger_Vulkan : public Logger
{
public:

	void InitiateDebugMessenger(const VkInstance& Instance);
	VkResult CreateDebugUtilsMessenger(VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo, const VkAllocationCallbacks* Allocator, VkDebugUtilsMessengerEXT* DebugMessenger);
	void DestroyDebugUtilsMessenger(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* Allocator);
	void Cleanup(const VkInstance& Instance);

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData);

private:

	VkDebugUtilsMessengerEXT DebugMessengerHandle;

	static std::string GetPartLogDomainByMessageTypeFlag(VkDebugUtilsMessageTypeFlagsEXT MessageType);
	static LogType MapSeverityFlagBitToLogType(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity);
};

#define LogVk(LogType, Verbosity, Message, ...) LogV(LogType, LOGDOMAIN_RENDERER_VULKAN, Verbosity, Message, __VA_ARGS__)

#endif