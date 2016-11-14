#version 430 core

in VS_OUT
{
	vec3 posw;
	vec3 normalw;
} fs_in;

out vec4 color;

layout (location = 3) uniform vec3 eye;
layout (location = 4) uniform int option; // 0 = reflection, 1 = refraction
layout (binding = 0) uniform samplerCube sampler;

void main(void)
{
	vec3 inc = fs_in.posw - eye;


	vec3 ref;
	if(option == 0)
	{
		ref = reflect(inc, fs_in.normalw);
	}
	else if(option == 1)
	{
		float ratio = 1.00 / 1.52;
		ref =  refract(inc, fs_in.normalw, ratio);
	}

	color = texture(sampler, ref);
}