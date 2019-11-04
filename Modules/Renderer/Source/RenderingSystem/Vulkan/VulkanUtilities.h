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

struct UniformBufferObject
{
	glm::mat4 Model;
	glm::mat4 View;
	glm::mat4 Projection;
};

#endif // VULKANUTILITIES_H