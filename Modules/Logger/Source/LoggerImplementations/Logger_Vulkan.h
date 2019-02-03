#ifndef LOGGER_VULKAN_H
#define LOGGER_VULKAN_H

#include <string>
#include <vector>

#include "Logger_Impl.h"
#include "vulkan/vulkan.h"

#include "LogSystemUtilities.h"

class Logger_Vulkan : public Logger_Impl
{
public:

	Logger_Vulkan();
	void InitiateDebugMessenger(const VkInstance& Instance);

	void AddValidationLayer(const char* ValidationLayerName);
	void EnableValidationLayers(bool Enable);
	bool AreValidationLayersEnabled() const;
	const std::vector<const char*>& GetValidationLayers() const;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity, VkDebugUtilsMessageTypeFlagsEXT MessageType, const VkDebugUtilsMessengerCallbackDataEXT* CallbackData, void* UserData);

private:

	bool ValidationLayersEnabled;
	std::vector<const char*> ValidationLayers;
	VkDebugUtilsMessengerEXT DebugMessengerHandle;

	VkResult CreateDebugUtilsMessenger(VkInstance Instance, const VkDebugUtilsMessengerCreateInfoEXT* CreateInfo, const VkAllocationCallbacks* Allocator, VkDebugUtilsMessengerEXT* DebugMessenger);
	void DestroyDebugUtilsMessenger(VkInstance Instance, VkDebugUtilsMessengerEXT DebugMessenger, const VkAllocationCallbacks* Allocator);
	void Cleanup(const VkInstance& Instance);

	static std::string GetPartLogDomainByMessageTypeFlag(VkDebugUtilsMessageTypeFlagsEXT MessageType);
	static LogType MapSeverityFlagBitToLogType(VkDebugUtilsMessageSeverityFlagBitsEXT MessageSeverity);
};

#endif