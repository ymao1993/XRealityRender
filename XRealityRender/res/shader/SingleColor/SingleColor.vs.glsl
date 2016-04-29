#version 430 core

layout(location = 0) in vec3 pos;    

//model-to-clip space transformation matrix
layout(location = 0) uniform mat4 m2c;

void main(void)                             
{
	gl_Position = m2c * vec4(pos,1);
}