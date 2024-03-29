#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec3 norm;

out vec3 color;
out vec3 normal;
out vec3 position;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
	gl_Position = projection * view * model * transform * vec4(aPos, 1.0);
	color = aCol;
	normal = norm;
	position = aPos;
}