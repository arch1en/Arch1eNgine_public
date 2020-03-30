#ifndef VULKANUTILITIES_H
#define VULKANUTILITIES_H

#include "GeometrySystem/Vertex.h"

#include <glm/mat4x4.hpp>

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

// [Todo][Critical] This should definitely be implemented.
//class VulkanCommons
//{
//	VulkanCommons()
//	{
//		if (IsInstanceAlreadyCreated())
//		{
//			LogVk(LogType::Fatal, "An attempt of creating a second singleton object is beign made!");
//		}
//	}
//	VulkanCommons(VulkanCommons&) = delete;
//	VulkanCommons(VulkanCommons&&) = delete;
//	VulkanCommons& operator=(VulkanCommons&) = delete;
//
//public:
//
//	static VulkanCommons& Get()
//	{
//		if (Instance == nullptr)
//		{
//			Instance = new VulkanCommons();
//		}
//		return *Instance;
//	}
//
//	// Do not modify this variale!
//	static VulkanCommons* Instance;
//
//	void SetLogicalDevice(const VkDevice* Param)
//	{
//		LogicalDevice = Param;
//	}
//
//	void SetPhysicalDevice(const VkPhysicalDevice* Param)
//	{
//		PhysicalDevice = Param;
//	}
//
//	void SetGraphicQueue(const VkQueue* Param)
//	{
//		GraphicQueue = Param;
//	}
//
//	void SetSingleTimeCommandPool(const VkCommandPool* Param)
//	{
//		SingleTimeCommandPool = Param;
//	}
//
//	const VkDevice& GetLogicalDevice()
//	{
//		return *LogicalDevice;
//	}
//
//	const VkPhysicalDevice& GetPhysicalDevice()
//	{
//		return *PhysicalDevice;
//	}
//
//	const VkQueue& GetGraphicQueue()
//	{
//		return *GraphicQueue;
//	}
//
//	const VkCommandPool& GetSingleTimeCommandPool()
//	{
//		return *SingleTimeCommandPool;
//	}
//
//private:
//
//	const VkDevice* LogicalDevice;
//	const VkPhysicalDevice* PhysicalDevice;
//	const VkQueue* GraphicQueue;
//	const VkCommandPool* SingleTimeCommandPool;
//
//	bool IsInstanceAlreadyCreated()
//	{
//		return Instance != nullptr;
//	}
//};

#endif // VULKANUTILITIES_H