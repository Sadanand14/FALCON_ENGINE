#version 450 core

out vec2 out_UV;

void main()
{
	out_UV = vec2(
	(gl_VertexID<<1) & 2,
	(gl_VertexID & 2) * -1);

	gl_Position = vec4(out_UV , 0 , 1);
	gl_Position.x =  gl_Position.x * 2 - 1;
	gl_Position.y = gl_Position.y * 2 + 1;
	
}