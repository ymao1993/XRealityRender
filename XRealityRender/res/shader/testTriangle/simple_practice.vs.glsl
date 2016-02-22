#version 430 core

//vertex attributes
layout(location = 0) in vec4 pos;

//uniform variable
layout(location = 0) uniform mat4 transform;

//use interface block to group data
out VS_OUT
{
	vec4 color;
}vs_out;

vec4 colors[] = vec4[3](vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1));

void main(void)                             
{								  
	gl_Position = transform * pos;
	vs_out.color = colors[gl_VertexID%3];
}