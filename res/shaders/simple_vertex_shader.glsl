#version 460 core

layout (location = 0) in vec3 aPos;

out vec4 vertexColor;

void main() {
	vertexColor = vec4(0.4, 0.2, 0.5, 1.0);
	gl_Position = vec4(aPos, 1.0);
}