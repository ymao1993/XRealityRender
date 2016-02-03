#version 420 core

//the geometry shader would expect triangles as input
layout(triangles) in;

//the geometry shader would emit points and the maximum number of points emmited would be 3
layout(points, max_vertices = 3) out;

void main(void)
{
	int i;
	for (i =0; i< gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
}