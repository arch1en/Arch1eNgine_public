#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm.hpp>
#include "Vertex.h"

class Mesh
{
	std::vector<Vertex> Vertices = {
		{{0.f, -0.5f, 0.f},{1.f, 0.f, 0.f}},
		{{0.5f, 0.5f, 0.f},{0.f, 1.f, 0.f}},
		{{-0.5f, 0.5f, 0.f},{1.f, 0.f, 1.f}},
	};
}
#endif //MESH_H
