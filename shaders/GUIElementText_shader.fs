#version 410

in vec2 TexCoord;

uniform sampler2D Tex1;

out vec4 FragColor;

void main()
{
	FragColor = texture(Tex1, TexCoord);
}