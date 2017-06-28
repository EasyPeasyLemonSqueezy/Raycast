#version 430 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexture;

out vec2 texCoord;

void main(void)
{
    texCoord = inTexture;

	gl_Position = vec4(inPosition, 0, 1);
}