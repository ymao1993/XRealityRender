#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;    
layout(location = 1) in vec2 txcoords;

//model-to-clip space transformation matrix
layout(location = 0) uniform mat4 m2c;

//output to fragment shader
out VS_OUT
{
	vec2 txcoords;
}vs_out;

void main(void)                             
{
	gl_Position = m2c * vec4(pos,1);
	vs_out.txcoords = txcoords;
}