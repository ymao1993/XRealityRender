#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;    
layout(location = 1) in vec2 txcoords;

//model-to-clip space transformation matrix
layout(location = 0) uniform mat4 m2w;
layout(location = 1) uniform mat4 w2c;

//instanced vertex attributes
layout(location = 2) in vec3 instanced_position;

//output to fragment shader
out VS_OUT
{
	vec2 txcoords;
}vs_out;

void main(void)                             
{
	vec4 wpos = m2w * vec4(pos,1);
	wpos /= wpos.w;
	wpos.w = 1;
	gl_Position = w2c * (wpos + vec4(instanced_position,1));
	vs_out.txcoords = txcoords;
}