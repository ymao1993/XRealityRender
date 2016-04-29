#version 430 core

layout(location = 1) uniform vec4 ucolor;

out vec4 color;

void main(void)
{
	color = ucolor;
}