#version 430 core

//vertex attributes (both are defined in model space)
layout(location = 0) in vec4 pos;    
layout(location = 1) in vec3 normal;

//transformation matrices
layout (location = 0) uniform mat4 w2v_matrix;
layout (location = 1) uniform mat4 proj_matrix;

//material&light attributes
//Note:
// ambient:  material_ambient * light_ambient;
// diffuse:  material_diffuse * light_diffuse;
// specular: material_specular * light_specular;
   
layout (location = 2) uniform vec3  ambient 			= vec3(0.0,0.0,0.0);
layout (location = 3) uniform vec3  diffuse 			= vec3(0.0,0.0,0.0);
layout (location = 4) uniform vec3  specular 			= vec3(1.0,1.0,1.0);
layout (location = 5) uniform float specular_power 		= 2;				//a default traditional values for specular power
layout (location = 6) uniform vec4  light_pos 			= vec4(0,1,1,1); 	//defined in world space


//output to fragment shader
out VS_OUT
{
	vec3 color;
}vs_out;

void main(void)                             
{
	//compute L, N, P vectors in view space
	vec4 P = w2v_matrix * pos;
	vec3 N = (w2v_matrix * vec4(normal, 0)).xyz;
	vec3 L = (w2v_matrix * light_pos - P).xyz;
	vec3 E = -P.xyz;
	
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