#version 420 core

out vec3 color;

//use interface block to group data
in VS_OUT
{
	vec3 color;
}fs_in;


void main(void)
{
	color = fs_in.color;
}