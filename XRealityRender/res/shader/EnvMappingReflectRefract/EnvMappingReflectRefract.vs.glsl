#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;    
layout(location = 1) in vec3 normal;

//transformation matrices
layout (location = 0) uniform mat4 m2w;
layout (location = 1) uniform mat4 w2v;
layout (location = 2) uniform mat4 proj;

//output to fragment shader
out VS_OUT
{
	vec3 posw;
	vec3 normalw;
} vs_out;

void main(void)                             
{	

	//compute position in world space
	vec4 posw = m2w * vec4(pos,1);
	posw/=posw.w;

	//compute normal in world space
	vec4 normalw = m2w * vec4(normal,0);
	normalw = normalize(normalw);

	//pass them to fragment shader
	vs_out.posw = posw.xyz;
	vs_out.normalw = normalw.xyz;

	//compute position in clip space, of course
	gl_Position = proj * w2v * posw;
	
}