#version 410

in vec3 vPosition;
in vec4 vColor;

out vec4 Color;

void main()
{
	gl_Position = vec4(vPosition.xyz, 1.0);
	Color = vColor;
}