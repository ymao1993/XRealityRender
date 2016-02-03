#version 420 core

//the number of vertices per patch produced by the tessellation control shader

layout(vertices = 3) out;

void main(void)
{
	if(gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = 5.0;
		gl_TessLevelOuter[0] = 5.0;
		gl_TessLevelOuter[1] = 5.0;
		gl_TessLevelOuter[2] = 5.0;
	}
	
	//just simple copy the vertex data to the next stage
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	
}