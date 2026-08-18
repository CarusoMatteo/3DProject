#version 330 core

uniform vec3 viewPosition;
uniform samplerCube skyboxSampler;

in vec3 normal;
in vec3 position;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;

void main()
{
	// View direction
	vec3 E = normalize(position - viewPosition);
	// Reflection vector
	vec3 R = reflect(E, normalize(normal));
	// Cubemap sampling
	fragColor = mainColor = texture(skyboxSampler, R);
	normalColor = vec4(normal, 1);
	depthColor = vec4(gl_FragCoord.zzz, 1);
}
