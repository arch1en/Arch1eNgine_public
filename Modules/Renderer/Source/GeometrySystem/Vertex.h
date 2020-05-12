#ifndef VERTEX_H
#define VERTEX_H

#include <array>

#include <glm/glm.hpp>

struct Vertex
{
public:

	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoord;
};

#endif //VERTEX_H
