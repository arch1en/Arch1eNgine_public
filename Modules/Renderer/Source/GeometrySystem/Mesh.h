#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Vertex.h"

struct Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<uint16_t> Indices;
	uint8_t VerticesPerFace = 3;
};

struct PrimitivePlaneMesh : public Mesh 
{
	PrimitivePlaneMesh() 
	{
		Vertices = {
				{{-0.5f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}, {1.f, 0.f}},
				{{0.5f, -0.5f, 0.f}, {0.0f, 1.0f, 0.0f}, {0.f, 0.f}},
				{{0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}, {0.f, 1.f}},
				{{-0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}}
		};

		Indices = {
			0, 1, 2, 2, 3, 0
		};
	}
};

#endif //MESH_H
