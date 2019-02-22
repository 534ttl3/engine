#version 410
in vec3 vPosition;

out vec4 fColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	fColor = vec4(1.0, 1.0, 1.0, 1.0);
	mat4 MVP = P*V*M;
	gl_Position = MVP * vec4(vPosition, 1.0);
}
