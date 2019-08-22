#ifndef COMMANDSHANDLER_H
#define COMMANDSHANDLER_H

#include <vulkan/vulkan.h>

#include "RenderingSystem/Vulkan/DeviceHandler.h"

struct CommandPoolCreateInfo
{
    const VkDevice* mLogicalDevice;
	const std::vector<QueueFamilyData>* mQueueFamilyData;
};


class CommandsHandler
{
public:

    void CreateCommandPool(const CommandPoolCreateInfo& CreateInfo);

	const VkCommandPool* GetCommandPool() const;

	void CleanUp(const VkDevice& Device);

private:

	VkCommandPool mCommandPool;
	std::vector<VkCommandBuffer> mCommandBuffers;
};

#endif
