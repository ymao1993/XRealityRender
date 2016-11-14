#version 430 core

layout (binding = 1) uniform sampler2D color_sampler;

out vec3 color;

//use interface block to group data

in TES_OUT
{
	vec2 txc;
}fs_in;


void main(void)
{
	color = texture2D(color_sampler, fs_in.txc).xyz;
}