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

#endif // VULKANUTILITIES_H