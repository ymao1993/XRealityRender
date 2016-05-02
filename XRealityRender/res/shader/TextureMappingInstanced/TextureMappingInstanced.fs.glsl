#version 430 core

layout(location = 2, binding = 0) uniform sampler2D sampler;

in VS_OUT
{
	vec2 txcoords;
}fs_in;

out vec4 color;

void main(void)
{
	color = texture2D(sampler,fs_in.txcoords);
}