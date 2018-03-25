#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 VertColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	TexCoord = aTexCoord;

	VertColor = vec3(1.0, 0.0, 0.0);

	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
