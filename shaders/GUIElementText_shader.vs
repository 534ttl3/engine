#version 410
 
in vec3 vPosition;
in vec2 vTexCoord;

out vec2 TexCoord;
 
void main(void)
{
	gl_Position = vec4(vPosition.xyz, 1.0);
	TexCoord = vTexCoord;
}