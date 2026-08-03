#version 330 core

uniform vec3 viewPosition;
uniform samplerCube skyboxSampler;

in vec3 normal;
in vec3 position;

out vec4 fragColor;

void main()
{
	// Calcolo direzione di vista
	vec3 E = normalize(position - viewPosition);
	// Calcolo vettore riflesso
	vec3 R = reflect(E, normalize(normal));
	// Campionamento della cubemap
	fragColor = texture(skyboxSampler, R);
}