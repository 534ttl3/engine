#version 400
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 Color;
uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform float Shininess; // Specular shininess factor

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

vec3 ads( vec4 position, vec3 norm )
{
	vec3 s;
	if( LightPosition.w == 0.0 )	// if directional light
		s = normalize(vec3(LightPosition));
	else
		s = normalize(vec3(LightPosition - position));
	vec3 v = normalize(vec3(-position));
	vec3 r = reflect( -s, norm );
	return
		LightIntensity * ( Ka +
		Kd * max( dot(s, norm), 0.0 ) +
		Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}

void main()
{
	mat4 MV = V*M;
	mat3 N = mat3(transpose(inverse(MV)));
	
	vec3 eyeNorm = normalize( N * vNormal);
	vec4 eyePosition = MV * vec4(vPosition,1.0);
	// Evaluate the lighting equation
	Color = ads( eyePosition, eyeNorm );
	gl_Position = MV * P * vec4(vPosition,1.0);
}