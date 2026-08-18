#version 330 core
uniform samplerCube skyboxSampler;

in vec3 fragmentTextureCoordinate;

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
	fragColor = mainColor = texture(skyboxSampler, fragmentTextureCoordinate);
	normalColor = vec4(0, 0, 1, 1);
	depthColor = vec4(1.0);
}
