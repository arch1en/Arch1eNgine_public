#include "CommandsHandler.h"

#include "LogSystem.h"

void CommandsHandler::CreateCommandPool(const CommandPoolCreateInfo& CreateInfo)
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

const VkCommandPool* CommandsHandler::GetCommandPool() const
{
	return &mCommandPool;
}

void CommandsHandler::CleanUp(const VkDevice& Device)
{
	vkDestroyCommandPool(Device, mCommandPool, nullptr);
}
