#version 410
in vec3 eyePosition;
in vec3 eyeNormal;

// light
uniform vec3 LightPosition;
uniform vec3 LightIntensity;

// material
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

layout( location = 0 ) out vec4 FragColor;

vec3 ads()
{
	vec3 n = normalize( eyeNormal );
	vec3 s = normalize( vec3(LightPosition) - eyePosition );
	vec3 v = normalize(vec3(-eyePosition));
	vec3 r = reflect( -s, n );
	return
		LightIntensity *
		( Ka +
		Kd * max( dot(s, n), 0.0 ) +
		Ks * pow( max( dot(r,v), 0.0 ), Shininess ) );
}
void main()
{
	FragColor = vec4(ads(), 1.0);
}