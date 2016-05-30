#version 430 core

layout(vertices = 3) out;

in VS_OUT
{
	vec2 txc;
}vs_out[];

out TCS_OUT
{
	vec2 txc;
}tcs_out[];

void main()
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 5.0;
		gl_TessLevelOuter[0] = 8.0;
		gl_TessLevelOuter[1] = 8.0;
		gl_TessLevelOuter[2] = 8.0;
	}
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID].txc = vs_out[gl_InvocationID].txc;
}