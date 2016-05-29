#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;

//transformation matrices
layout (location = 0) uniform mat4 m2c_matrix;

layout (binding = 0) uniform sampler2D disp_sampler;
layout (binding = 1) uniform sampler2D color_sampler;

//output to fragment shader
out VS_OUT
{
	vec3 color;
}vs_out;

void main(void)                             
{
	//apply displacement mapping
	vec3 pos_real = pos;
	pos_real.y = texture2D(disp_sampler, texcoord).x;

	//sample color
	vs_out.color = texture2D(color_sampler, texcoord).xyz;
	
	//transform the position from view space to clip space
	gl_Position = m2c_matrix * vec4(pos_real, 1);
}