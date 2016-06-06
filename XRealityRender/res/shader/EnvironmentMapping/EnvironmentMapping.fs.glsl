#version 430 core

uniform samplerCube sampler;

in vec3 sampleDir;

out vec4 color;

void main()
{
	color =  texture(sampler,sampleDir);
}
