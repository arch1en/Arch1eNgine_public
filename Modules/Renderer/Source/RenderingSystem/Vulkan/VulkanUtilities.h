#ifndef VULKANUTILITIES_H
#define VULKANUTILITIES_H

#include "GeometrySystem/Vertex.h"

struct BufferData
{
	VkBuffer mBuffer;
	VkBufferCreateInfo mBufferCreateInfo = {};
	VkDeviceMemory mBufferMemory;
	std::vector<Vertex> mVertices;
};

#endif // VULKANUTILITIES_H