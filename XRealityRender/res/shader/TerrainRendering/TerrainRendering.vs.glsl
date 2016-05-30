#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;

//output to tessellation control shader
out VS_OUT
{
	vec2 txc;
}vs_out;

void main(void)                             
{
	//sample color
	vs_out.txc = texcoord;
	
	//pass the position along the pipeline. 
	//The coordinates transformation is handled by tessellation evaluation shader
	gl_Position = vec4(pos,1);
}