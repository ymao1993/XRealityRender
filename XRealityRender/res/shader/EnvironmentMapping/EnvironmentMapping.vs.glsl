#version 430 core

in vec3 pos;

layout(location = 0) uniform mat4 w2c;

out vec3 sampleDir;

void main()
{
	gl_Position = w2c * vec4(pos,1);
	sampleDir = pos;
}