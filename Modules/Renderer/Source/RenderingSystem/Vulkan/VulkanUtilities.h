#ifndef VULKANUTILITIES_H
#define VULKANUTILITIES_H

#include "GeometrySystem/Vertex.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include "vulkan/vulkan_core.h"
#include "Debug/LogSystem.h"

struct BufferData
{
	VkBuffer mBuffer;
	VkDeviceMemory mBufferMemory;
};

struct VertexBufferData
{
	BufferData mBufferData;
	std::vector<Vertex> mVertices;
};

struct IndexBufferData
{
	BufferData mBufferData;
	std::vector<uint16_t> mIndices;
};

// [vulkan][disclaimer] Vulkan has a specification regarding memory alignments.
// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/chap14.html#interfaces-resources-layout
struct UniformBufferObject
{
	alignas(16) glm::mat4 Model;
	alignas(16) glm::mat4 View;
	alignas(16) glm::mat4 Projection;
};

//static void DestroySemaphore(const VkDevice& Device, const VkSemaphore& Semaphore);

static void DestroySemaphoreArray(const VkDevice& Device, std::vector<VkSemaphore>& Array)
{
	for (int i = static_cast<int>(Array.size()) - 1; i >= 0; i--)
	{
		vkDestroySemaphore(Device, Array[i], nullptr);
	}
	Array.erase(Array.begin(), Array.end());
}

static VkCommandPool CreateCommandPool(const VkDevice* LogicalDevice, uint32_t PresentationSuitableQueueFamilyIndex)
{
	VkCommandPool NewCommandPool = {};

	VkCommandPoolCreateInfo PoolInfo = {};

	PoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	PoolInfo.queueFamilyIndex = PresentationSuitableQueueFamilyIndex;
	PoolInfo.flags = 0; // Optional

	if (vkCreateCommandPool(*LogicalDevice, &PoolInfo, nullptr, &NewCommandPool) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Command pool creation failed.");
	}

	return NewCommandPool;
}

static VkSemaphore CreateSemaphore(const VkDevice* Device)
{
	VkSemaphore NewSemaphore = {};

	VkSemaphoreCreateInfo CreateInfo = {};
	CreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(*Device, &CreateInfo, nullptr, &NewSemaphore) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Render pass frame rendering finish semaphore creation failed!");
	}

	return NewSemaphore;
}

static void CreateAndAllocateCommandBuffers
(
	const VkDevice* LogicalDevice,
	std::vector < VkCommandBuffer >& BufferArray,
	const VkCommandPool* CommandPoolHandle,
	size_t BufferSize
)
{
	BufferArray.resize(BufferSize);

	VkCommandBufferAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	AllocInfo.commandPool = *CommandPoolHandle;
	AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	AllocInfo.commandBufferCount = uint32_t(BufferArray.size());

	if (vkAllocateCommandBuffers(*LogicalDevice, &AllocInfo, BufferArray.data()) != VK_SUCCESS)
	{
		LogVk(LogType::Error, 0, "Command buffer allocation failed.");
	}
}
#endif // VULKANUTILITIES_H