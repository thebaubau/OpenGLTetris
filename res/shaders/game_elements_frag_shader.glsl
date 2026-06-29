#version 460 core
in vec2 texCoords;

out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main()
{
    color = texture(image, texCoords);
}