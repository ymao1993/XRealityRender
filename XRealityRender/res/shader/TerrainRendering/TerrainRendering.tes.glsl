#version 430 core

layout(triangles) in;

in TCS_OUT
{
	vec2 txc;
}tes_in[];

out TES_OUT
{
	vec2 txc;
}tes_out;

//transformation matrices
layout (location = 0) uniform mat4 m2c_matrix;
layout (binding = 0) uniform sampler2D disp_sampler;

void main(void)
{
	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position) + 
				  (gl_TessCoord.y * gl_in[1].gl_Position) +
				  (gl_TessCoord.z * gl_in[2].gl_Position);

	tes_out.txc = (gl_TessCoord.x * tes_in[0].txc) + 
				  (gl_TessCoord.y * tes_in[1].txc) +
				  (gl_TessCoord.z * tes_in[2].txc);

	//apply displacement and perform coordinate transformation
	gl_Position.y = texture2D(disp_sampler, tes_out.txc).x;
	gl_Position = m2c_matrix * gl_Position;
}