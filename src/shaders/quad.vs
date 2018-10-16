#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 uv;

out vec3 vNormal;
//out screen-space texture coordinates (?)
//out world-space positions (?)

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vNormal = abs(normal);
}