#version 430 core

in vec2 texCoord;

layout(location = 0) out vec4 outColor;

uniform sampler2D image;

void main(void)
{
	outColor = texture(image, texCoord);
}