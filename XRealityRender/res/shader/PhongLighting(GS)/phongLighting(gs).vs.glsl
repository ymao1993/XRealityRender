#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec3 pos;    
layout(location = 1) in vec3 normal;

//transformation matrices
layout (location = 0) uniform mat4 m2w_matrix;
layout (location = 1) uniform mat4 w2v_matrix;
layout (location = 2) uniform mat4 proj_matrix;

layout(std140, binding = 0) uniform Material{
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
	float spower;
}material;

layout(std140, binding = 1) uniform PointLight{
	vec3  pos;
	vec3  ambient;
	vec3  diffuse;
	vec3  specular;
}pointLight;

//output to fragment shader
out VS_OUT
{
	vec3 color;
}vs_out;

void main(void)                             
{
	vec3  ambient = pointLight.ambient * material.ambient;
	vec3  diffuse = pointLight.diffuse * material.diffuse;
	vec3  specular = pointLight.specular * material.specular;
	float specular_power = material.spower;
	vec3  light_pos = pointLight.pos;

	//Comput L, N, P, E in view space
	mat4 m2v_matrix = w2v_matrix * m2w_matrix;
	vec4 Pw = m2w_matrix * vec4(pos, 1); Pw.xyz /= Pw.w; Pw.w = 1;
	vec4 P  = w2v_matrix * Pw; P.xyz /= P.w; P.w = 1;
	vec3 N  = (m2v_matrix * vec4(normal, 0)).xyz;
	vec3 L  = (w2v_matrix * vec4(light_pos - Pw.xyz,0)).xyz;
	vec3 E  = -P.xyz;
	
	N = normalize(N);
	L = normalize(L);
	E = normalize(E);
	
	/****compute lighting using Phong model (with Gouraud shading)****/
	vec3 color_diffuse = diffuse * max(dot(N, L),0);//diffuse light
	vec3 color_specular = specular * pow(max(dot(reflect(-L, N), E),0), specular_power);//specular light
	vs_out.color = ambient + color_diffuse + color_specular;
	
	//transform the position from view space to clip space
	gl_Position = proj_matrix * P;
}