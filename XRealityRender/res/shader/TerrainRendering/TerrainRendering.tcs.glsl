#version 430 core

layout (location = 0) uniform mat4 m2c_matrix;

layout(vertices = 3) out;

in VS_OUT
{
	vec2 txc;
}vs_out[];

out TCS_OUT
{
	vec2 txc;
}tcs_out[];

vec2 decideTessLev();

void main()
{
	if(gl_InvocationID == 0)
	{
		vec2 levs = decideTessLev();
		gl_TessLevelInner[0] = levs[0];
		gl_TessLevelOuter[0] = levs[1];
		gl_TessLevelOuter[1] = levs[1];
		gl_TessLevelOuter[2] = levs[1];
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID].txc = vs_out[gl_InvocationID].txc;
}

//
// This function decides the level of tessellation
//
vec2 decideTessLev()
{
	vec2 result = vec2(5, 8);
	return result;
}
