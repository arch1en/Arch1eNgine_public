#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec4 VertColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

void main() 
{
	TexCoord = aTexCoord;

	VertColor = color;

	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
