#version 410
in vec3 vPosition;
in vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 eyePosition;
out vec3 eyeNormal;

void main()
{
	mat4 MV = V*M;
	mat3 N = mat3(transpose(inverse(MV)));
	
	eyeNormal = normalize( N * vNormal);
	eyePosition = vec3( MV * vec4(vPosition, 1.0) );
	
	gl_Position = P * MV * vec4(vPosition, 1.0);
}
