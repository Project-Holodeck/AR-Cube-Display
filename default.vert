#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * vec4(aPos.x, aPos.z, aPos.y, 1.0);
	color = aColor;
}