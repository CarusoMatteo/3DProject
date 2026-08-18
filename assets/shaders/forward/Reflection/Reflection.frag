#version 330 core

uniform vec3 viewPosition;
uniform samplerCube skyboxSampler;

in vec3 normal;
in vec3 position;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 mainColor;
layout (location = 2) out vec4 normalColor;
layout (location = 3) out vec4 depthColor;
layout (location = 4) out vec4 velocityColor;

const float nearPlane = 0.1;
const float farPlane = 75.0;

float linearizeDepth(float depth)
{
	// Transform the depth buffer from [0,1] to normalized device coordinates [-1,1]
	float z = depth * 2.0 - 1.0;
	// Apply the linearization formula
	return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane)) / farPlane;
}

void main()
{
	// View direction
	vec3 E = normalize(position - viewPosition);
	// Reflection vector
	vec3 R = reflect(E, normalize(normal));
	// Cubemap sampling
	fragColor = mainColor = texture(skyboxSampler, R);
	normalColor = vec4(normal, 1);
	depthColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)), 1);
}
