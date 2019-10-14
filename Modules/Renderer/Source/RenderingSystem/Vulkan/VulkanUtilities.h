#ifndef VULKANUTILITIES_H
#define VULKANUTILITIES_H

#include "GeometrySystem/Vertex.h"

struct BufferData
{
	VkBuffer mBuffer;
	VkBufferCreateInfo mBufferCreateInfo = {};
	VkDeviceMemory mBufferMemory;
};

struct VertexBufferData
{
	BufferData mBufferData;
	std::vector<Vertex> mVertices;
};

#endif // VULKANUTILITIES_H